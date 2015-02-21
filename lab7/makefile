# Author:  David Rigert
# Created: 2/20/2015
# Lab 7 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
PROGS = lab7 createFiles
OUTPUTS = *.txt
OBJS = utility.o lab7.o createFiles.o

all: $(PROGS)

lab7: utility.o lab7.o
	$(CXX) $(CXXFLAGS) utility.o lab7.o -o $@

createFiles: utility.o createFiles.o
	$(CXX) $(CXXFLAGS) utility.o createFiles.o -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
