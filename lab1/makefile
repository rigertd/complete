# Author:  David Rigert
# Created: 1/4/2015
# Lab 1 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x ${DEBUG}
PROGS = lab1-1 lab1-2

all: ${PROGS}

lab1-1:
	${CXX} ${CXXFLAGS} lab1-1.cpp -o lab1-1

lab1-2:
	${CXX} ${CXXFLAGS} h.cpp g.cpp lab1-2.cpp -o lab1-2

clean:
	${RM} ${PROGS}
