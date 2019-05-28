#!/bin/makefile

EXEC=libLAssert_alloc.so
CLEAN=*.o *~ *.so



all:$(EXEC)

$(EXEC):LAssert_alloc.o
	gcc $^ -o $@ -shared

%.o:%.c
	gcc -o $@ -c $< -Wall -Wextra -fPIC

doc:
	@rm -rf docs/*
	doxygen Doxyfile
	mv docs/html/* docs
	@rm -rf docs/html
	@rm -rf docs/latex

clean:
	rm $(CLEAN)
clear: clean
cleaner:
	rm -f $(EXEC) $(CLEAN)
	rm docs/*
