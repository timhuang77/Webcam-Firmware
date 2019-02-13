/*
 * wifi.c
 *
 * Created: 2/22/2018 12:29:32 PM
 *  Author: Timothy Huang & Jason Fishman
 */ 
#include <asf.h>
#include <stdio.h>
#include "wifi.h"
//#include "ov2640.h"
#include "timer_interface.h"



/* Websocket flags */
//volatile bool no_open_streams;
//volatile bool stream0_open = false;
//volatile bool stream1_open = false;
//volatile bool stream2_open = false;
//volatile bool stream3_open = false;
//volatile bool stream4_open = false;
//volatile bool stream5_open = false;
//volatile bool stream6_open = false;
//volatile bool stream7_open = false;

/* Image Transfer flags */
volatile bool image_transf_complete = false;
volatile bool image_capture_success = false;
volatile bool image_transf_ready = false;

volatile uint32_t received_byte_wifi = 0;
	//declared volatile because variable can change during an Interrupt Service Routine
volatile bool new_rx_wifi = false;
volatile uint32_t input_pos_wifi = 0;

/**
 *  \brief Interrupt handler for USART.
 *
 * Increment the number of bytes received in the current second and start
 * another transfer if the desired bps has not been met yet.
 *
 */
//System level function - already declared
void WIFI_USART_HANDLER(void)
{	
	uint32_t ul_status;
	
	/* Read USART status. */
	ul_status = usart_get_status(WIFI_USART);

	/* Receive buffer is full. */
	if (ul_status & US_CSR_RXBUFF) {
		usart_read(WIFI_USART, &received_byte_wifi);	//Place received byte in "received_byte_wifi"
		new_rx_wifi = true;
		process_incoming_byte_wifi((uint8_t)received_byte_wifi);	//Processed received byte
			//cast received_byte_wifi as uint8_t
	}
}

/**
 * \brief Handler for WiFi command response interrupt
 */
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	wifi_cmd_complete = true;	//Set WiFi command complete flag to true
	process_data_wifi();	//Parse data to see if the command response is something comprehensible.
	for(uint32_t ii = 0; ii < MAX_INPUT_WIFI; ii++){	//Clear WiFi input line (response) buffer
		input_line_wifi[ii] = 0;
	}	
	input_pos_wifi = 0;
}


/**
 *
 * \Handler for button to initiate web setup of AMW136. 
 * \Should set a flag indicating a request to initiate web setup.
 *
 */
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	initiate_web_setup_flag = true;
}

/**
 *  Configuration of USART port used to communicate with the AMW136.
 */
void configure_usart_wifi(void)
{
	/* Configure USART RX, TX, CTS, RTS pins */
	gpio_configure_pin(PIN_USART0_RXD_IDX, PIN_USART0_RXD_FLAGS);
	gpio_configure_pin(PIN_USART0_TXD_IDX, PIN_USART0_TXD_FLAGS);
	gpio_configure_pin(PIN_USART0_CTS_IDX, PIN_USART0_CTS_FLAGS);
	//gpio_configure_pin(PIN_USART0_RTS_IDX, PIN_USART0_RTS_FLAGS);

	static uint32_t ul_sysclk;
	const sam_usart_opt_t usart_console_settings = {
		WIFI_USART_BAUDRATE,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};

	/* Get peripheral clock. */
	ul_sysclk = sysclk_get_peripheral_hz();

	/* Enable peripheral clock. */
	sysclk_enable_peripheral_clock(WIFI_USART_ID);

	/* Configure USART. */
	usart_init_hw_handshaking(WIFI_USART, &usart_console_settings, ul_sysclk);

	/* Disable all the interrupts while USART (TX & RX) is enabled */
	usart_disable_interrupt(WIFI_USART, ALL_INTERRUPT_MASK);
	
	/* Enable TX & RX function. */
	usart_enable_tx(WIFI_USART);
	usart_enable_rx(WIFI_USART);

	/*Enable USART Interrupt. */
	usart_enable_interrupt(WIFI_USART, US_IER_RXRDY);
		//RXRDY - Data ready in the receive line
		//RXBUFF - When you have data in the buffer

	/* Configure and enable interrupt of USART. */
	NVIC_EnableIRQ(WIFI_USART_IRQn);
}

/**
 * \Con?guration of ¡§command complete¡¨ rising-edge interrupt.
 */
void configure_wifi_comm_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WIFI_COMM_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WIFI_COMM_PIO, WIFI_COMM_ID, WIFI_COMM_PIN_NUM, WIFI_COMM_ATTR, wifi_command_response_handler);

	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WIFI_COMM_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WIFI_COMM_PIO, WIFI_COMM_PIN_NUM);
}

/**
 * \Configuration of button interrupt to initiate web setup
 */
void configure_wifi_web_setup_pin(void)
{
	/* Configure PIO clock. */
	pmc_enable_periph_clk(WEB_SETUP_BUTTON_ID);

	/* Adjust PIO debounce filter using a 10 Hz filter. */
	pio_set_debounce_filter(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_PIN_MSK, 10);

	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_ID, WEB_SETUP_BUTTON_PIN_MSK,
			WEB_SETUP_BUTTON_ATTR, wifi_web_setup_handler);
			 
	/* Enable PIO controller IRQs. */
	NVIC_EnableIRQ((IRQn_Type)WEB_SETUP_BUTTON_ID);

	/* Enable PIO interrupt lines. */
	pio_enable_interrupt(WEB_SETUP_BUTTON_PIO, WEB_SETUP_BUTTON_PIN_MSK);
}

void process_incoming_byte_wifi(uint8_t in_byte)
{
	/* Continuously read wifi bytes into buffer until /n */
	input_line_wifi[input_pos_wifi] = in_byte;
	input_pos_wifi++;
}

void process_data_wifi(void){
	if(strstr(input_line_wifi,"Unknown commmand") ) {
		return;
	}
	
	//Parsing buffer to see if no open Websocket streams
	if(strstr(input_line_wifi, "None")){
		no_open_streams = true;
	}
	
	//Parsing for open Websocket streams
	if(strstr(input_line_wifi, "0,0")){
		stream0_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "1,0")){
		stream1_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "2,0")){
		stream2_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "3,0")){
		stream3_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "4,0")){
		stream4_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "5,0")){
		stream5_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "6,0")){
		stream6_open = true;
		no_open_streams = false;
	}
	if(strstr(input_line_wifi, "7,0")){
		stream7_open = true;
		no_open_streams = false;
	}
	//Parsing buffer for image transfer ready
	if(strstr(input_line_wifi, "Start transfer")){
		image_transf_ready = true;
	}	
	//Parsing buffer for image transfer success
	if(strstr(input_line_wifi, "Complete")){
		image_transf_complete = true;
	}
	

}



/*
* \Writes a command(comm) to the AMW136, and waits either for an
 acknowledgment or a timeout. The timeout can be created by 
 setting the global variable counts to zero, which will 
 automatically increment every second, and waiting while 
 counts< cnt. 
*/
 void write_wifi_command(char* comm, uint8_t cnt)
 {
	counts = 0;
	wifi_cmd_complete = false;
		usart_write_line(WIFI_USART, comm);

	while(wifi_cmd_complete == false && counts < cnt){
		continue;
	}

}

void write_image_to_file(void){
	//image_size = find_image_len();
	if(image_size != 0){ //Image valid since image_size != 0
		char str[80];
		
		sprintf(str, "image_transfer %d\r\n", image_size);
		write_wifi_command(str, 50);
		
		if(image_transf_ready){
			for (int i = 0; i < image_size; i++){
				usart_putchar(WIFI_USART, image_buffer[startindex + i]);
					//Writes the image byte-by-byte via USART
			}
		}
		
	} else {	//Image invalid since image_size = 0
		return;
	}

	
}