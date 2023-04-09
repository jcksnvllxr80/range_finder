#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "ultrasonic/ultrasonic.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

#define SLEEPTIME 25

void setup_gpios(void);
void display_text(ssd1306_t disp, char *text);

int disp_start_x = 8;
int disp_start_y = 8;
int text_scale = 2;

int main() {

    const uint led_pin = 25;
    uint trigPin = 2;
    uint echoPin = 3;
    int dist;
    char dist_str[7];

    // Initialize LED pin
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);

    stdio_init_all();

    setupUltrasonicPins(trigPin, echoPin);
    setup_gpios();
    
    ssd1306_t disp;
    disp.external_vcc=false;
    ssd1306_init(&disp, 128, 32, 0x3C, i2c_default);

    // Loop forever
    while (true) {
        // turn on led
        gpio_put(led_pin, true);

        // get measurement, log, and display
        dist = getCm(trigPin, echoPin);
        itoa(dist, dist_str, 10);
        strcat(dist_str, " cm");
        printf("%s\r\n", dist_str);
        display_text(disp, dist_str);
        // sleep_ms(30);

        // turn off led
        gpio_put(led_pin, false);

        // give some time to rest
        sleep_ms(100);
    }
}

void setup_gpios(void) {
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
}

void display_text(ssd1306_t disp, char *text) {
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, disp_start_x, disp_start_y, text_scale, text);
    ssd1306_show(&disp);
}
