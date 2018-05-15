# CC and CFLAGS are varilables
CC = g++
CFLAGS = -c
AR = ar
ARFLAGS = rcv
# -c option ask g++ to compile the source files, but do not link.
# -g option is for debugging version
# -O2 option is for optimized version
DBGFLAGS = -g -D_DEBUG_ON_
OPTFLAGS = -O2

all	: bin/WorkerAnt
	@echo -n ""

# optimized version
bin/WorkerAnt	: WorkerAnt_opt.o main_opt.o lib
			$(CC) $(OPTFLAGS) WorkerAnt_opt.o main_opt.o -ltm_usage -Llib -o bin/WorkerAnt
main_opt.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
WorkerAnt_opt.o	: src/WorkerAnt.cpp src/WorkerAnt.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : bin/WorkerAnt_dbg
	@echo -n ""

bin/WorkerAnt_dbg	: WorkerAnt_dbg.o main_dbg.o lib
			$(CC) $(DBGFLAGS) WorkerAnt_dbg.o main_dbg.o -ltm_usage -Llib -o bin/WorkerAnt_dbg
main_dbg.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
WorkerAnt_dbg.o	: src/WorkerAnt.cpp src/WorkerAnt.h
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

