/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "W9812G6.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
LTDC_HandleTypeDef hltdc;

SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LTDC_Init(void);
static void MX_FMC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LTDC_Init();
  MX_FMC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  w9812g6_init(&hsdram1);

  for(uint8_t i=0; i<MAX_LAYER;i++)
  {
	  HAL_LTDC_SetAddress(&hltdc,LCD_FRAME_BUFFER(i), i);
  }

  HAL_GPIO_WritePin(LTDC_PWM_GPIO_Port, LTDC_PWM_Pin, GPIO_PIN_SET);
  //TFT_FillScreen(0xFF000000);
  test_output();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 10;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 47;
  hltdc.Init.VerticalSync = 2;
  hltdc.Init.AccumulatedHBP = 135;
  hltdc.Init.AccumulatedVBP = 34;
  hltdc.Init.AccumulatedActiveW = 935;
  hltdc.Init.AccumulatedActiveH = 514;
  hltdc.Init.TotalWidth = 975;
  hltdc.Init.TotalHeigh = 527;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 0;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 0;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  pLayerCfg.Alpha = 0;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0;
  pLayerCfg.ImageWidth = 0;
  pLayerCfg.ImageHeight = 0;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;

  const int Horizontal_Synchronization_Width      = 48;//Horizontal Synchronization Width must be between 1 pixels and 4.095 Kpixels.
  const int Horizontal_Back_Porch	                =	88;// 	Horizontal Back Porch must be between 0 pixels and 4.095 Kpixels.
  const int Horizontal_Front_Porch                = 40;//    Horizontal Front Porch must be between 0 pixels and 4.095 Kpixels.
  const int Vertical_Synchronization_Height       = 3;//		  Vertical Synchronization Height must be between 1 lines and 2.047 Klines.
  const int Vertical_Back_Porch                   = 32;// 		Vertical Back Porch must be between 0 lines and 2.047 Klines.
  const int Vertical_Front_Porch                  = 13;//		Vertical Front Porch must be between 0 lines and 2.047 Klines.

  hltdc.Init.HorizontalSync = Horizontal_Synchronization_Width-1;//127;      //This register value is equal to (Horizontal Synchronization Width - 1)
  hltdc.Init.VerticalSync = Vertical_Synchronization_Height-1;          //This register value is equal to (Vertical Synchronization Width - 1)
  hltdc.Init.AccumulatedHBP = Horizontal_Synchronization_Width+Horizontal_Back_Porch-1;      //This register value is equal to the (Horizontal Synchronization Width + Horizontal Back Porch - 1)
  hltdc.Init.AccumulatedVBP = Vertical_Synchronization_Height+Vertical_Back_Porch-1;       //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch - 1)
  hltdc.Init.AccumulatedActiveW = MX_DISPLAY_WIDTH+Horizontal_Synchronization_Width+Horizontal_Back_Porch-1; //This register value is equal to (Horizontal Synchronization Width + Horizontal Back Porch + Active Width - 1)
  hltdc.Init.AccumulatedActiveH = MX_DISPLAY_HEIGHT+Vertical_Synchronization_Height+Vertical_Back_Porch-1;  //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch + Active Height - 1)
  hltdc.Init.TotalWidth = MX_DISPLAY_WIDTH+Horizontal_Synchronization_Width+Horizontal_Back_Porch+Horizontal_Front_Porch-1;          //This register value is equal to (Horizontal Synchronization Width + Horizontal Back Porch + Active Width + Horizontal Front Porch - 1)
  hltdc.Init.TotalHeigh = MX_DISPLAY_HEIGHT+Vertical_Synchronization_Height+Vertical_Back_Porch+Vertical_Front_Porch-1;          //This register value is equal to the (Vertical Synchronization Width + Vertical Back Porch + Active Height + Vertical Front Porch - 1)
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  for(int n=0;n<MAX_LAYER;n++){
	  pLayerCfg.WindowX0 = 0;
	  pLayerCfg.WindowX1 = n==0?MX_DISPLAY_WIDTH:0;
	  pLayerCfg.WindowY0 = 0;
	  pLayerCfg.WindowY1 = n==0?MX_DISPLAY_HEIGHT:0;
	  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
	  pLayerCfg.Alpha = 255;
	  pLayerCfg.Alpha0 = 0;
	  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
	  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
	  pLayerCfg.FBStartAdress = LCD_FRAME_BUFFER(n);
	  pLayerCfg.ImageWidth = MX_DISPLAY_WIDTH;
	  pLayerCfg.ImageHeight = MX_DISPLAY_HEIGHT;
	  pLayerCfg.Backcolor.Blue = 0;
	  pLayerCfg.Backcolor.Green = 0;
	  pLayerCfg.Backcolor.Red = 0;
	  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, n) != HAL_OK)
	  {
		Error_Handler();
	  }
  }
  /* USER CODE END LTDC_Init 2 */

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_DISABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 1;
  SdramTiming.ExitSelfRefreshDelay = 1;
  SdramTiming.SelfRefreshTime = 1;
  SdramTiming.RowCycleDelay = 4;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 1;
  SdramTiming.RCDDelay = 1;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTDC_PWM_GPIO_Port, LTDC_PWM_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LTDC_RES_GPIO_Port, LTDC_RES_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LTDC_PWM_Pin */
  GPIO_InitStruct.Pin = LTDC_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LTDC_PWM_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LTDC_RES_Pin */
  GPIO_InitStruct.Pin = LTDC_RES_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LTDC_RES_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void delay_us(uint32_t delay)
{
	uint32_t i=0;
	//byte j=0;
	for (i=0;i<(delay*10);i++)
	{
		asm("NOP");
	}
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

