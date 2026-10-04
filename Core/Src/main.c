/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "dma.h"
#include "gpio.h"
#include "tim.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define PWM_PAYLOAD_SIZE 8

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

COM_InitTypeDef BspCOMInit;

/* USER CODE BEGIN PV */

// center-aligned mode generates two update events per PWM period
uint16_t pwm_arr[PWM_PAYLOAD_SIZE] = {1000 - 1, 1000 - 1, 500 - 1,  500 - 1,
                                      1000 - 1, 1000 - 1, 1000 - 1, 1000 - 1};
uint16_t pwm_ch1_ccr[PWM_PAYLOAD_SIZE] = {100 - 1, 100 - 1, 200 - 1,  200 - 1,
                                          300 - 1, 300 - 1, 1000 - 1, 1000 - 1};
uint16_t pwm_ch2_ccr[PWM_PAYLOAD_SIZE] = {300 - 1, 300 - 1, 200 - 1,  200 - 1,
                                          100 - 1, 100 - 1, 1000 - 1, 1000 - 1};
uint16_t pwm_ch3_ccr[PWM_PAYLOAD_SIZE] = {500 - 1, 500 - 1, 200 - 1,  200 - 1,
                                          400 - 1, 400 - 1, 1000 - 1, 1000 - 1};

HAL_StatusTypeDef hal_status; // for debugging

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

// note removed __HAL_TIM_ENABLE(htim);
HAL_StatusTypeDef HAL_TIM_Base_Start_DMA_BU(TIM_HandleTypeDef *htim,
                                            const uint32_t *pData,
                                            uint16_t Length);

HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA_BU(TIM_HandleTypeDef *htim,
                                           uint32_t Channel,
                                           const uint32_t *pData,
                                           uint16_t Length);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  // start DMA without starting TIM
  hal_status =
      HAL_TIM_Base_Start_DMA_BU(&htim1, (uint32_t *)pwm_arr, PWM_PAYLOAD_SIZE);
  TIM1->EGR = TIM_EGR_UG; // / DMA writes to ARR working register
  while (!((TIM1->SR) & TIM_SR_UIF)) {
    __NOP();
  }

  hal_status = HAL_TIM_PWM_Start_DMA_BU(
      &htim1, TIM_CHANNEL_1, (uint32_t *)pwm_ch1_ccr, PWM_PAYLOAD_SIZE);
  hal_status = HAL_TIM_PWM_Start_DMA_BU(
      &htim1, TIM_CHANNEL_2, (uint32_t *)pwm_ch2_ccr, PWM_PAYLOAD_SIZE);
  hal_status = HAL_TIM_PWM_Start_DMA_BU(
      &htim1, TIM_CHANNEL_3, (uint32_t *)pwm_ch3_ccr, PWM_PAYLOAD_SIZE);

  TIM1->EGR = TIM_EGR_CC1G; // DMA writes to CCR1 working register
  while (!((TIM1->SR) & TIM_SR_CC1IF)) {
    __NOP();
  }

  TIM1->EGR = TIM_EGR_CC2G; // DMA writes to CCR2 working register
  while (!((TIM1->SR) & TIM_SR_CC2IF)) {
    __NOP();
  }

  TIM1->EGR = TIM_EGR_CC3G; // DMA writes to CCR3 working register
  while (!((TIM1->SR) & TIM_SR_CC3IF)) {
    __NOP();
  }

  TIM1->EGR =
      TIM_EGR_UG; // TIM copies working (preload) registers to shadow registers
  while (!((TIM1->SR) & TIM_SR_UIF)) {
    __NOP();
  }

  HAL_GPIO_WritePin(LOGIC_ANALYZER_PA11_GPIO_Port, LOGIC_ANALYZER_PA11_Pin,
                    GPIO_PIN_SET);
  HAL_Delay(2); // just to illustrate that signals are not yet generated
  __HAL_TIM_ENABLE(&htim1);

  /* USER CODE END 2 */

  /* Initialize COM1 port (115200, 8 bits (7-bit data + 1 stop bit), no parity
   */
  BspCOMInit.BaudRate = 115200;
  BspCOMInit.WordLength = COM_WORDLENGTH_8B;
  BspCOMInit.StopBits = COM_STOPBITS_1;
  BspCOMInit.Parity = COM_PARITY_NONE;
  BspCOMInit.HwFlowCtl = COM_HWCONTROL_NONE;
  if (BSP_COM_Init(COM1, &BspCOMInit) != BSP_ERROR_NONE) {
    Error_Handler();
  }

  /* USER CODE BEGIN BSP */

  /* -- Sample board code to send message over COM1 port ---- */
  printf("\r\nWelcome to STM32 TIM DMA preload world!\r\n");

  /* USER CODE END BSP */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    __NOP();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  if ((htim->Instance == TIM1) && (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)) {
    HAL_GPIO_TogglePin(LOGIC_ANALYZER_PA11_GPIO_Port, LOGIC_ANALYZER_PA11_Pin);
  }
}

#if (0) // added to stm32g4xx_hal_tim.h

HAL_StatusTypeDef HAL_TIM_Base_Start_DMA_BU(TIM_HandleTypeDef *htim,
                                            const uint32_t *pData,
                                            uint16_t Length) {
  uint32_t tmpsmcr;

  /* Check the parameters */
  assert_param(IS_TIM_DMA_INSTANCE(htim->Instance));

  /* Set the TIM state */
  if (htim->State == HAL_TIM_STATE_BUSY) {
    return HAL_BUSY;
  } else if (htim->State == HAL_TIM_STATE_READY) {
    if ((pData == NULL) || (Length == 0U)) {
      return HAL_ERROR;
    } else {
      htim->State = HAL_TIM_STATE_BUSY;
    }
  } else {
    return HAL_ERROR;
  }

  /* Set the DMA Period elapsed callbacks */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferCpltCallback = TIM_DMAPeriodElapsedCplt;
  htim->hdma[TIM_DMA_ID_UPDATE]->XferHalfCpltCallback =
      TIM_DMAPeriodElapsedHalfCplt;

  /* Set the DMA error callback */
  htim->hdma[TIM_DMA_ID_UPDATE]->XferErrorCallback = TIM_DMAError;

  /* Enable the DMA channel */
  if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)pData,
                       (uint32_t)&htim->Instance->ARR, Length) != HAL_OK) {
    /* Return error status */
    return HAL_ERROR;
  }

  /* Enable the TIM Update DMA request */
  __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_UPDATE);

  /* Enable the Peripheral, except in trigger mode where enable is automatically
   * done with trigger */
  if (IS_TIM_SLAVE_INSTANCE(htim->Instance)) {
    tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
    if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr)) {
      // __HAL_TIM_ENABLE(htim);
    }
  } else {
    // __HAL_TIM_ENABLE(htim);
  }

  /* Return function status */
  return HAL_OK;
}

HAL_StatusTypeDef HAL_TIM_PWM_Start_DMA_BU(TIM_HandleTypeDef *htim,
                                           uint32_t Channel,
                                           const uint32_t *pData,
                                           uint16_t Length) {
  HAL_StatusTypeDef status = HAL_OK;
  uint32_t tmpsmcr;

  /* Check the parameters */
  assert_param(IS_TIM_CCX_CHANNEL(htim->Instance, Channel));

  /* Set the TIM channel state */
  if (TIM_CHANNEL_STATE_GET(htim, Channel) == HAL_TIM_CHANNEL_STATE_BUSY) {
    return HAL_BUSY;
  } else if (TIM_CHANNEL_STATE_GET(htim, Channel) ==
             HAL_TIM_CHANNEL_STATE_READY) {
    if ((pData == NULL) || (Length == 0U)) {
      return HAL_ERROR;
    } else {
      TIM_CHANNEL_STATE_SET(htim, Channel, HAL_TIM_CHANNEL_STATE_BUSY);
    }
  } else {
    return HAL_ERROR;
  }

  switch (Channel) {
  case TIM_CHANNEL_1: {
    /* Set the DMA compare callbacks */
    htim->hdma[TIM_DMA_ID_CC1]->XferCpltCallback = TIM_DMADelayPulseCplt;
    htim->hdma[TIM_DMA_ID_CC1]->XferHalfCpltCallback =
        TIM_DMADelayPulseHalfCplt;

    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_CC1]->XferErrorCallback = TIM_DMAError;

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC1], (uint32_t)pData,
                         (uint32_t)&htim->Instance->CCR1, Length) != HAL_OK) {
      /* Return error status */
      return HAL_ERROR;
    }

    /* Enable the TIM Capture/Compare 1 DMA request */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC1);
    break;
  }

  case TIM_CHANNEL_2: {
    /* Set the DMA compare callbacks */
    htim->hdma[TIM_DMA_ID_CC2]->XferCpltCallback = TIM_DMADelayPulseCplt;
    htim->hdma[TIM_DMA_ID_CC2]->XferHalfCpltCallback =
        TIM_DMADelayPulseHalfCplt;

    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_CC2]->XferErrorCallback = TIM_DMAError;

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC2], (uint32_t)pData,
                         (uint32_t)&htim->Instance->CCR2, Length) != HAL_OK) {
      /* Return error status */
      return HAL_ERROR;
    }
    /* Enable the TIM Capture/Compare 2 DMA request */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC2);
    break;
  }

  case TIM_CHANNEL_3: {
    /* Set the DMA compare callbacks */
    htim->hdma[TIM_DMA_ID_CC3]->XferCpltCallback = TIM_DMADelayPulseCplt;
    htim->hdma[TIM_DMA_ID_CC3]->XferHalfCpltCallback =
        TIM_DMADelayPulseHalfCplt;

    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_CC3]->XferErrorCallback = TIM_DMAError;

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC3], (uint32_t)pData,
                         (uint32_t)&htim->Instance->CCR3, Length) != HAL_OK) {
      /* Return error status */
      return HAL_ERROR;
    }
    /* Enable the TIM Output Capture/Compare 3 request */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC3);
    break;
  }

  case TIM_CHANNEL_4: {
    /* Set the DMA compare callbacks */
    htim->hdma[TIM_DMA_ID_CC4]->XferCpltCallback = TIM_DMADelayPulseCplt;
    htim->hdma[TIM_DMA_ID_CC4]->XferHalfCpltCallback =
        TIM_DMADelayPulseHalfCplt;

    /* Set the DMA error callback */
    htim->hdma[TIM_DMA_ID_CC4]->XferErrorCallback = TIM_DMAError;

    /* Enable the DMA channel */
    if (HAL_DMA_Start_IT(htim->hdma[TIM_DMA_ID_CC4], (uint32_t)pData,
                         (uint32_t)&htim->Instance->CCR4, Length) != HAL_OK) {
      /* Return error status */
      return HAL_ERROR;
    }
    /* Enable the TIM Capture/Compare 4 DMA request */
    __HAL_TIM_ENABLE_DMA(htim, TIM_DMA_CC4);
    break;
  }

  default:
    status = HAL_ERROR;
    break;
  }

  if (status == HAL_OK) {
    /* Enable the Capture compare channel */
    TIM_CCxChannelCmd(htim->Instance, Channel, TIM_CCx_ENABLE);

    if (IS_TIM_BREAK_INSTANCE(htim->Instance) != RESET) {
      /* Enable the main output */
      __HAL_TIM_MOE_ENABLE(htim);
    }

    /* Enable the Peripheral, except in trigger mode where enable is
     * automatically done with trigger */
    if (IS_TIM_SLAVE_INSTANCE(htim->Instance)) {
      tmpsmcr = htim->Instance->SMCR & TIM_SMCR_SMS;
      if (!IS_TIM_SLAVEMODE_TRIGGER_ENABLED(tmpsmcr)) {
        // __HAL_TIM_ENABLE(htim);
      }
    } else {
      // __HAL_TIM_ENABLE(htim);
    }
  }

  /* Return function status */
  return status;
}

#endif

/* USER CODE END 4 */

/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/**
 * @}
 */

/**
 * @}
 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
