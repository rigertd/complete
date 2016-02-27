# Author:  David Rigert
# Created: 2/26/2016
# CS344 Project 3 makefile

C = gcc
CFLAGS = -Wall
PROGS = smallsh
OBJS = bgvector.o smallsh.o

all: $(OBJS)
	$(C) $(CFLAGS) $(OBJS) -o smallsh

$(OBJS): $(@:.o=.c)
	$(C) $(CFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS)

