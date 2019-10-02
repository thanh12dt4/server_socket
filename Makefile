#----------------------------------------------------------------------------
# COMPILE macros
#----------------------------------------------------------------------------
.c.o:
	$(CC) $(CFLAGS) -c $*.c

CFLAGS	= -g -Wall -O2 -I. -Itarget_h -iquote -D_GNU_SOURCE -D_REENTRANT

#----------------------------------------------------------------------------
# Make the program...
#----------------------------------------------------------------------------
OBJS =  \
	main.o server.o writelog.o

LIBS =
PROG = main

all:	$(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(PROG) $(LIBS)

#----------------------------------------------------------------------------
# Clean object files and executable file
#----------------------------------------------------------------------------
clean:
	rm -f *.o $(PROG)

