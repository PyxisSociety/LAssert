#!/bin/makefile

EXEC=a.out
CLEAN=*.cpp *.o *~
PWD=$(shell pwd)
LIB_PATH=$(PWD)/../..
LIB=$(LIB_PATH)/libLAssert_alloc.so
LDFLAGS=-g -ldl -Wl,-rpath,$(LIB_PATH) -L$(LIB_PATH) -lLAssert_alloc
CFLAGS=-DLASSERT_CUSTOM_ALLOC

CC?=g++



all: clean
	@cp ../c/main.c ./main.cpp
	@cp ../c/sub.c ./sub.cpp
	@make $(EXEC) --no-print-directory

$(EXEC):main.o sub.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o:%.cpp
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -f $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
