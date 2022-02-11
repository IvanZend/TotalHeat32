/*
 * total_display.h
 *
 *  Created on: Feb 9, 2022
 *      Author: TZprog2
 */

#ifndef INC_TOTAL_DISPLAY_HPP_
#define INC_TOTAL_DISPLAY_HPP_

#include "stm32f4xx_hal.h"

typedef struct
{
const uint8_t *data;
uint16_t width;
uint16_t height;
uint8_t dataSize;
} tImage;



#define LCD_FRAME_BUFFER(N) ((uint32_t)(0xC0000000 + (N)*0x1D4C00))
#define LAYER_PAGE_0		0
#define LAYER_PAGE_1		1
#define LAYER_PAGE_2		2
#define LAYER_PAGE_3		3
#define LAYER_PAGE_4		4
#define X_SIZE	 800
#define Y_SIZE	 480

#define MX_DISPLAY_WIDTH  X_SIZE
#define MX_DISPLAY_HEIGHT Y_SIZE

#ifdef __cplusplus
	extern "C"
	{
#endif

	void TFT_DrawBitmap_d(uint32_t Xpos, uint32_t Ypos, const uint8_t *bitmap, uint16_t width, uint16_t height,  uint8_t layer);
	void test_output(void);
	void TFT_FillScreen(uint32_t color);

#ifdef __cplusplus
	}
#endif


#endif /* INC_TOTAL_DISPLAY_HPP_ */
