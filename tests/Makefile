#!/bin/makefile

EXEC=ctest.out
EXECPP=cpptest.out
CLEAN=*.o *~



all:$(EXEC)

$(EXEC):main.c.o
	gcc $^ -o $@ -g

main.c.o:main.c
	gcc -o $@ -c $< -Wall -Wextra



cpp:$(EXECPP)

$(EXECPP):main.cpp.o
	g++ $^ -o $@ -g

main.cpp.o:main.cpp
	g++ -o $@ -c $< -Wall -Wextra



clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm $(EXEC) $(CLEAN)
