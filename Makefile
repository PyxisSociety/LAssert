#!/bin/makefile

EXEC=libLAssert_alloc.so
CLEAN=*.o *~ *.so



all:$(EXEC)

$(EXEC):LAssert_alloc.o
	gcc $^ -o $@ -shared

%.o:%.c
	gcc -o $@ -c $< -Wall -Wextra -fPIC



clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
