#!/bin/makefile

EXEC=libLAssert_my_malloc.so
CLEAN=*.o *~



all:$(EXEC)

$(EXEC):LAssert_my_malloc.o
	gcc $^ -o $@ -shared

%.o:%.c
	gcc -o $@ -c $< -Wall -Wextra -fPIC



clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
