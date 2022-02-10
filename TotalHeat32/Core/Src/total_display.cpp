/*
 * total_display.cpp
 *
 *  Created on: Feb 9, 2022
 *      Author: TZprog2
 */

#include <total_display.hpp>
#include <iostream>
#include <cstring>

#include "Images/img_btn_timer_pause_rlsd.h"

extern LTDC_HandleTypeDef hltdc;


#ifdef __cplusplus
extern "C"
{
#endif

void TFT_DrawBitmap_d(uint32_t Xpos, uint32_t Ypos, const uint8_t *bitmap, uint16_t width, uint16_t height,  uint8_t layer){
	 uint32_t y,x,p=Xpos,c=0; //len=strlen((char*)bitmap);
  /* Set the address */
  uint32_t  address = hltdc.LayerCfg[layer].FBStartAdress +  Ypos*X_SIZE*4;
  /* Bypass the bitmap header */
  for(y=0;y<height;y++){
    //for(x=1;x<width+1;x++){
		for(x=0;x<width;x++){
		c=0;
		c=0x00000000;
		c |= (bitmap[((y*width)+x)*4+3]);
		c |= (bitmap[((y*width)+x)*4+2]) << 8;
		c |= (bitmap[((y*width)+x)*4+1]) << 16;
		c |= (bitmap[((y*width)+x)*4+0]) << 24;

      *(uint32_t*) (address + (y+x+p)*4) = c;//TTZ_COL_ORAN;
      if((y+x)>=(X_SIZE*Y_SIZE)) return;
    }
	p=p+X_SIZE-1;
  }
  return;
}

void test_output(void)
{
	HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(LAYER_PAGE_0), 0);

	TFT_DrawBitmap_d(0, 0, img_btn_timer_pause_rlsd.data, img_btn_timer_pause_rlsd.width, img_btn_timer_pause_rlsd.height, LAYER_PAGE_0);
}

void TFT_FillScreen(uint32_t color)
{
 uint32_t i;
 uint32_t n = hltdc.LayerCfg[0].ImageHeight*hltdc.LayerCfg[0].ImageWidth;
 for(i=0;i<n;i++)
 {
  *(uint32_t*) (hltdc.LayerCfg[0].FBStartAdress + (i*4)) = color;
 }
}

#ifdef __cplusplus
}
#endif
