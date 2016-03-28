# daemonizer
daemonizer is a tool can run any program in daemon,and can set some resource limit in command line.

Usage:daemonizer --path <prog_file> [--core] [--fds <max_open_fd>] [--stack <stack_size>]

--path <prog_file>\tThe program want to run in daemon.

--core\t\t\tEnable coredump for daemon program.

--fds <max_open_fd>\tSet max number of file descriptors for daemon program.

--stack <stack_size>\tSet stack size of daemon program(in KiByte).

