# Model Based Synthesis and Validation of High Performance Supervisory Controllers for Embedded Systems

**Master's Thesis** — Stavros Martini

Hellenic Mediterranean University, Department of Electrical and Computer Engineering

Supervisor: Prof. George Kornaros

May 2026

---

## Abstract

This thesis presents a two-layer embedded control architecture that couples formal supervisory synthesis with classical real-time motor control. The upper layer uses the Eclipse ESCET toolkit and the CIF specification language to model system behavior as Extended Finite Automata. Safety and coordination requirements are expressed as formal constraints, and a supervisor is synthesized automatically with mathematical guarantees of controllability, non-blocking, and maximal permissiveness. The lower layer handles real-time field-oriented control (FOC) of BLDC motors using the SimpleFOC library at approximately 20 kHz, independently of the network supervisor.

The architecture is validated on a physical two-node BLDC motor network connected via CAN 2.0B, implementing a Heat Recovery Ventilation (HRV) system. The application required 46 formal requirements covering coordinated startup, sequential calibration, fault recovery with bounded retries, and balanced fan operation. Electrical faults, thermal faults, and power loss were injected on live hardware. In all cases the supervisor behaved exactly as the formal model predicted.

## System Overview

The system consists of three classes of participant on a shared CAN bus:

- **Supervisor node**: ARM-based single-board computer (Luckfox Lyra Plus) running the synthesized C99 supervisor application over Linux SocketCAN.
- **Motor drive nodes**: Teensy 4.0 microcontrollers each driving a BLDC motor through a DRV8302 three-phase gate driver, running SimpleFOC for real-time control and a local state machine for safety.
- **Operator interface**: Text-based terminal UI accessed via SSH.

Communication follows the CANCommander register protocol. Controllable events (supervisor commands) map to CAN register writes. Uncontrollable events (node state transitions) map to CAN event push notifications.

## Repository Structure

```
thesis/
    main.typ                 Thesis document (Typst)
    references.bib           Bibliography
    img/                     Figures and diagrams

Models/
    NODE.cif                 Node plant model (DRV8302 motor node EFA)
    Coordinator.cif          System coordinator plant model
    NET.cif                  Network instantiation (coordinator + 2 nodes)
    Requirements.cif         46 formal requirements

Synth/
    output_NET.cif           Synthesized supervisor (abstract)
    output_NET_checked.cif   Verified supervisor (with controller properties)

gen/
    NET_engine.c             Generated supervisor engine
    NET_engine.h
    NET_library.c            Generated CIF runtime library
    NET_library.h

supervisor/
    main.c                   Supervisor application entry point
    can_if.c / can_if.h      CAN interface library (SocketCAN)
    net_tui.c / net_tui.h    Terminal user interface

firmware/
    node_firmware.ino        Motor drive node firmware (Teensy 4.0)
```

## Dependencies

**Supervisor (Linux, ARM)**
- GCC with C99 support (arm-linux-gnueabihf-gcc for cross-compilation)
- Linux kernel with SocketCAN support

**Node firmware**
- Arduino / PlatformIO toolchain for Teensy 4.0
- SimpleFOC library
- FlexCAN_T4 library

**Modeling and synthesis**
- Eclipse ESCET toolkit (https://eclipse.dev/escet/)
- CIF toolset for synthesis, simulation, controller properties checking, and C99 code generation

**Thesis document**
- Typst (https://typst.app/)

## Building

**Supervisor**

Cross-compile the generated engine together with the integration code:

```
arm-linux-gnueabihf-gcc -std=c99 -O2 \
    gen/NET_engine.c gen/NET_library.c \
    supervisor/main.c supervisor/can_if.c supervisor/net_tui.c \
    -o supervisor_app
```

Deploy the resulting binary to the Luckfox board and run over SSH.

**Node firmware**

Open the firmware project in PlatformIO or the Arduino IDE configured for Teensy 4.0, install the SimpleFOC and FlexCAN_T4 libraries, and upload to each node.

**Synthesis (reproducing the supervisor from models)**

From the ESCET IDE or command line:

```
cifdatasynth("Models/Requirements.cif -o Synth/output_NET.cif");
cifcontrollercheck("Synth/output_NET.cif -o Synth/output_NET_checked.cif");
cifcodegen("Synth/output_NET_checked.cif -o gen/ -l c99 -p NET");
```

## Key Metrics

| Metric | Value |
|---|---|
| CIF model size | 844 lines |
| Requirements | 46 |
| Synthesis time | ~2.5 seconds |
| Generated C99 code | 2,392 lines |
| Hand-written integration code | 814 lines |
| Supervisor cycle | 10 ms (100 Hz) |
| Measured engine execution time | < 150 us |
| Runtime memory footprint | ~43 KB |
| Bounded response | <= 2 iterations |
| Confluence | Verified |
| Non-blocking under control | Verified |

## References

- Ramadge, P.J. and Wonham, W.M. (1987). Supervisory Control of a Class of Discrete Event Processes. SIAM Journal on Control and Optimization, 25(1), 206-230.
- Eclipse Foundation. Eclipse ESCET -- The Eclipse Supervisory Control Engineering Toolkit. https://eclipse.dev/escet/
- Skuric, A. et al. (2022). SimpleFOC: A Field Oriented Control (FOC) Library for Controlling BLDC and Stepper Motors. Journal of Open Source Software, 7(74), 4232.
- Texas Instruments. DRV8302 Three Phase Gate Driver (SLVSA73C).

## License

This repository contains the source material for a master's thesis. The CIF models, generated code, and integration code are provided for academic reference. The Eclipse ESCET toolkit is available under the Eclipse Public License. SimpleFOC is licensed under the MIT License.
