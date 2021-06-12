/*
 * test_RGB_CC_LED.cpp
 *
 *  Created on: June 11, 2021
 *      Author: Jacob
 */


#include "unity.h"
#include "RGB_CC_LED.h"


#define TEST_LED_R_PIN		25
#define TEST_LED_G_PIN		26
#define TEST_LED_B_PIN		27

TEST_CASE("ledc timer gets set up correctly", "[RGB_CC_LED]")
{
	RGB_CC_LED led = RGB_CC_LED(TEST_LED_R_PIN, TEST_LED_G_PIN, TEST_LED_B_PIN);
	TEST_ASSERT_EQUAL(
		RGB_CC_LED_FREQ_HZ,
		ledc_get_freq(RGB_CC_LED_SPEED_MODE, RGB_CC_LED_TIMER_NUM)
	);
}


TEST_CASE("ledc channel gets set up correctly", "[RGB_CC_LED]")
{
	RGB_CC_LED led = RGB_CC_LED(TEST_LED_R_PIN, TEST_LED_G_PIN, TEST_LED_B_PIN);
	TEST_ASSERT_EQUAL(
		RGB_CC_LED_HPOINT,
		ledc_get_hpoint(RGB_CC_LED_SPEED_MODE, RGB_CC_LED_R_CHANNEL)
	);
	TEST_ASSERT_EQUAL(
		RGB_CC_LED_HPOINT,
		ledc_get_hpoint(RGB_CC_LED_SPEED_MODE, RGB_CC_LED_G_CHANNEL)
	);
	TEST_ASSERT_EQUAL(
		RGB_CC_LED_HPOINT,
		ledc_get_hpoint(RGB_CC_LED_SPEED_MODE, RGB_CC_LED_B_CHANNEL)
	);
}