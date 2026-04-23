The 'NET_*' files here are generated using the CIF to C99 code generator.
Below is a description of what it contains, and how to use and/or tailor them
towards your wishes.

Generated files
===============
* NET_library.c and NET_library.h
  Runtime support code for the CIF data types in C99. This code is the same
  for every generated set of files. Generating it however simplifies
  distribution and maintenance (you never get out-of-date files).

* NET_engine.c and NET_engine.h
  The 'real' code. includes the above library files. The code is
  self-supporting, except for a few external functions discussed below.

* NET_compile.sh
  Simple compile script to compile everything with gcc in C99.

* NET_test_code.c
  Example usage and test compile code.

* NET_readme.txt
  This file.

User-defined functions
======================
To integrate the generated code into the world, and use it as a controller,
the following functions need to be considered.

* NET_AssignInputVariables(void)
  When called, the input variables of the model should be filled with their
  current values. If the CIF model has no input variables, no call is
  performed.

* NET_InfoEvent(NET_Event_ event, BoolType pre)
  Announcement or report of an event taking place. Can be disabled with a
  compile-time flag.

* NET_PrintOutput(const char *line, const char *fname)
  A CIF print statement was performed, the text 'line' should be written into
  'fname'. The latter may also be a pseudo-filename, like ':stdout'. Can be
  disabled with a compile-time flag.

Usage API
=========
The generated code provides two functions that should be called when the
program should progress.

* NET_EngineFirstStep(void)
  Initializes the internal data, reads the input variables, and performs
  events until the first time delay step.

* NET_EngineTimeStep(double delta)
  After a non-zero delay step 'delta', the engine updates its continuous
  variables, and performs events to update its internal state.

Since a control system is always on, there is no 'shutdown' code.

Compile-time options
====================
Several pre-processor symbols are available to influence the compilation result.

* MAX_STRING_SIZE
  Length of a string in characters, including the termination character. Is
  used both by the library and the engine code.

* EVENT_OUTPUT
  If true (-DEVENT_OUTPUT=1), execution of events is reported through the
  external "NET_InfoEvent" function. There is no CIF equivalent of this
  function.

* PRINT_OUTPUT
  If true (-DPRINT_OUTPUT=1), print statements are performed as specified in
  CIF. The results are routed to the external "NET_PrintOutput" function.

* CHECK_RANGES
  If set (recommended, -DCHECK_RANGES=1), the generated code checks the copied
  integers between partially overlapping integer ranges. For example

    int[1..5] i; int[2..10] j;
    ...
    i = j; // Should abort if j > 5

* MAX_NUM_ITERS
  If defined, its value is the maximum number of uncontrollable or controllable
  event loop iterations that can be taken after each time step. In CIF there is
  no maximum. In this implementation however, the maximum avoids livelock.
  Behavior of the specification becomes however undefined. Default value is 1000.

* KEEP_RUNNING
  The CIF language checks for model correctness with every operation. When an
  error is detected, the normal procedure is to abort execution. In some
  cases it may be desirable to continue running instead. Adding this flag
  disables a few abort causes. The remaining abort check are in assertion
  checks. While these can be disabled as well, most of them detect fatal
  programming errors where the program is doomed already.
