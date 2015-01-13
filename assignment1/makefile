# Author:  David Rigert
# Created: 1/13/2015
# Assignment 1 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x ${DEBUG}
PROGS = lifesim
SRCS = Simulator.cpp Pattern.cpp utility.cpp menu.cpp
OBJS = ${SRCS:.cpp=.o}

all: ${PROGS}

${PROGS}: ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o $@

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c ${@:.o=.cpp}

clean:
	${RM} ${PROGS}
