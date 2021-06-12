#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"

#include "hal/gpio_types.h"
#include "hal/i2c_types.h"
#include "driver/i2c.h"

#include "settings.h"
#include "tasks.h"
#include "queues.h"
#include "NunchukController.h"
#include "rgb_cc_led.h"


extern "C" void app_main(void);


static void i2c_init()
{
    i2c_config_t i2cConfig;
	i2cConfig.mode = I2C_MODE_MASTER;
	i2cConfig.scl_io_num = I2C_SCL_PIN;
	i2cConfig.sda_io_num = I2C_SDA_PIN;
	i2cConfig.scl_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.sda_pullup_en = GPIO_PULLUP_ENABLE;
	i2cConfig.master.clk_speed = I2C_FREQ_HZ;
	ESP_ERROR_CHECK(i2c_param_config(I2C_PORT_NUM, &i2cConfig));
	ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT_NUM, I2C_MODE_MASTER, 0, 0, 0));
}


void app_main(void)
{
    // queues
    queuesCheck();

    // I2C init - configure in settings.h
	i2c_init();

    // objects init
    RGB_CC_LED mainLED = RGB_CC_LED(25, 26, 27);
    NunchukController nunchuk = NunchukController(I2C_PORT_NUM);
    
    // tasks
    xTaskCreate(&nunchukReadTask, NUNCHUK_READ_TASK, 2048,
        (void*)&nunchuk, 24, NULL);	                // max priority
    xTaskCreate(&printTask, PRINT_TASK, 4096,
        (void*)&nunchuk, tskIDLE_PRIORITY, NULL);   // lowest priority
    xTaskCreate(&fadeLEDTask, FADE_LED_TASK, 2048,
        (void*)&mainLED, tskIDLE_PRIORITY, NULL);	// lowest priority

    while(true)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}