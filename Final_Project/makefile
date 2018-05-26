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




all	: bin/color_route
	@echo -n ""

# optimized version
bin/color_route	: Routing_opt.o main_opt.o lib
			$(CC) $(OPTFLAGS) Routing_opt.o main_opt.o -ltm_usage -Llib -o bin/color_route
main_opt.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
Routing_opt.o	: src/Routing.cpp src/Routing.h
			$(CC) $(CFLAGS) $(OPTFLAGS) $< -o $@

# DEBUG Version
dbg : bin/color_route_dbg
	@echo -n ""

bin/color_route_dbg	: color_route_dbg.o main_dbg.o lib
			$(CC) $(DBGFLAGS) color_route_dbg.o main_dbg.o -ltm_usage -Llib -o bin/color_route_dbg
main_dbg.o 	   	: src/main.cpp lib/tm_usage.h
			$(CC) $(CFLAGS) $< -Ilib -o $@
color_route_dbg.o	: src/Routing.cpp src/Routing.h
			$(CC) $(CFLAGS) $(DBGFLAGS) $< -o $@

lib: lib/libtm_usage.a

lib/libtm_usage.a: tm_usage.o
	$(AR) $(ARFLAGS) $@ $<
tm_usage.o: lib/tm_usage.cpp lib/tm_usage.h
	$(CC) $(CFLAGS) $<

# clean all the .o and executable files
clean:
		rm -rf *.o lib/*.a lib/*.o bin/*

