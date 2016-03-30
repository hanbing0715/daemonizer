
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
			{"run", required_argument, NULL, OPT_RUN},
			{"workdir", required_argument, NULL, OPT_WORKDIR},
			{"core", no_argument, NULL, OPT_COREDUMP},
			{"fds", required_argument, NULL, OPT_FDS},
			{"stack", required_argument, NULL, OPT_STACKSIZE},
			{"help", no_argument, NULL, OPT_HELP},
			{"stdout", required_argument, NULL, OPT_STDOUT},
			{"stderr", required_argument, NULL, OPT_STDERR},
			{0, 0, 0, 0}
	};
	int c = 0;
	while ((c = getopt_long(argc,argv,"",long_opts,NULL)) != -1)
	{
		switch(c)
		{
			case OPT_COREDUMP:
				conf.coredump = true;
				break;
			case OPT_RUN:
				conf.run = optarg;
				conf.runFlag = true;
				break;
			case OPT_WORKDIR:
				conf.workDir = optarg;
				conf.wdFlag = true;
				break;
			case OPT_FDS:
				conf.openFiles = atoi(optarg);
				conf.fdFlag = true;
				break;
			case OPT_STACKSIZE:
				conf.stackSize = atoi(optarg)*1024;
				conf.stackFlag = true;
				break;
			case OPT_HELP:
				showUsage();
				exit(EXIT_SUCCESS);
			case OPT_STDOUT:
				conf.stdoutPath = optarg;
				conf.stdoutFlag = true;
				break;
			case OPT_STDERR:
				conf.stderrPath = optarg;
				conf.stderrFlag = true;
		}
	}
	if (!conf.runFlag) //没有设置run参数
	{
		fprintf(stderr,"[%s:%d] run file is not set!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	pid_t child,sid;
	child = vfork();
	if (child < 0) //执行fork失败
	{
		fprintf(stderr,"[%s:%d] vfork error!\n",__FILE__,__LINE__);
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
		fprintf(stderr,"[%s:%d] setsid error!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}

	if (conf.coredump) enableCoredump();
	if (conf.stackFlag) setStackSize(conf.stackSize);
	if (conf.fdFlag) setOpenFiles(conf.openFiles);
	if (conf.wdFlag) setWorkPath(conf.workDir);

	close(STDIN_FILENO);

	if (conf.stdoutFlag)
	{
		redirectStdout(conf.stdoutPath);
	}
	else
	{
		close(STDOUT_FILENO);
	}
	if (conf.stderrFlag)
	{
		redirectStderr(conf.stderrPath);
	}
	else
	{
		close(STDERR_FILENO);
	}

	char * const *child_argv = { NULL };
	char * const *child_envp = { NULL };
	execve(conf.run,child_argv,child_envp);

	perror("execve");
	exit(EXIT_FAILURE);
	return 0;
}


void defaultConfig(config_t *config)
{
	config->coredump = false;
	config->run = NULL;
	config->runFlag = false;
	config->openFiles = 1024;
	config->fdFlag = false;
	config->stackSize = 8192*1024;
	config->stackFlag = false;
	config->workDir = NULL;
	config->wdFlag = false;
	config->stdoutPath = "/dev/null";
	config->stdoutFlag = false;
	config->stderrPath = "/dev/null";
	config->stderrFlag = false;
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

void setWorkPath(const char *workdir)
{
	if (chdir(workdir) < 0)
	{
		fprintf(stderr,"[%s:%d] change to workdir failed!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
}

void redirectStdout(const char *stdoutPath)
{
	FILE *fd;
	fd  = fopen(stdoutPath,"w+");
	if (NULL == fd)
	{
		fprintf(stderr,"[%s:%d] open stdout file failed!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	int ret = dup2(fileno(fd), STDOUT_FILENO);
	if (-1 == ret)
	{
		fprintf(stderr,"[%s:%d] redirect stdout failed!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	fclose(fd);
}

void redirectStderr(const char *stderrPath)
{
	FILE *fd;
	fd  = fopen(stderrPath,"w+");
	if (NULL == fd)
	{
		fprintf(stderr,"[%s:%d] open stderr file failed!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	int ret = dup2(fileno(fd), STDERR_FILENO);
	if (-1 == ret)
	{
		fprintf(stderr,"[%s:%d] redirect stderr failed!\n",__FILE__,__LINE__);
		exit(EXIT_FAILURE);
	}
	fclose(fd);
}

void showUsage()
{
	printf("daemonizer %s (%s) by <hanbing0715@gmail.com>\n\n",VERSION,DATE);
	printf("A tool can run any program in daemon,\n");
	printf("and can set some resource limit in command line.\n\n");
	printf("Usage:daemonizer --run <prog_file> [options...]\n\n");
	printf("--run <prog_file>\tThe program want to run in daemon.\n");
	printf("--core\t\t\tEnable coredump for daemon program.\n");
	printf("--fds <max_open_fd>\tSet max number of file descriptors for daemon program.\n");
	printf("--stack <stack_size>\tSet stack size of daemon program(in KiByte).\n");
	printf("--workdir <work_dir>\tSet daemon program work directory.\n");
	printf("--stdout <stdout_file>\tRedirect stdout to file.\n");
	printf("--stderr <stderr_file>\tRedirect stderr to file.\n");
	return;
}
