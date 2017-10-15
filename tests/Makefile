#!/bin/makefile

SRCTMP=$(wildcard ../*.c)
OFILESTMP=$(SRCTMP:../.o=.c)
SRC=$(wildcard *.c)
OFILES=$(SRC:.o=.c) $(OFILESTMP)
EXEC=a.out
LDFLAGS=-g 
CFLAGS=-Wall -Wextra -pedantic -ansi
CC=gcc
CLEAN=*.o *~
all:$(EXEC)
$(EXEC):$(OFILES)
	$(CC) $^ -o $@ $(LDFLAGS)
../%.o:../%.c
	$(CC) -c $< -o $@ $(CFLAGS)
%.o:%.c
	$(CC) -c $< $(CFLAGS)
clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
