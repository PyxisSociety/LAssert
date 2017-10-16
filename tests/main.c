#include "../LAssert.h"
#include <string.h>

void succeed(void);
void failed(void);
void pointer_alloc_failed(void);

int not_in(char * s,char c);

BEGIN_SECTION(succeed)

TEST_CASE(test1){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test2){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test3){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

END_SECTION

BEGIN_SECTION(failed)

    TEST_CASE(test1){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test2){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test3){
    int i;

    for(i = 4;; --i)
	require(i);
}
    
END_SECTION


BEGIN_SECTION(pointer_alloc_failed)

    TEST_CASE(test1){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test2){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

TEST_CASE(test3){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
    require_not_null(1);
    require_not_null(NULL);
}
END_SECTION

int main(int argc, char *argv[]){
    if(argc != 2)
	puts("Not the good number of arguments.");
    else{
	if(strlen(argv[1]) != 2 || argv[1][0] != '-' || not_in("spf",argv[1][1]))
	    fprintf(stderr,"bad argument : %s\n",argv[1]);
	else{	    
	    switch(argv[1][1]){
	    case 's':
	        RUN_SECTION(succeed);
		break;
	    case 'p':
	        RUN_SECTION(pointer_alloc_failed);
		break;
	    case 'f':
	        RUN_SECTION(failed);
		break;
	    }
	}
    }
    
    return 0;
}

int not_in(char * s,char c){
    int res = 0;
    int i;

    if(s){
	i = 0;
	while(s[i] && s[i] != c)
	    ++i;
	res = !s[i];
    }
	

    return res;
}
