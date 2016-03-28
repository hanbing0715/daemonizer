
#include "daemonizer.h"

int main(int argc,char *argv[])
{
	if (argc == 1)
	{
		showUsage();
		exit(EXIT_SUCCESS);
	}
	config_t conf;
	defaultConfig(&conf);
	const struct option long_opts[] = {
			{"path", required_argument, NULL, 'p'},
			{"core", no_argument, NULL, 'c'},
			{"fds", required_argument, NULL, 'f'},
			{"stack", required_argument, NULL, 's'},
			{"help", no_argument, NULL, 'h'},
			{0, 0, 0, 0}
	};
	int c = 0;
	while ((c = getopt_long(argc,argv,"",long_opts,NULL)) != -1)
	{
		switch(c)
		{
			case 'c':
				conf.coredump = true;
				break;
			case 'p':
				conf.path = optarg;
				conf.pflag = true;
				break;
			case 'f':
				conf.openFiles = atoi(optarg);
				conf.fdflag = true;
				break;
			case 's':
				conf.stackSize = atoi(optarg)*1024;
				conf.sflag = true;
				break;
			case 'h':
				showUsage();
				exit(EXIT_SUCCESS);
		}
	}
	if (!conf.pflag) //没有设置path参数
	{
		fprintf(stderr,"[%s:%d] path is not set!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	pid_t child,sid;
	child = vfork();
	if (child < 0) //执行fork失败
	{
		fprintf(stderr,"[%s:%d] vfork error!",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	if (child > 0) //执行fork成功 && 主进程
	{
		fprintf(stdout,"[%s:%d] %d vfork %d success!\n",__FILE__,__LINE__,getpid(),child);
		exit(EXIT_SUCCESS);
	}
	umask(0);
	sid = setsid();
	if (sid < 0)
	{
		exit(EXIT_FAILURE);
	}
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	if (conf.coredump) enableCoredump();
	if (conf.sflag) setStackSize(conf.stackSize);
	if (conf.fdflag) setOpenFiles(conf.openFiles);

	char * const *child_argv = { NULL };
	char * const *child_envp = { NULL };
	execve(conf.path,child_argv,child_envp);

	perror("execve");
	exit(EXIT_FAILURE);
	return 0;
}


void defaultConfig(config_t *config)
{
	config->coredump = false;
	config->path = NULL;
	config->pflag = false;
	config->openFiles = 1024;
	config->fdflag = false;
	config->stackSize = 8192*1024;
	config->sflag = false;
}

void enableCoredump()
{
	struct rlimit r;
	r.rlim_cur = ULONG_MAX;
	r.rlim_max = ULONG_MAX;
	setrlimit(RLIMIT_CORE,&r);
	return;
}

void setStackSize(unsigned long stackSize)
{
	struct rlimit r;
	getrlimit(RLIMIT_STACK,&r);
	r.rlim_cur = stackSize;
	setrlimit(RLIMIT_STACK,&r);
	return;
}

void setOpenFiles(unsigned long openFiles)
{
	struct rlimit r;
	getrlimit(RLIMIT_NOFILE,&r);
	r.rlim_cur = openFiles;
	setrlimit(RLIMIT_NOFILE,&r);
	return;
}

void showUsage()
{
	printf("daemonizer %s (%s) by <hanbing0715@gmail.com>\n\n",VERSION,DATE);
	printf("A tool can run any program in daemon,\n");
	printf("and can set some resource limit in command line.\n\n");
	printf("Usage:daemonizer --path <prog_file> [--core] [--fds <max_open_fd>] [--stack <stack_size>]\n\n");
	printf("--path <prog_file>\tThe program want to run in daemon.\n");
	printf("--core\t\t\tEnable coredump for daemon program.\n");
	printf("--fds <max_open_fd>\tSet max number of file descriptors for daemon program.\n");
	printf("--stack <stack_size>\tSet stack size of daemon program(in KiByte).\n");
	return;
}
