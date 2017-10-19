#define LASSERT_MANUAL_MAIN
#include "../LAssert.h"

TEST_SECTION(tralala)
#ifndef LASSERT_MANUAL_MAIN
{
#endif
    
    TEST_CASE(tralalatsointsoin){
	unsigned i = 1;

	REQUIRE_CASE(i == 1);
	REQUIRE_CASE(i == 0);
    }

    REQUIRE( 1 );

    TEST_CASE(tralalapastsointsoin){
	unsigned j = 3;

	REQUIRE_CASE(j);
	REQUIRE_CASE(j);
	REQUIRE_CASE(j);
	REQUIRE_CASE(j);
    }

    TEST_CASE(plop){
	REQUIRE_CASE_NOT_NULL( NULL );
    }

#ifndef LASSERT_MANUAL_MAIN
}
#else
END_SECTION
#endif

TEST_SECTION(un_bon_gros_test)
#ifndef LASSERT_MANUAL_MAIN
{
#endif
    TEST_CASE(on_l_appel_baudhuit){
	unsigned * j = malloc(sizeof(*j));

	REQUIRE_CASE_NOT_NULL(j);

	printf("j == %d\n",*j);

	free(j);
    }
#ifndef LASSERT_MANUAL_MAIN
}
#else
END_SECTION
#endif

TEST_SECTION(un_test_vide)
#ifndef LASSERT_MANUAL_MAIN
{
#endif
#ifndef LASSERT_MANUAL_MAIN
}
#else
END_SECTION
#endif

TEST_SECTION(random_test)
#ifndef LASSERT_MANUAL_MAIN
{
#endif
    RAND_CASE(first_random,tab,3,3,1,10,10,100){
	puts("BEEEEGIN\n");
	for(unsigned i = 0;i < 3; ++i){
	    printf("%d\n",tab[i]);
	    REQUIRE_CASE(tab[i]);
	}
    }

    RAND_CASE(should_failed,tab2,5,5,1){
    }
#ifndef LASSERT_MANUAL_MAIN
}
#else
END_SECTION
#endif

TEST_SECTION(range_test)
#ifndef LASSERT_MANUAL_MAIN
{
#endif
    RANGE_CASE(should_succeed,tab,3,1,5,1){
	for(unsigned i = 0; i < 3; ++i)
	    printf("%d ",tab[i]);
	putchar('\n');
    }

    RANGE_CASE(should_failed,tab2,3,1){
    }
#ifndef LASSERT_MANUAL_MAIN
}
#else
END_SECTION
#endif

#ifdef LASSERT_MANUAL_MAIN
int main(){
    RUN_SECTION(tralala);
    RUN_SECTION(un_bon_gros_test);
    RUN_SECTION(un_test_vide);
    RUN_SECTION(random_test);
    RUN_SECTION(range_test);
    
    return 0;
}
#endif
