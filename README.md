# daemonizer
daemonizer is a tool can run any program in daemon,and can set some resource limit in command line.

Usage:daemonizer --path <prog_file> [--core] [--fds <max_open_fd>] [--stack <stack_size>]

--path <prog_file> The program want to run in daemon.

--core Enable coredump for daemon program.

--fds <max_open_fd> Set max number of file descriptors for daemon program.

--stack <stack_size> Set stack size of daemon program(in KiByte).

