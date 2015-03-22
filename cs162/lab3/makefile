# Author:  David Rigert
# Created: 1/19/2015
# Lab 3 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x ${DEBUG}
PROGS = lab3
OBJS = lab3.o

all: ${PROGS}

${PROGS}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o $@

${OBJS}: ${@:.o=.cpp}
	${CXX} ${CXXFLAGS} -c ${@:.o=.cpp}

clean:
	${RM} ${PROGS} ${OBJS}
