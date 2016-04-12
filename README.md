daemonizer
===

A tool can run any program in daemon,and can set some resource limit in command line.

Usage:
---


`daemonizer --run <prog_file> [options...]`
* `--run <prog_file>` The program want to run in daemon.
* `--core` Enable coredump for daemon program.
* `--fds <max_open_fd>` Set max number of file descriptors for daemon program.
* `--stack <stack_size>` Set stack size of daemon program(in KiByte).
* `--workdir <work_dir>` Set daemon program work directory.
* `--stdout <stdout_file>` Redirect stdout to file.
* `--stderr <stderr_file>` Redirect stderr to file.

TODO
---

* Set `LD_LIBRARY_PATH` in command line.
* Set `uid` and `gid` in command line.