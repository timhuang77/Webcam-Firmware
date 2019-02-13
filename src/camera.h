/*
 * camera.h
 *
 * Created: 2/22/2018 12:30:50 PM
 *  Author: ece-lab3
 */ 


#ifndef CAMERA_H_
#define CAMERA_H_

#include <asf.h>
#include "ov2640.h"
#include "wifi.h"

/* Constants */
#define MAX_IMAGE_SIZE					100000

/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, \
PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	\
PIO_PERIPH_A, PIO_DEFAULT}

///* TWI board defines. */
#define ID_BOARD_TWI					ID_TWI0
#define BOARD_TWI						TWI0
#define BOARD_TWI_IRQn					TWI0_IRQn
//#define ID_BOARD_TWI					(19)
//#define BOARD_TWI						((Twi    *)0x40018000U)
//#define BOARD_TWI_IRQn					19

///* Pointer to the image data destination buffer */
//uint8_t *g_p_uc_cap_dest_buf;
//
///* Rows size of capturing picture */
//uint16_t g_us_cap_rows = IMAGE_HEIGHT;

/* Declaring an Array-Buffer for storing image data */
volatile uint8_t image_buffer[MAX_IMAGE_SIZE];
volatile uint32_t image_buff_addr;
volatile uint32_t startindex;

/* Actual image size */
volatile uint32_t image_size;
/**
 * \brief Input parameters when initializing the TWI module mode.
 */
//typedef struct twi_options {
	////! MCK for TWI.
	//uint32_t master_clk;
	////! The baud rate of the TWI bus.
	//uint32_t speed;
	////! The desired address.
	//uint8_t chip;
	////! SMBUS mode (set 1 to use SMBUS quick command, otherwise don't).
	//uint8_t smbus;
//} twi_options_t;


/* Helper Functions */
//Handlers
void vsync_handler(uint32_t ul_id, uint32_t ul_mask);
void init_vsync_interrupts(void);

//Configurators
void configure_twi(void);
void pio_capture_init(Pio *p_pio, uint32_t ul_id);
void init_camera(void);
void configure_camera(void);

//Processors
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size);
void start_capture(void);
uint8_t find_image_len(void);





#endif /* CAMERA_H_ */

