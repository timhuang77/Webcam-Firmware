/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	ioport_init();

	//ioport_set_pin_dir(LED_PIN, IOPORT_DIR_OUTPUT);
		////Set IOPORT to be an output for LED
	//ioport_set_pin_level(LED_PIN, false);

	/* Configure WiFi Reset Pin */	
	ioport_set_pin_dir(WIFI_RST_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_RST_PIN, true);
		//WiFi RST pin pulled HIGH initially. 
		//Set LOW for System reset
		
	/* Configure  Reset Pin */
	ioport_set_pin_dir(WIFI_INT_CONNECT_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(WIFI_INT_CONNECT_PIN, IOPORT_MODE_PULLUP);
		//Configure WiFi Internet Connectivity Pin as input
	
	ioport_set_pin_dir(OV_RST_GPIO, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(OV_RST_GPIO, true);
		//OV2640 Camera Reset pin is set to initially pull HIGH
		//Pulling LOW performs a hardware reset
	
	
	/*Configure WiFi CTS Pin */
	ioport_set_pin_dir(WIFI_CTS_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(WIFI_CTS_PIN, false);
		// Pull CTS pin of WiFi low to enable communication

}
