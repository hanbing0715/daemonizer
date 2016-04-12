
PROJECT = daemonizer
C_COMPILER = gcc
C_DEBUG_FLAGS = -Wall -g
CLEAN = rm -f


all:daemonizer.o
	$(C_COMPILER) $^ $(C_DEBUG_FLAGS) -o $(PROJECT)
	
daemonizer.o:daemonizer.c daemonizer.h
	$(C_COMPILER) -c $< $(C_DEBUG_FLAGS) -o $@
	
clean:
	$(CLEAN) *.o $(PROJECT)
	
release:daemonizer.c daemonizer.h
	$(C_COMPILER) $< -o $(PROJECT)
    
install:daemonizer
	install -s $(PROJECT) /usr/bin/