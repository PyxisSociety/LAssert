#include "../LAssert.h"

TEST_SECTION(tralala){
    
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
}
