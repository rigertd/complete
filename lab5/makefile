# Author:  David Rigert
# Created: 2/6/2015
# Lab 5 makefile

CXX = g++
DEBUG = -g
CXXFLAGS = -std=c++0x $(DEBUG)
PROGS = lab5
OUTPUTS = CiphertextOutput.txt EncryptionOutput.txt UpperOutput.txt CopyOutput.txt
OBJS = Filter.o CopyFilter.o UppercaseFilter.o EncryptionFilter.o CipherTextFilter.o lab5.o

all: $(PROGS)

$(PROGS): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

$(OBJS): $(@:.o=.cpp)
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp)

clean:
	$(RM) $(PROGS) $(OBJS) $(OUTPUTS)
