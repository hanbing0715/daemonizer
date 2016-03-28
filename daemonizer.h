
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

#define DATE "2016-3-28"
#define VERSION "0.1"

typedef struct config
{
	char *path;
	bool pflag;
	bool coredump;
	unsigned long stackSize;
	bool sflag;
	unsigned long openFiles;
	bool fdflag;
} config_t;

void defaultConfig(config_t *config);
void enableCoredump();
void setStackSize(unsigned long stackSize);
void setOpenFiles(unsigned long openFiles);
void showUsage();

#endif
