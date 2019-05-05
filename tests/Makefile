#!/bin/makefile

EXEC=ctest.out
EXECPP=cpptest.out
EXEC_ALLOC=alloctest.out
CLEAN=main.cpp *.o *~
PWD=$(shell pwd)
LIB_PATH=$(PWD)/..
LIB=$(LIB_PATH)/libLAssert_alloc.so
LDFLAGS=-g -ldl -Wl,-rpath,$(LIB_PATH) -L$(LIB_PATH) -lLAssert_alloc
CFLAGS=-DLASSERT_CUSTOM_ALLOC

CC?=gcc
CXX?=g++



all: clean
	@make c --no-print-directory
	@cp main.c main.cpp
	@make cpp --no-print-directory
	@make clean --no-print-directory

c:$(EXEC)

$(EXEC):main.c.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.c.o:main.c
	$(CC) -o $@ -c $< $(CFLAGS)



cpp:$(EXECPP)

$(EXECPP):main.cpp.o
	$(CXX) $^ -o $@ $(LDFLAGS)

main.cpp.o:main.cpp
	$(CXX) -o $@ -c $< $(CFLAGS)



clean:
	rm -f $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
