#!/bin/sh
set -e -u -x

# MAX_STRING_SIZE = (127+1)  default
# Maximum length of string including terminator.

# MAX_NUM_ITERS = 1000  default
# Maximum number of uncontrollable or controllable event loop iterations before deciding it may be an infinite loop.

# PRINT_OUTPUT
# If set, 'print' statements are forwarded to NET_PrintOutput.

# EVENT_OUTPUT
# If set, performed events are reported to NET_EventOutput

# CHECK_RANGES (recommended to set to 1)
# If set, enable checking of integer in partly overlapping ranged integers (as defined by CIF).

# KEEP_RUNNING
# If set, do not abort on range errors and bad continuous variable updates.
# Note that most other checks are in assert() which are not controlled by this flag.

COMPILE_OPTIONS="-DPRINT_OUTPUT=1 -DEVENT_OUTPUT=1 -DCHECK_RANGES=1"
CFLAGS="-Wall -std=c99 -g $COMPILE_OPTIONS"
LFLAGS="-lm"

gcc $CFLAGS -c -o NET_library.o NET_library.c
gcc $CFLAGS -c -o NET_engine.o NET_engine.c
gcc $CFLAGS -c -o NET_test_code.o NET_test_code.c
gcc $CFLAGS -o NET_engine_test NET_test_code.o NET_engine.o NET_library.o $LFLAGS
