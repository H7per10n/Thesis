#!/bin/bash
# ============================================
#   BUILD SCRIPT: ARM CROSS-COMPILE SUPERVISOR
# ============================================

set -e          # Exit on error
set -o pipefail # Catch pipeline failures

# -----------------------------
# Compile options
# -----------------------------
#load from the drive 
cp -v -rL '/home/martini/shared-drives/G:/My Drive/SCHOOL/Thesis/CIF/Project' CIF


COMPILE_OPTIONS="-DPRINT_OUTPUT=1 -DEVENT_OUTPUT=1 -DCHECK_RANGES=1"

# Paths
GEN_SRC="CIF/gen"
DEST_DIR="bin"

# Compiler flags
CFLAGS="-Wall -static -std=c99 -O2 -g $COMPILE_OPTIONS"
INCLUDES=(-I. -I"$GEN_SRC")
LFLAGS="-lm -lpthread"

# Object files
OBJ_NET_LIB="$DEST_DIR/NET_library.o"
OBJ_NET_TUI="$DEST_DIR/NET_tui.o"
OBJ_NET_ENGINE="$DEST_DIR/NET_engine.o"
OBJ_CAN_IF="$DEST_DIR/can_if.o"
OBJ_MAIN="$DEST_DIR/main.o"

# Executable
EXE_ARM="$DEST_DIR/NET_engine_arm"

# Remote target
SBC="root@192.168.123.100"
REMOTE_PATH="/tmp/NET_engine_arm"

# Create output directory
mkdir -p "$DEST_DIR"

# -----------------------------
# Helper function
# -----------------------------
run_step() {
    echo ""
    echo "--------------------------------------------"
    echo "[BUILD] $1"
    echo "--------------------------------------------"
}

# ============================================
# START BUILD
# ============================================

echo "============================================"
echo "  ARM Cross-Compile Build Script"
echo "============================================"
echo "Generated source path: $GEN_SRC"
echo "Output directory: $DEST_DIR"
echo ""

# --- Compile generated files ---
run_step "Compiling generated files"

echo " -> NET_library.c"
arm-linux-gnueabihf-gcc $CFLAGS "${INCLUDES[@]}" \
    -c "$GEN_SRC/NET_library.c" \
    -o "$OBJ_NET_LIB"

echo " -> NET_engine.c"
arm-linux-gnueabihf-gcc $CFLAGS "${INCLUDES[@]}" \
    -c "$GEN_SRC/NET_engine.c" \
    -o "$OBJ_NET_ENGINE"

# --- Compile local files ---
run_step "Compiling local files"

echo " -> can_if.c"
arm-linux-gnueabihf-gcc $CFLAGS "${INCLUDES[@]}" \
    -c can_if.c \
    -o "$OBJ_CAN_IF"

echo " -> net_tui.c"
arm-linux-gnueabihf-gcc $CFLAGS "${INCLUDES[@]}" \
    -c net_tui.c \
    -o "$OBJ_NET_TUI"

echo " -> main.c"
arm-linux-gnueabihf-gcc $CFLAGS "${INCLUDES[@]}" \
    -c main.c \
    -o "$OBJ_MAIN"

# --- Linking ---
run_step "Linking executable"

arm-linux-gnueabihf-gcc \
    "$OBJ_MAIN" \
    "$OBJ_CAN_IF" \
    "$OBJ_NET_ENGINE" \
    "$OBJ_NET_LIB" \
    "$OBJ_NET_TUI" \
    -o "$EXE_ARM" \
    $LFLAGS

# --- Size reporting ---
run_step "Memory Usage Report"

echo "[SIZE] Basic summary:"
arm-linux-gnueabihf-size "$EXE_ARM"

echo ""
echo "[SIZE] Verbose section breakdown:"
arm-linux-gnueabihf-size -A "$EXE_ARM"

echo ""
echo "[SIZE] Human-readable totals:"
arm-linux-gnueabihf-size -B "$EXE_ARM"

# --- Deploy only if everything succeeded ---
run_step "Deploying to SBC"

echo "[SCP] Copying executable..."
scp "$EXE_ARM" "$SBC:$REMOTE_PATH"

echo "[SSH] Setting executable permissions..."
ssh "$SBC" "chmod +x $REMOTE_PATH"

echo ""
echo "============================================"
echo "  BUILD SUCCESSFUL"
echo "  Executable: $EXE_ARM"
echo "  Deployed to: $SBC:$REMOTE_PATH"
echo "============================================"
