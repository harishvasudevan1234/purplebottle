#-------------------------------------------------------------------------
#
# Makefile for parser
#
#-------------------------------------------------------------------------

srcdir = .

# Linking
LIBS = -lz -lreadline -lresolv -ldl -lm 

# Compilers
CPP = gcc -E
CPPFLAGS = -I../include/ 

CC = gcc
GCC = yes
CFLAGS = -g -w -Wall -Wmissing-prototypes -Wpointer-arith -Wdeclaration-after-statement -Wold-style-definition -Wendif-labels -fno-strict-aliasing

# Kind-of compilers
YACC = bison -y
YFLAGS = 
FLEX = /usr/bin/flex
FLEXFLAGS =  $(LFLAGS)

all: a.out

override CPPFLAGS := -I$(srcdir) $(CPPFLAGS)

OBJS=sqlite3.o test.o

a.out: $(OBJS)
	$(CC) $(LIBS) $(LDFLAGS) -o a.out $(OBJS) -lpthread -ldl

sqlite3.o: sqlite3.c
	$(CC) $(LIBS) $(LDFLAGS) -c sqlite3.c

# remove executables and other stuff left behind 
clean: 
	rm -f a.out $(OBJS)
