/**
 * \file
 *
 * \brief User board configuration template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#ifndef CONF_BOARD_H
#define CONF_BOARD_H

//#include <asf.h>

//


/******************************* TWI definition
 *********************************/
/* TWI clock frequency in Hz (400KHz) */
#define TWI_CLK     (400000UL)

/** TWI0 data pin */
#define PIN_TWI_TWD0                   {PIO_PA3A_TWD0, PIOA, ID_PIOA, \
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 clock pin */
#define PIN_TWI_TWCK0                  {PIO_PA4A_TWCK0, PIOA, ID_PIOA,	\
                                         PIO_PERIPH_A, PIO_DEFAULT}

/** TWI0 Data pins definition */
#define TWI0_DATA_GPIO                 PIO_PA3_IDX
#define TWI0_DATA_FLAGS                (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_DATA_MASK                 PIO_PA3
#define TWI0_DATA_PIO                  PIOA
#define TWI0_DATA_ID                   ID_PIOA
#define TWI0_DATA_TYPE                 PIO_PERIPH_A
#define TWI0_DATA_ATTR                 PIO_DEFAULT

/** TWI0 clock pins definition */
#define TWI0_CLK_GPIO                  PIO_PA4_IDX
#define TWI0_CLK_FLAGS                 (PIO_PERIPH_A | PIO_DEFAULT)
#define TWI0_CLK_MASK                  PIO_PA4
#define TWI0_CLK_PIO                   PIOA
#define TWI0_CLK_ID                    ID_PIOA
#define TWI0_CLK_TYPE                  PIO_PERIPH_A
#define TWI0_CLK_ATTR                  PIO_DEFAULT

/** TWI0 pins */
#define PINS_TWI0                      PIN_TWI_TWD0, PIN_TWI_TWCK0

#define ID_BOARD_TWI		               ID_TWI0
#define BOARD_TWI			                 TWI0
#define BOARD_TWI_IRQn		             TWI0_IRQn
			/*** Camera Related ***/


//
///** OV Data Bus pins */
//#define OV_SW_OVT_MASK					PIO_PC10
//#define OV_SW_OVT_PIO					PIOC
//
///*Image sensor board defines */
//// Image sensor Power pin.
//#define OV_POWER_PIO					OV_SW_OVT_PIO
//#define OV_POWER_MASK					OV_SW_OVT_MASK

// Image sensor VSYNC pin.
//#define OV2640_VSYNC_PIO				PIOA
//#define OV2640_VSYNC_ID					ID_PIOA
//#define OV2640_VSYNC_MASK				PIO_PA15
//#define OV2640_VSYNC_TYPE				PIO_PULLUP
// Image sensor data pin.
#define OV2640_DATA_BUS_PIO				PIOA
#define OV2640_DATA_BUS_ID				ID_PIOA

/* WiFi PIO's */
#define WIFI_SETUP_PIN					PIO_PA18_IDX
#define WIFI_RST_PIN					PIO_PA22_IDX
#define WIFI_CTS_PIN					PIO_PA7_IDX
#define WIFI_INT_CONNECT_PIN			PIO_PA11_IDX


/* Camera Databuses */
#define OV_DATA_BUS_D0					PIO_PA24_IDX
#define OV_DATA_BUS_D1					PIO_PA25_IDX
#define OV_DATA_BUS_D2					PIO_PA26_IDX
#define OV_DATA_BUS_D3					PIO_PA27_IDX
#define OV_DATA_BUS_D4					PIO_PA28_IDX
#define OV_DATA_BUS_D5					PIO_PA29_IDX
#define OV_DATA_BUS_D6					PIO_PA30_IDX
#define OV_DATA_BUS_D7					PIO_PA31_IDX
#define OV_DATA_BUS_FLAGS              (PIO_INPUT | PIO_PULLUP)
#define OV_DATA_BUS_MASK               (0xFF000000UL)
#define OV_DATA_BUS_PIO                PIOA
#define OV_DATA_BUS_ID                 ID_PIOA
#define OV_DATA_BUS_TYPE               PIO_INPUT
#define OV_DATA_BUS_ATTR               PIO_DEFAULT

/** OV_RST pin definition */
#define OV_RST_GPIO                    PIO_PA20_IDX
#define OV_RST_FLAGS                   (PIO_OUTPUT_1 | PIO_DEFAULT)
#define OV_RST_MASK                    PIO_PC20
#define OV_RST_PIO                     PIOC
#define OV_RST_ID                      ID_PIOC
#define OV_RST_TYPE                    PIO_OUTPUT_1


/** OV_XCLK pin definition */
#define OV_XCLK_GPIO					(PIO_PA17_IDX)
#define OV_XCLK_FLAGS					(PIO_PERIPH_B | PIO_DEFAULT)
#define OV_XCLK_MASK					PIO_PA17
#define OV_XCLK_PIO						PIOA
#define OV_XCLK_ID						ID_PIOA
#define OV_XCLK_TYPE					PIO_PERIPH_B
#define OV_XCLK_ATTR					PIO_DEFAULT

/** OV_HSYNC pin definition */
#define OV_HSYNC_GPIO                  PIO_PA16_IDX
#define OV_HSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_HSYNC_MASK                  PIO_PA16
#define OV_HSYNC_PIO                   PIOA
#define OV_HSYNC_ID                    ID_PIOA
#define OV_HSYNC_TYPE                  PIO_PULLUP

/** OV_VSYNC pin definition */
#define OV_VSYNC_GPIO                  PIO_PA15_IDX
#define OV_VSYNC_FLAGS                 (PIO_PULLUP | PIO_IT_RISE_EDGE)
#define OV_VSYNC_MASK                  PIO_PA15
#define OV_VSYNC_PIO                   PIOA
#define OV_VSYNC_ID                    ID_PIOA
#define OV_VSYNC_TYPE                  PIO_PULLUP

/* Camera Peripherals */
//#define OV_PCLK_GPIO					PIO_PA23_IDX
//#define OV_XCLK_GPIO					PIO_PA17_IDX
//#define OV_HSYNC_GPIO					PIO_PA16_IDX
//#define OV_VSYNC_GPIO					PIO_PA15_IDX
//#define OV_RST_GPIO					PIO_PA20_IDX
















#endif // CONF_BOARD_H
