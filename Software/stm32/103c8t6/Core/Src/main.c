/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "string.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
int key_flag;

uint8_t data[2];

uint8_t RxBuff[1];      //进入中断接收数据的数组
uint8_t DataBuff[5000]; //保存接收到的数据的数组
int RxLine=0;           //接收到的数据长度
uint8_t trans[2] = {1,2};
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(data[0] == '0'){
		key_flag = 1; 
	}
	if(data[0] == '1'){
		key_flag = 2; 
	}
	if(data[0] == '2'){
		if(data[1] == '1'){
		key_flag = 3; 
		}
		else if(data[1] == '2'){
		key_flag = 4; 
		}
		else if(data[1] == '3'){
		key_flag = 5; 
		}
	}
	if(data[0] == '3'){
		key_flag = 6; 
	}
	HAL_UART_Receive_IT(&huart1,(uint8_t*)&data,sizeof(data));//打开下一次串口接收中断
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin == Key_1_Pin){
		key_flag = 1; 
		HAL_UART_Transmit_IT(&huart1,"1",sizeof("1"));
	}
	else if(GPIO_Pin == Key_2_Pin){

	}
	else if(GPIO_Pin == Key_3_Pin){
		HAL_UART_Transmit_IT(&huart1,"5",sizeof("5"));

	}
	else if(GPIO_Pin == Key_4_Pin){
	}
	else if(GPIO_Pin == TTP_223_Pin){
		HAL_UART_Transmit_IT(&huart1,"2",sizeof("2"));
	}
}
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
  MX_I2C1_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	oled_init();
	HAL_UART_Receive_IT(&huart1,(uint8_t*)data,sizeof(data));//打开下一次串口接收中断


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//		if(HAL_GPIO_ReadPin(TTP_223_GPIO_Port,TTP_223_Pin)){
//			HAL_GPIO_WritePin(LED_red_GPIO_Port,LED_red_Pin,GPIO_PIN_RESET);

//		}
//		else{
//			HAL_GPIO_WritePin(LED_red_GPIO_Port,LED_red_Pin,GPIO_PIN_SET);

//		}

		switch(key_flag){
			case 1:
				oled_show_string(0,0,"mode 1",2);
				break;
			case 2:
				oled_show_string(0,0,"mode 2",2);
				break;
			//white
			case 3:
				oled_show_string(0,0,"mode 3",2);
				HAL_GPIO_WritePin(LED_yellow_GPIO_Port,LED_yellow_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_white_GPIO_Port,LED_white_Pin,GPIO_PIN_SET);
				break;
			//yellow
			case 4:
				oled_show_string(0,0,"mode 3",2);
				HAL_GPIO_WritePin(LED_yellow_GPIO_Port,LED_yellow_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_white_GPIO_Port,LED_white_Pin,GPIO_PIN_RESET);
				break;
			//shanshuo
			case 5:
				oled_show_string(0,0,"mode 3",2);
				HAL_GPIO_WritePin(LED_yellow_GPIO_Port,LED_yellow_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(LED_white_GPIO_Port,LED_white_Pin,GPIO_PIN_RESET);
				HAL_Delay(400);
				HAL_GPIO_WritePin(LED_yellow_GPIO_Port,LED_yellow_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_white_GPIO_Port,LED_white_Pin,GPIO_PIN_SET);
				HAL_Delay(400);
				break;
			case 6:
				oled_show_string(0,0,"mode 4",2);
				break;
		}
		oled_show_string(0,5,(char*)data,2);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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

