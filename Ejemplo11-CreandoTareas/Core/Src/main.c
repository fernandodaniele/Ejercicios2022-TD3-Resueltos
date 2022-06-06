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
#include "cmsis_os.h"
#include "i2c.h"
#include "i2s.h"
#include "spi.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "usbd_cdc_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define	BLOQUEANTE	0
#define PULSADOR	0
#define MONITOREO	1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void tareaPrimaria( void * pvParameters ); //Prototipo de la tarea
void tareaSecundaria( void * pvParameters ); //Prototipo de la tarea
void tareaMonitoreo ( void * pvParameters );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* Configure the system clock */
	SystemClock_Config();
	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USB_DEVICE_Init();

	TaskHandle_t xHandle1, xHandle2, xHandle3 = NULL;           //Punteros a la tarea

    xTaskCreate( tareaPrimaria,           //Función que implementa la tarea. La misma no debe retornar.
                 "Tarea Primaria",        //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+1,          //Prioridad de la tarea
                 &xHandle1                  //Puntero a la tarea
                );
    configASSERT( xHandle1 );                //entra si no se pudieron crear las tareas, para debug

    xTaskCreate( tareaSecundaria,           //Función que implementa la tarea. La misma no debe retornar.
                 "Tarea secundaria",        //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+1,          //Prioridad de la tarea
                 &xHandle2                  //Puntero a la tarea
                );
    configASSERT( xHandle2 );                //entra si no se pudieron crear las tareas, para debug
#if	MONITOREO
    xTaskCreate( tareaMonitoreo,           //Función que implementa la tarea. La misma no debe retornar.
                 "tareaMonitoreo",        //Nombre que reprenta la tarea, para facilitar la depuración.
                 configMINIMAL_STACK_SIZE,  //Tamaño del stack en bytes
                 NULL,                      //Puntero que se utilizará como parámetro para la tarea que se está creando. Como no lo usamos ponemos NULL
                 tskIDLE_PRIORITY+1,          //Prioridad de la tarea
                 &xHandle3                  //Puntero a la tarea
                );
    configASSERT( xHandle3 );                //entra si no se pudieron crear las tareas, para debug
#endif
    vTaskStartScheduler(); // Iniciar scheduler

    while (1)
    {
    	// Si cae en este while 1 significa que no pudo iniciar el scheduler
    }
}

void tareaPrimaria( void * pvParameters )
{
    while (1)
    {
#if BLOQUEANTE
		HAL_GPIO_WritePin(GPIOD, LD3_Pin,1);
		HAL_Delay(1);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin,0);
		HAL_Delay(1);
#else
		HAL_GPIO_WritePin(GPIOD, LD3_Pin,1);
		vTaskDelay(1);
		HAL_GPIO_WritePin(GPIOD, LD3_Pin,0);
		vTaskDelay(1);
#endif
    }
}

void tareaSecundaria( void * pvParameters )
{
    while (1)
    {
#if BLOQUEANTE
  	  HAL_GPIO_WritePin(GPIOD, LD4_Pin,1);
  	  HAL_Delay(1);
  	  HAL_GPIO_WritePin(GPIOD, LD4_Pin,0);
  	  HAL_Delay(1);
#else
  	  HAL_GPIO_WritePin(GPIOD, LD4_Pin,1);
  	  vTaskDelay(1);
  	  HAL_GPIO_WritePin(GPIOD, LD4_Pin,0);
  	  vTaskDelay(1);
#endif
#if	PULSADOR
		if (HAL_GPIO_ReadPin(GPIOA, B1_Pin) == 1)
		{
			vTaskDelete( NULL );
		}
#endif
    }
}

#if	MONITOREO
//Borra la tarea usando el handle
void tareaMonitoreo( void * pvParameters )
{
	int stack1, stack2;
	uint8_t data[50] = "";
    while (1)
    {
    	stack1 = uxTaskGetStackHighWaterMark( xHandle1);
		stack2 = uxTaskGetStackHighWaterMark( xHandle2);
		sprintf(data,"stack1 %d - stack2 %d\n", stack1,stack2);
		CDC_Transmit_FS(data, strlen(data));
		vTaskDelay(1000);
    }
}
#endif
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
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
