
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>

#ifndef DAEMONIZER_H
#define DAEMONIZER_H

#define DATE "2016-3-30"
#define VERSION "0.2"

#define OPT_RUN 0
#define OPT_WORKDIR 1
#define OPT_COREDUMP 2
#define OPT_FDS 3
#define OPT_STACKSIZE 4
#define OPT_HELP 5
#define OPT_STDOUT 6
#define OPT_STDERR 7

typedef struct config
{
	char *run;
	bool runFlag;
	bool coredump;
	unsigned long stackSize;
	bool stackFlag;
	unsigned long openFiles;
	bool fdFlag;
	char *workDir;
	bool wdFlag;
	char *stdoutPath;
	bool stdoutFlag;
	char *stderrPath;
	bool stderrFlag;
} config_t;

void defaultConfig(config_t *config);
void enableCoredump();
void setStackSize(unsigned long stackSize);
void setOpenFiles(unsigned long openFiles);
void setWorkPath(const char *workpath);
void redirectStdout(const char *stdoutPath);
void redirectStderr(const char *stderrPath);
void showUsage();

#endif
