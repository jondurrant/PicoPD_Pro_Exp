/**
 * Jon Durrant.
 *
 * Blink LED on Raspberry PICO
 */

#include "pico/stdlib.h"
#include <stdio.h>

#define DELAY 500 // in microseconds

int main() {
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;

	stdio_init_all();
	sleep_ms(1000);

	printf("Go\n");

    gpio_init(LED_PIN);

    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (true) { // Loop forever
        gpio_put(LED_PIN, 1);
        printf("On\n");
        sleep_ms(DELAY);
        gpio_put(LED_PIN, 0);
        printf("Off\n");
        sleep_ms(DELAY);
    }

}
