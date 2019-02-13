/**
Created by Timothy Huang & Jason Fishman
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "wifi.h"
#include "camera.h"
//#include "timer_interface.h"
//#include "ov2640.h"


int main (void)
{
/*Initialization*/
	/* Initialize clock and board definitions */
		/* Insert system clock initialization code here (sysclk_init()). */

	//Copied from USART Hard Handshaking
	uint8_t uc_char;
	uint8_t uc_flag;

	/* Initialize the system. */
	sysclk_init();
	wdt_disable(WDT);
	board_init();

	/* Configure and start Timer. (Look in "timer_interface" functions.) */
	configure_tc();
	
	/* Configure the WiFi USART, as well as Command and Web Setup pin. */
	configure_usart_wifi();
	configure_wifi_comm_pin();
	configure_wifi_web_setup_pin();

	/* Reset WiFi */
	ioport_set_pin_level(WIFI_RST_PIN, false);	//Pull LOW to reset WiFi
	delay_ms(1000);
	ioport_set_pin_level(WIFI_RST_PIN, true);
	
	/* Wait for it to connect to a network. While waiting, make sure to listen for Web Setup pin. */
	bool internet_connected = ioport_get_pin_level(WIFI_INT_CONNECT_PIN);
	
	/* Initialize and configure the camera */
	init_camera();
	configure_camera();

	/* Tell WiFi to turn off the command prompt and command echo */
		/* Turn off the command prompt with the command "set sy c p off"*/
		
	write_wifi_command("set sy c p off\r\n",1000);
	
		/* Turn off the command echo with the command "set sy c e off" */
	write_wifi_command("set sy c e off\r\n",1000);
	
	delay_ms(12000);
	
	internet_connected = ioport_get_pin_level(WIFI_INT_CONNECT_PIN);
	
	/* Check for Web Setup request. */
	/*If network is available, query available websocket connections. 
		If it is not, reset the module. */
	if(!internet_connected){
		//put the WiFi chip into web setup mode
		write_wifi_command("setup web\r\n",50);
		initiate_web_setup_flag = false; //Web setup is done
	}
/* Loop */
	while(true){
		if(!initiate_web_setup_flag){ // Check if Web Setup flag cleared
			/* Listen to to see if chip is connected to internet */
			internet_connected = ioport_get_pin_level(WIFI_INT_CONNECT_PIN);

			/*If network is available, query available websocket connections. 
			If it is not, reset the module. */
			if(internet_connected){
				write_wifi_command("poll all\r\n",50);	//Poll streams
				//no_open_streams = false;
				if(!no_open_streams){	//If streams available, take picture
					//Select available stream
					start_capture();
					if(image_capture_success){ /* If picture taken successfully, transfer it to the AMW136. */
						//write_wifi_command("image_transfer\r\n",50);
						//write_wifi_command("start_transfer\r\n",50);
						write_image_to_file();
					}
				} else {			// If no connections available, delay 1s and start over.
					delay_ms(1000);	//Wait 1 second, then restart loop
					no_open_streams = true;
				}
			} else {
				ioport_set_pin_level(WIFI_RST_PIN, false);	//Pull LOW to reset WiFi
			}
			
		} else{
			//put the wifi chip into web setup mode
			write_wifi_command("setup web\r\n",50);
			initiate_web_setup_flag = false; //Web setup is done
		}
		// Need to WAIT FOR CONNECTION ???
		
		
	}
}
