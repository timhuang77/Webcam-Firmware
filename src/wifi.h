/*
 * wifi.h
 *
 * Created: 2/22/2018 12:29:53 PM
 *  Author: ece-lab3
 */ 
#ifndef WIFI_H_
#define WIFI_H_

#include <asf.h>
#include <string.h>
//#include <stdio.h>
//#include "ov2640.h"
#include "timer_interface.h"
#include "camera.h"


#define MAX_INPUT_WIFI				1000

/* Websocket flags */
volatile bool no_open_streams;
volatile bool stream0_open;
volatile bool stream1_open;
volatile bool stream2_open;
volatile bool stream3_open;
volatile bool stream4_open;
volatile bool stream5_open;
volatile bool stream6_open;
volatile bool stream7_open;

/* Image Transfer flags */
volatile bool image_capture_success;
volatile bool image_transf_ready;
volatile bool image_transf_complete;

volatile uint32_t image_size;
volatile bool wifi_cmd_complete;
volatile uint32_t initiate_web_setup_flag;
volatile uint32_t received_byte_wifi;
volatile char input_line_wifi[MAX_INPUT_WIFI];	//MAX_INPUT_WIFI: Maximum number of characters to expect in a transmission
volatile bool new_rx_wifi;
volatile uint32_t input_pos_wifi;







/* WiFi USART Defines */
#define WIFI_USART					USART0
#define WIFI_USART_ID				ID_USART0
#define WIFI_USART_BAUDRATE			115200
#define WIFI_USART_HANDLER			USART0_Handler
#define WIFI_USART_IRQn				USART0_IRQn
#define WIFI_USART_CHAR_LENGTH		US_MR_CHRL_8_BIT
#define WIFI_USART_PARITY			US_MR_PAR_NO
#define WIFI_USART_STOP_BITS		US_MR_NBSTOP_1_BIT
#define WIFI_USART_MODE				US_MR_CHMODE_NORMAL

/** All interrupt mask. */
#define ALL_INTERRUPT_MASK			0xffffffff

/* USART Pins */
/** USART0 pin RX */
#define PIN_USART0_RXD	  {PIO_PA5A_RXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RXD_IDX        (PIO_PA5_IDX)
#define PIN_USART0_RXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin TX */
#define PIN_USART0_TXD    {PIO_PA6A_TXD0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_TXD_IDX        (PIO_PA6_IDX)
#define PIN_USART0_TXD_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin CTS */
#define PIN_USART0_CTS    {PIO_PA8A_CTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_CTS_IDX        (PIO_PA8_IDX)
#define PIN_USART0_CTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)
/** USART0 pin RTS */
#define PIN_USART0_RTS    {PIO_PA7A_RTS0, PIOA, ID_PIOA, PIO_PERIPH_A, PIO_PULLUP}
#define PIN_USART0_RTS_IDX        (PIO_PA7_IDX)
#define PIN_USART0_RTS_FLAGS      (PIO_PERIPH_A | PIO_PULLUP)


/* WiFi Command Port Defines */
/* Push button board defines. */
#define WIFI_COMM_PIN_NUM				PIO_PA12
#define WIFI_COMM_ID					ID_PIOA
#define WIFI_COMM_PIO					PIOA
#define WIFI_COMM_TYPE					PIO_INPUT
#define WIFI_COMM_MASK					PIO_PA12_IDX
#define WIFI_COMM_ATTR					PIO_IT_RISE_EDGE
 //* \param p_pio PIO controller base address.
 //* \param ul_id PIO ID.
 //* \param ul_mask Pins (bit mask) to configure.
 //* \param ul_attr Pins attribute to configure.
 //* \param p_handler Interrupt handler function pointer.


/* Web Setup Button Defines */
//Used to be configure_button defines
#define WEB_SETUP_BUTTON_PIN_MSK            PIO_PA18
#define WEB_SETUP_BUTTON_ID                 ID_PIOA
#define WEB_SETUP_BUTTON_PIO                PIOA
#define WEB_SETUP_BUTTON_TYPE				PIO_INPUT
#define WEB_SETUP_BUTTON_ATTR               PIO_IT_RISE_EDGE
//PIO_PULLUP | PIO_DEBOUNCE | PIO_IT_RISE_EDGE
//#define USART_IRQn					USART0_IRQn


	/* Helper Functions */
//Handlers
void wifi_command_response_handler(uint32_t ul_id, uint32_t ul_mask);
void wifi_web_setup_handler(uint32_t ul_id, uint32_t ul_mask);
//void WIFI_USART_HANDLER(void);

//Configurators
void configure_usart_wifi(void);
void configure_wifi_comm_pin(void);
void configure_wifi_web_setup_pin(void);
void reset_wifi(void);

//Processors
void process_incoming_byte_wifi(uint8_t in_byte);
void process_data_wifi();
void write_wifi_command(char* comm, uint8_t cnt);
void write_image_to_file(void);
#endif /* WIFI_H_ */