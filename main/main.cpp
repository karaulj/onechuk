#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "hal/gpio_types.h"

//#include "tinypico.h"
#include "rgb_cc_led.h"


extern "C" void app_main(void);

void app_main(void)
{
    RGB_CC_LED testLED = RGB_CC_LED(25, 26, 27);
    
    while (true) {
        printf("R getting brighter\n");
        testLED.setColor(255, 0, 0, 3000, 3000, 3000);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        printf("G getting brighter\n");
        testLED.setColor(255, 255, 0, 3000, 3000, 3000);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
        printf("B getting brighter\n");
        testLED.setColor(255, 255, 255, 3000, 3000, 3000);
        vTaskDelay(3000 / portTICK_PERIOD_MS);

        printf("All getting dimmer\n");
        testLED.setColor(0, 0, 0, 5000, 5000, 5000);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}