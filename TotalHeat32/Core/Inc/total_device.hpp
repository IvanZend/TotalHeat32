/*
 * total_display.h
 *
 *  Created on: Feb 9, 2022
 *      Author: TZprog2
 */

#ifndef INC_TOTAL_DEVICE_HPP_
#define INC_TOTAL_DEVICE_HPP_

#include "stm32f4xx_hal.h"
#include "main.h"


#ifdef __cplusplus
	extern "C"
	{
#endif


	#define LCD_FRAME_BUFFER(N) ((uint32_t)(0xC0000000 + (N)*0x1D4C00))
	#define LAYER_PAGE_0		0
	#define LAYER_PAGE_1		1
	#define LAYER_PAGE_2		2
	#define LAYER_PAGE_3		3
	#define LAYER_PAGE_4		4
	#define X_SIZE	 			800
	#define Y_SIZE	 			480
	#define I2C_TRANSMISSION_INTERVAL_MS    200
	#define WITHIN(N,L,H)       ((N) >= (L) && (N) <= (H))
	#define TIME_PERSENT_TO_VACUUM      40
	#define TIME_PERCENT_TO_HEAT        30
	#define TEN_TEMPERATURE_ADDITION_0  160
	#define TEN_TEMPERATURE_ADDITION_1  200
	#define TEN_ADDITION_TRESHOLD       80
	#define MX_DISPLAY_WIDTH  X_SIZE
	#define MX_DISPLAY_HEIGHT Y_SIZE
	#define I2C_TIMEOUT					1000

	void TFT_DrawBitmap_d(uint32_t Xpos, uint32_t Ypos, const uint8_t *bitmap, uint16_t width, uint16_t height,  uint8_t layer);
	void test_output(void);
	void TFT_FillScreen(uint32_t color);
	void test_draw_all(void);
	void sec_handler(void);
	void handle_ms_tick(uint32_t);

#ifdef __cplusplus
	}
#endif


#endif /* INC_TOTAL_DEVICE_HPP_ */
