/*
 * test_tinypico.cpp
 *
 *  Created on: Apr 10, 2021
 *      Author: Jacob
 */


#include "unity.h"
#include "tinypico.h"


TEST_CASE("successive calls to getInstance returns the same pointer", "[TINYPICO]")
{
	TinyPICO* tp1 = TinyPICO::getInstance();
	TinyPICO* tp2 = TinyPICO::getInstance();
	TEST_ASSERT_EQUAL(tp1, tp2);
}
