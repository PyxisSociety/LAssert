#include "../LAssert.h"
#include <string.h>

void succeed(void);
void failed(void);
void pointer_alloc_failed(void);

int not_in(char * s,char c);

int main(int argc, char *argv[])
{
    if(argc != 2)
	puts("Not the good number of arguments.");
    else{
	if(strlen(argv[1]) != 2 || argv[1][0] != '-' || not_in("spf",argv[1][1]))
	    fprintf(stderr,"bad argument : %s\n",argv[1]);
	else{
	    test_case(succeed,NULL);
	    test_case(succeed,NULL);
	    
	    switch(argv[1][1]){
	    case 's':
		test_case(succeed,NULL);
		break;
	    case 'p':
		test_case(pointer_alloc_failed,NULL);
		break;
	    case 'f':
		test_case(failed,NULL);
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

void succeed(void){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
}

void failed(void){
    int i;

    for(i = 4;; --i)
	require(i);
}

void pointer_alloc_failed(void){
    int i;

    for(i = 0; i < 4; ++i)
	require(i + 1);
    require_not_null(1);
    require_not_null(NULL);
}
