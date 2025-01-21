/**
 * Jon Durrant.
 *
 * Demonstrate the AP33772S
 */

#include "pico/stdlib.h"
#include <stdio.h>
#include "AP33772S.h"
extern "C"{
#include "ssd1306.h"
}


int main() {


	stdio_init_all();
	sleep_ms(1000);

	printf("Go\n");

	//Set up I2C0 and AP33772S
    i2c_init(i2c0, 100 * 1000);
    gpio_set_function(0, GPIO_FUNC_I2C);
    gpio_set_function(1, GPIO_FUNC_I2C);
    gpio_pull_up(0);
    gpio_pull_up(1);
    AP33772S usbpd;

    //Start Power Management and display profiles
    usbpd.begin();
    usbpd.displayProfiles();
    usbpd.setOutput(1);

    //Setup I2C1 and the SSD1306 Oled Screen
    i2c_init(i2c1, 100 * 1000);
        gpio_set_function(18, GPIO_FUNC_I2C);
        gpio_set_function(19, GPIO_FUNC_I2C);
        gpio_pull_up(18);
        gpio_pull_up(19);
    ssd1306_t oled;
    ssd1306_init(
    		&oled,
			128,
			64,
			0x3c,
			i2c1);
    ssd1306_clear(&oled);

    //Display a Summary of profile on top of Oled
    char line[80];
    int pps = (usbpd.getPPSIndex() > 1);
    int avs = (usbpd.getAVSIndex() > 1);
    sprintf(line, "PDO %d PPS %d AVS %d",
    		usbpd.getNumPDO(),
			pps,
			avs);
    ssd1306_draw_string(
       		&oled,
   			1,
   			1,
   			1,
   			line
   			);
    ssd1306_show(&oled);


    //Forever loop
    for (;;){

    	//Cycle through the Fixed PDO Profiles
		for (int i=1; i <= usbpd.getNumPDO(); i++){
			usbpd.setFixPDO(i, 3000);
			sleep_ms(500);
			ssd1306_clear_square(
					&oled,
					1,
					9,
					128,
					64-9
					);
			sprintf(line, "PDO %d", i);
			ssd1306_draw_string(
						&oled,
						1,
						14,
						2,
						line
						);

			 float f = usbpd.readVoltage();
			 f = f / 1000;
			 sprintf(line, "=%.1fV",f);
			 ssd1306_draw_string(
						&oled,
						1,
						32,
						2,
						line
						);
			 ssd1306_show(&oled);
			 sleep_ms(3000);
		}

		//Cycle through PPS voltages from 3.6V to 20V
		if (usbpd.getPPSIndex() > 0){
			for(int i = 3600; i <= 20000; i=i+200){
				int index = usbpd.getPPSIndex();
				//index = 6;
				printf("Request(%d) %d\n", index,  i);
				usbpd.setPPSPDO(index, i, 4000);

				sleep_ms(500);
				ssd1306_clear_square(
						&oled,
						1,
						9,
						128,
						64-9
						);
				float f = i / 1000.0;
				sprintf(line, "PPS %.1fV", f);
				ssd1306_draw_string(
							&oled,
							1,
							14,
							2,
							line
							);

				 f = usbpd.readVoltage();
				 f = f / 1000;
				 sprintf(line, "=%.1fV",f);
				 ssd1306_draw_string(
							&oled,
							1,
							32,
							2,
							line
							);
				 ssd1306_show(&oled);

				 printf("AP Temp %d\n", usbpd.readTemp());
				 printf("%dmV %dmA\n", usbpd.readVoltage(), usbpd.readCurrent());
				 sleep_ms(3000);
			}
		} //end if

    } // For ever


}
