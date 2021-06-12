/*
 * test_NunchukController.cpp
 *
 *  Created on: Jun 11, 2021
 *      Author: Jacob
 */


#include "unity.h"
#include "NunchukController.h"

#include "driver/i2c.h"
#include "hal/i2c_types.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TEST_WAIT_TIME			3000 / portTICK_PERIOD_MS
#define TEST_I2C_FREQ_HZ		100000
#define TEST_I2C_PORT_NUM		0
#define TEST_SDA_PIN			21
#define TEST_SCL_PIN			22

#define JOYSTICK_THRESHOLD		0.85
#define ACCEL_THRESHOLD			0.75
#define SWITCH_DEBOUNCE_TIME_MS	20

static void i2c_init()
{
    i2c_config_t i2cConfig;
	i2cConfig.mode = I2C_MODE_MASTER;
	i2cConfig.scl_io_num = TEST_SCL_PIN;
	i2cConfig.sda_io_num = TEST_SDA_PIN;
	i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.master.clk_speed = TEST_I2C_FREQ_HZ;
	ESP_ERROR_CHECK(i2c_param_config(TEST_I2C_PORT_NUM, &i2cConfig));
	ESP_ERROR_CHECK(i2c_driver_install(TEST_I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0));
}

static void i2c_shutdown()
{
	i2c_driver_delete(TEST_I2C_PORT_NUM);
}


TEST_CASE("joystickX high within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_JOYSTICK_MAX*JOYSTICK_THRESHOLD);
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getJoystickX() >= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("joystickX low within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_JOYSTICK_MAX*(1-JOYSTICK_THRESHOLD));
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getJoystickX() <= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}

TEST_CASE("joystickY high within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_JOYSTICK_MAX*JOYSTICK_THRESHOLD);
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getJoystickY() >= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("joystickY low within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_JOYSTICK_MAX*(1-JOYSTICK_THRESHOLD));
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getJoystickY() <= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}

TEST_CASE("accelX high within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*ACCEL_THRESHOLD);
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelX() >= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("accelX low within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*(1-ACCEL_THRESHOLD));
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelX() <= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}

TEST_CASE("accelY high within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*ACCEL_THRESHOLD);
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelY() >= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("accelY low within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*(1-ACCEL_THRESHOLD));
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelY() <= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}

TEST_CASE("accelZ high within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*ACCEL_THRESHOLD);
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelZ() >= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("accelZ low within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	uint32_t threshold = (uint32_t)((float)NUNCHUK_CONTROLLER_ACCEL_MAX*(1-ACCEL_THRESHOLD));
	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getAccelZ() <= threshold)
		{
			triggerDetected = 1;
			break;
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}

TEST_CASE("cButton pressed within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getCButton() == 1)
		{
			vTaskDelay(SWITCH_DEBOUNCE_TIME_MS / portTICK_PERIOD_MS);
			if (controller.getCButton() == 1) {
				triggerDetected = 1;
				break;
			}
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
TEST_CASE("zButton pressed within 3 seconds", "[NUNCHUK_CONTROLLER][i2c]")
{
	i2c_init();
	NunchukController controller = NunchukController(TEST_I2C_PORT_NUM);

	uint8_t triggerDetected = 0;
	uint32_t start = xTaskGetTickCount();

	while (start + TEST_WAIT_TIME >= xTaskGetTickCount())
	{
		controller.fetchLatestReadings();
		if (controller.getZButton() == 1)
		{
			vTaskDelay(SWITCH_DEBOUNCE_TIME_MS / portTICK_PERIOD_MS);
			if (controller.getZButton() == 1) {
				triggerDetected = 1;
				break;
			}
		}
		vTaskDelay(1/portTICK_PERIOD_MS);
	}

	i2c_shutdown();
	TEST_ASSERT_EQUAL(1, triggerDetected);
}
