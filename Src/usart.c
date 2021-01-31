/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

volatile int8_t g_bUartIsSending = 0;
char g_pUartReceiveBuffer[UART_CMD_BUFFER_SIZE] = { 0 };
char *g_pUartReceiveBufferReader = g_pUartReceiveBuffer;

/* USER CODE END 0 */

/* USART2 init function */

void MX_USART2_UART_Init(void)
{
  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
  
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration  
  PA2   ------> USART2_TX
  PA3   ------> USART2_RX 
  */
  GPIO_InitStruct.Pin = USART_TX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  LL_GPIO_Init(USART_TX_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = USART_RX_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  LL_GPIO_Init(USART_RX_GPIO_Port, &GPIO_InitStruct);

  /* USART2 DMA Init */
  
  /* USART2_TX Init */
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_7, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_7, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_7, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_7);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),3, 0));
  NVIC_EnableIRQ(USART2_IRQn);

  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);
  LL_USART_Enable(USART2);

}

/* USER CODE BEGIN 1 */

int8_t NW_UART_Send(const char * str, uint32_t len)
{
    uint32_t trytimes = 2000;

    if (!str || len == 0)
        return -2;

    while (len--)
    {
        while (g_bUartIsSending || !LL_USART_IsActiveFlag_TXE(USART2))
        {
            if (--trytimes == 0)
                return -1;
        }

        LL_USART_TransmitData8(USART2, *str);
        ++str;
    }

    return 0;
}

int8_t NW_UART_Send_DMA(const char * str, uint32_t len)
{
    if (!str || len == 0)
        return -2;

    ///< Wait for previous transfer
    while (g_bUartIsSending);

    LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_7);          ///< Disable DMA to load new length to be tranmitted
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_7, len);      ///< set length to be tranmitted
    LL_DMA_ConfigAddresses(DMA1, LL_DMA_CHANNEL_7, (uint32_t)str, (uint32_t)&(USART2->DR), LL_DMA_DIRECTION_MEMORY_TO_PERIPH); ///< configure address to be transmitted by DMA
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_7);           ///< Enable DMA again
    LL_USART_EnableDMAReq_TX(USART2);                       ///< Fire

    NW_UART_SetSendingFlag();

    return 0;
}

const char * NW_UART_Get_Received()
{
    return g_pUartReceiveBufferReader;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
