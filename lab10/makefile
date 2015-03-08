# Author:  David Rigert
# Created: 3/7/2015
# Lab 10 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
OPTIMIZE = -foptimize-sibling-calls
PROGS = lab10 lab10_op

all: $(PROGS)

lab10: lab10.cpp
	$(CXX) $(CXXFLAGS) lab10.cpp -o $@

lab10_op: lab10.cpp
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) lab10.cpp -o $@

clean:
	$(RM) $(PROGS)
