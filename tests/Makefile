#!/bin/makefile

EXEC=ctest.out
EXECPP=cpptest.out
CLEAN=*.o *~
LIB=../libLAssert_alloc.so
PWD=$(shell pwd)
LDFLAGS=-g -ldl -Wl,-rpath,..
CFLAGS=-Wall -Wextra



all:$(EXEC)

$(EXEC):main.c.o
	gcc $^ -o $@ $(LDFLAGS)

main.c.o:main.c
	gcc -o $@ -c $< $(CLFAGS)

run:
	LD_PRELOAD=$(LIB) ./$(EXEC)



cpp:$(EXECPP)

$(EXECPP):main.cpp.o
	g++ $^ -o $@ $(LDFLAGS)

main.cpp.o:main.cpp
	g++ -o $@ -c $< $(CLFAGS)

cpprun:
	LD_PRELOAD=$(LIB) ./$(EXECPP)



clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
