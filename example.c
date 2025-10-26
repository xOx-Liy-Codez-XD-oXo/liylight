#include <stdio.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <math.h>
#include <ogc/lwp_watchdog.h> //For time

#include "liylight.h"

int main(int argc, char **argv) {
	WPAD_Init();
	PAD_Init();

	liylight_init(); //Start liylight

	CON_EnableGecko(1, 0);
	printf("Liylight demo... START or HOME to exit\n");

	u64 starttime = gettime();

	while(1) {
		WPAD_ScanPads();
		PAD_ScanPads();
		u32 pressed = WPAD_ButtonsDown(0);
		u32 gpressed = PAD_ButtonsDown(0);
		if ( pressed & WPAD_BUTTON_HOME ) break;
		if ( gpressed & PAD_BUTTON_START ) break;

		double time = (double)ticks_to_microsecs(gettime() - starttime) / 500000.0f;

		//Pass an int between 0 and 4095 to this function to directly set the light to that value
		//It will not be gamma corrected
		//liylight_setDutyDirect(2000);

		//Pass a float between 0.0 and 1.0 to this function to set the light to that value
		//It will be gamma corrected
		liylight_setDutyFloat(0.5f + (0.5f * sinf(time)));

		//You may set the light to whatever brightness as often as you like
		usleep(10000); //Update brightness and check keys every 10 ms
		//You may do whatever else you like with liylight running in the background
	}
	
	liylight_stop(); //Stopping liylight will turn off the light
	return 0;
}
