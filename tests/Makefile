#!/bin/makefile

EXEC=ctest.out
EXECPP=cpptest.out
EXEC_ALLOC=alloctest.out
CLEAN=*.o *~
LIB=../libLAssert_alloc.so
PWD=$(shell pwd)
LDFLAGS=-g
CUSTOMALLOCFLAGS=-ldl -Wl,-rpath,.. -L$(PWD)/.. -lLAssert_alloc -DLASSERT_CUSTOM_ALLOC
CFLAGS=-Wall -Wextra
ADDCFLAG?=



all:$(EXEC)

$(EXEC):main.c.o
	gcc $^ -o $@ $(LDFLAGS)

main.c.o:main.c
	gcc -o $@ -c $< $(CLFAGS) $(ADDCFLAG)


alloc:
	@make $(EXEC_ALLOC) ADDCFLAG=-DLASSERT_CUSTOM_ALLOC --no-print-directory

$(EXEC_ALLOC):main.c.o
	gcc $^ -o $@ $(CUSTOMALLOCFLAGS)



cpp:$(EXECPP)

$(EXECPP):main.cpp.o
	g++ $^ -o $@ $(LDFLAGS)

main.cpp.o:main.cpp
	g++ -o $@ -c $< $(CLFAGS)



clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
