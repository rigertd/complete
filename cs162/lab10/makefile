# Author:  David Rigert
# Created: 3/7/2015
# Lab 10 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
OPTIMIZE = -O3
PROGS = lab10 lab10_o3

all: $(PROGS)

lab10: lab10.cpp
	$(CXX) $(CXXFLAGS) lab10.cpp -o $@

lab10_o3: lab10.cpp
	$(CXX) $(CXXFLAGS) $(OPTIMIZE) lab10.cpp -o $@

clean:
	$(RM) $(PROGS)
