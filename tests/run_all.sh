#!/bin/bash

### GETTING INPUTS
# 1st parameter == c compiler
if [ "$1" == "" ]; then
    echo "First parameter is needed: C compiler used"
    exit 1
fi
cCompiler="$1"
# 2nd parameter == cpp compiler
if [ "$2" == "" ]; then
    echo "Second parameter is needed: C++ compiler used"
    exit 1
fi
cppCompiler="$2"



### Generic behavior of process
function test {
    make CC=$1
    error=$?
    if [ "${error}" == "0" ]; then
        ./a.out -nt > out.txt
        diff -y ../ref.txt out.txt
        error=$?
    fi
}



### PROCESSING
error=0
if [ -d "c" ]; then
    cd c
    test "${cCompiler}"
    rm -f out.txt
    cd ..
    if [ "${error}" != "0" ]; then
        exit ${error}
    fi
else
    echo "no c directory"
    exit 1
fi
    
if [ -d "cpp" ]; then
    cd cpp
    test "${cppCompiler}"
    rm -f out.txt
    cd ..
else
    echo "no cpp directory"
    exit 2
fi

exit ${error}
