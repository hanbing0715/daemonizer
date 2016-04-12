daemonizer
===
[hanbing0715](mailto:hanbing0715@gmail.com)
--

A tool can run any program in daemon,and can set some resource limit in command line.

>Usage:<br />
>daemonizer --run <prog_file> [options...]<br />
>--run <prog_file> The program want to run in daemon.<br />
>--core Enable coredump for daemon program.<br />
>--fds <max_open_fd> Set max number of file descriptors for daemon program.<br />
>--stack <stack_size> Set stack size of daemon program(in KiByte).<br />
>--workdir <work_dir> Set daemon program work directory.<br />
>--stdout <stdout_file> Redirect stdout to file.<br />
>--stderr <stderr_file> Redirect stderr to file.<br />