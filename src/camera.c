/*
 * camera.c
 *
 * Created: 2/22/2018 12:30:12 PM
 *  Author: ece-lab3
 */ 
#include <asf.h>
#include "camera.h"
#include "ov2640.h"
#include "conf_board.h"
//#include "twi.h"

/* Push button information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_push_button_trigger = false;

/* Vsync signal information (true if it's triggered and false otherwise) */
static volatile uint32_t g_ul_vsync_flag = false;

/**
 * \brief Handler to set a flag indicating rising-edge of VSYNC
 */
void vsync_handler(uint32_t ul_id, uint32_t ul_mask)
{
	unused(ul_id);
	unused(ul_mask);

	g_ul_vsync_flag = true;
}

/**
 * \brief Initialize Vsync_Handler.
 */
void init_vsync_interrupts(void)
{
	/* Initialize PIO interrupt handler, see PIO definition in conf_board.h
	**/
	pio_handler_set(OV_VSYNC_PIO, OV_VSYNC_ID, OV_VSYNC_MASK,
			OV_VSYNC_TYPE, vsync_handler);

	/* Enable PIO controller IRQs */
	NVIC_EnableIRQ((IRQn_Type)OV_VSYNC_ID);
}

/**
* \configuration of two wire interface (i2c)
*/
void configure_twi(void)
{

	///* Enable TWI peripheral */
	//pmc_enable_periph_clk(ID_BOARD_TWI);
//
	///* Init TWI peripheral */
	//opt.master_clk = sysclk_get_cpu_hz();
	//opt.speed      = TWI_CLK;
	//twi_master_init(BOARD_TWI, &opt);
//
	///* Configure TWI interrupts */
	//NVIC_DisableIRQ(BOARD_TWI_IRQn);
	//NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	//NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	//NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

/**
 * \brief Initialize PIO capture for the OV7740 image sensor communication.
 *
 * \param p_pio PIO instance to be configured in PIO capture mode.
 * \param ul_id Corresponding PIO ID.
 */
void pio_capture_init(Pio *p_pio, uint32_t ul_id)
{
	/* Enable periphral clock */
	pmc_enable_periph_clk(ul_id);

	/* Disable pio capture */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_PCEN);

	/* Disable rxbuff interrupt */
	p_pio->PIO_PCIDR |= PIO_PCIDR_RXBUFF;

	/* 32bit width*/
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_DSIZE_Msk);
	p_pio->PIO_PCMR |= PIO_PCMR_DSIZE_WORD;

	/* Only HSYNC and VSYNC enabled */
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_ALWYS);
	p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_HALFS);
//
	//#if !defined(DEFAULT_MODE_COLORED)
	///* Samples only data with even index */
	//p_pio->PIO_PCMR |= PIO_PCMR_HALFS;
	//p_pio->PIO_PCMR &= ~((uint32_t)PIO_PCMR_FRSTS);
	//#endif
}


/**
 * \brief Initialize PIO capture and the OV7740 image sensor.
 */
static void capture_init(void)
{
	twi_options_t opt;

	/* Init Vsync handler*/
	init_vsync_interrupts();

	/* Init PIO capture*/
	pio_capture_init(OV2640_DATA_BUS_PIO, OV2640_DATA_BUS_ID);
//
	///* Turn on ov7740 image sensor using power pin */
	//ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK1 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}

	/* OV2640 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}

	/* ov7740 configuration */
	//ov_configure(BOARD_TWI, QVGA_YUV422_20FPS);

	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}

/**
 * \brief Capture OV2640 data to a buffer.
 *
 * \param p_pio PIO instance which will capture data from OV2640 image sensor.
 * \param p_uc_buf Buffer address where captured data must be stored.
 * \param ul_size Data frame size.
 */
uint8_t pio_capture_to_buffer(Pio *p_pio, uint8_t *uc_buf, uint32_t ul_size)
{
	/* Check if the first PDC bank is free */
	if ((p_pio->PIO_RCR == 0) && (p_pio->PIO_RNCR == 0)) {
		p_pio->PIO_RPR = (uint32_t)uc_buf;
		p_pio->PIO_RCR = ul_size;
		p_pio->PIO_PTCR = PIO_PTCR_RXTEN;
		return 1;
	} else if (p_pio->PIO_RNCR == 0) {
		p_pio->PIO_RNPR = (uint32_t)uc_buf;
		p_pio->PIO_RNCR = ul_size;
		return 1;
	} else {
		return 0;
	}
}


/**
 * \brief Start picture capture.
 */
void start_capture(void)
{
	/* Enable vsync interrupt*/
	pio_enable_interrupt(OV_VSYNC_PIO, OV_VSYNC_MASK);

	/* Capture acquisition will start on rising edge of Vsync signal.
	 * So wait g_vsync_flag = 1 before start process
	 */
	while (!g_ul_vsync_flag) {
	}

	/* Disable vsync interrupt*/
	pio_disable_interrupt(OV_VSYNC_PIO, OV_VSYNC_MASK);

	/* Enable pio capture*/
	pio_capture_enable(OV2640_DATA_BUS_PIO);

	/* Capture data to buffer */
	pio_capture_to_buffer(OV2640_DATA_BUS_PIO, image_buffer, MAX_IMAGE_SIZE >> 2);
	

	/* Wait end of capture*/
	while (!((OV2640_DATA_BUS_PIO->PIO_PCISR & PIO_PCIMR_RXBUFF) ==
			PIO_PCIMR_RXBUFF)) {
	}

	/* Disable pio capture*/
	pio_capture_disable(OV2640_DATA_BUS_PIO);

	/* Reset vsync flag*/
	g_ul_vsync_flag = false;
	
	//uint32_t image_size_debug = find_image_len();
	if(find_image_len() == 1){
		image_capture_success = true;
	}
}

/**
* \Finds image length based on JPEG protocol.
*
* \retval = 1 on success
* \retval = 0 on error
* \(i.e. able to find "end of image" & "start of image" markers)
*/
uint8_t find_image_len(void)
{
	image_size = 0;
	int SOI = 0xD8;
	int EOI = 0xD9;
	startindex = 0;
	int endindex = 0;
	bool soi_found = false;
	bool eoi_found = false;
	/* Search through buffer for SOI and EOI JPEG markers */
	for (int ii = 0; ii < MAX_IMAGE_SIZE; ii++){
		if(image_buffer[ii] == 0xFF){
			if(image_buffer[ii+1] == SOI && !soi_found){
				startindex = ii;
				soi_found = true;
				} 
			if(image_buffer[ii+1] == EOI && soi_found && !eoi_found){
				endindex = ii+1;
				eoi_found = true;
			}
		}

	}

	image_size = endindex - startindex + 1;
	image_buff_addr = &image_buffer;
	//uint32_t endindex_addr = endindex + &image_buffer;
	if(image_size != 0){
		return 1;
	} else {
		return 0;
	}
	//If image_size == 0, then there must not have been SOI, EOI
	//Therefore there must be some error.
}


/*
* \Configuration of camera pins, camera clock (XCLK), and 
* calling configure_twi function
*/
void init_camera(void)
{
	twi_options_t opt;
	
	/*Configure MCU GPIO Pins for Camera Databuses */
	gpio_configure_pin(OV_DATA_BUS_D0, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D1, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D2, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D3, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D4, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D5, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D6, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_DATA_BUS_D7, OV_DATA_BUS_FLAGS);
	
	//gpio_configure_pin(OV_PCLK_GPIO, OV_DATA_BUS_FLAGS);
	gpio_configure_pin(OV_XCLK_GPIO, OV_XCLK_FLAGS);
	gpio_configure_pin(OV_HSYNC_GPIO, OV_HSYNC_FLAGS);
	gpio_configure_pin(OV_VSYNC_GPIO, OV_VSYNC_FLAGS);
	gpio_configure_pin(OV_RST_GPIO, OV_RST_FLAGS);

	pio_set_peripheral(PIOA, PIO_PERIPH_B, PIO_PA17);

	/* OV2460 send image sensor data at 24 Mhz. For best performances, PCK0
	 * which will capture OV7740 data, has to work at 24Mhz. It's easier and
	 * optimum to use one PLL for core (PLLB) and one other for PCK0 (PLLA).
	 */
	/* brief Enable PLLB clock. */
	pmc_enable_pllbck(7, 0x1, 1); /* PLLA work at 96 Mhz */

	///* OV7740 image sensor initialization*/
	//capture_init();
	
	//twi_options_t opt;

	/* Init Vsync handler*/
	init_vsync_interrupts();
	
	gpio_set_pin_high(OV_RST_GPIO);
	
	/* Init PIO capture*/
	pio_capture_init(OV2640_DATA_BUS_PIO, OV2640_DATA_BUS_ID);

	///* Turn on ov7740 image sensor using power pin */
	//ov_power(true, OV_POWER_PIO, OV_POWER_MASK);

	/* Init PCK0 to work at 24 Mhz */
	/* 96/4=24 Mhz */
	PMC->PMC_PCK[1] = (PMC_PCK_PRES_CLK_4 | PMC_PCK_CSS_PLLB_CLK);
	PMC->PMC_SCER = PMC_SCER_PCK1;
	while (!(PMC->PMC_SCSR & PMC_SCSR_PCK1)) {
	}
	
	//configure_twi();
			
	/* Configure TWI pins */
	gpio_configure_pin(TWI0_DATA_GPIO, TWI0_DATA_FLAGS);
	gpio_configure_pin(TWI0_CLK_GPIO, TWI0_CLK_FLAGS);
	
	/* Enable TWI peripheral */
	pmc_enable_periph_clk(ID_BOARD_TWI);

	/* Init TWI peripheral */
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	twi_master_init(BOARD_TWI, &opt);

	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
}

/*
* \Configuration of OV2640 registers for desired operation.
*/
void configure_camera(void)
{
	/* OV2640 Initialization */
	while (ov_init(BOARD_TWI) == 1) {
	}
	
	ov_configure(BOARD_TWI,JPEG_INIT);
	ov_configure(BOARD_TWI,YUV422);
	ov_configure(BOARD_TWI,JPEG);
	ov_configure(BOARD_TWI,JPEG_320x240);	
	//ov_configure(BOARD_TWI,JPEG_640x480);
	
	/* Wait 3 seconds to let the image sensor to adapt to environment */
	delay_ms(3000);
}