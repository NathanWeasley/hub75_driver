#include "display/rgb_output.h"
#include "logger/logger_api.h"
#include "spi.h"
#include "gpio.h"
#include "tim.h"

#include "stm32f103xb.h"

#include <stdint.h>
#include <string.h>

#define NW_ROW_GROUP_CNT    (NW_DISPLAY_HEIGHT / NW_DISPLAY_SCAN_DIVIDER)
#define NW_ROW_JUMP         (NW_DISPLAY_WIDTH * NW_DISPLAY_SCAN_DIVIDER)

uint8_t g_pBitMask[NW_DISPLAY_DEPTH];               ///< This mask table is naturally listed as [1, 2, 4, 8, 16, ...]
uint8_t g_pVRAM[NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL];
uint8_t g_pLineData[NW_BITS_PER_LINE / 8];
uint8_t g_currentRow = 0;
uint8_t g_currentBit = 0;
const uint16_t g_maxIntensity = 1 << NW_DISPLAY_DEPTH;

NW_TaskId g_rowPrepareTaskId = NW_InvalidTask;

void NW_LED_PrepareLine()
{
    uint8_t k, bmask = g_pBitMask[g_currentBit];
    uint8_t *pRRow, *pGRow, *pBRow, *pLine = g_pLineData;
    uint32_t i, j, rowbias = NW_DISPLAY_WIDTH * (uint32_t)g_currentRow;
    int8_t res;

    /** Process row group */

    pRRow = g_pVRAM + rowbias;
    pGRow = g_pVRAM + rowbias + NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH;
    pBRow = g_pVRAM + rowbias + NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * 2;
    for (i = 0; i < NW_ROW_GROUP_CNT; ++i)
    {
        k = 0x01;

        /** TODO: Adaption for flexible channel count */
        
        for (j = 0; j < NW_DISPLAY_WIDTH; ++j, ++pRRow)
        {
            *pLine |= (*pRRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
            {
                k = 0x01;
                ++pLine;
            }
        }

        for (j = 0; j < NW_DISPLAY_WIDTH; ++j, ++pGRow)
        {
            *pLine |= (*pGRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
            {
                k = 0x01;
                ++pLine;
            }
        }

        for (j = 0; j < NW_DISPLAY_WIDTH; ++j, ++pBRow)
        {
            *pLine |= (*pBRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
            {
                k = 0x01;
                ++pLine;
            }
        }

        pRRow += NW_ROW_JUMP - NW_DISPLAY_WIDTH;
        pGRow += NW_ROW_JUMP - NW_DISPLAY_WIDTH;
        pBRow += NW_ROW_JUMP - NW_DISPLAY_WIDTH;
    }

    /** Increment on channel bits and row */

    if (++g_currentBit >= NW_DISPLAY_DEPTH)
    {
        g_currentBit = 0;
        if (++g_currentRow >= NW_DISPLAY_SCAN_DIVIDER)
            g_currentRow = 0;
    }

    /** Send Data through SPI */
    res = NW_SPI2_Send_DMA((uint32_t)g_pLineData, NW_BITS_PER_LINE / 8);
    if (0 != res)
    {
        NW_Logger_Report(LOGGER_ERROR, "SPI2 DMA send failure: %d.", res);
        return;
    }

    /** Preload PWM OC counter */
    LL_TIM_OC_SetCompareCH1(TIM2, TIM2_RELOAD - TIM2_RELOAD / g_pBitMask[NW_DISPLAY_DEPTH-1 - g_currentBit]);

    /** Debugging only */
    // TOGGLE_USER_LED;
}

void NW_LED_SelectRowAndLatch()
{
    /** Select current row */
    GPIOC->ODR = 0x0F & NW_LED_GetCurrentRow();

    /** Latch loaded data */
    RGBLED_LAT_GPIO_Port->BSRR = RGBLED_LAT_Pin;
    RGBLED_LAT_GPIO_Port->BRR = RGBLED_LAT_Pin;
}

uint8_t NW_LED_GetCurrentBit()
{
    return g_pBitMask[NW_DISPLAY_DEPTH-1 - g_currentBit];
}



NW_TaskId NW_LED_GetPrepareTask()
{
    return g_rowPrepareTaskId;
}

NW_TaskId NW_LED_CreatePrepareTask()
{
    NW_TaskId tid = -1;

    tid = NW_Task_Create(NW_LED_PrepareLine, 0, NW_TASK_SLEEP);
    if (!NW_IsTaskValid(tid))
        NW_Logger_Report(LOGGER_ERROR, "LED row prepare task create failure.");

    return tid;
}

void NW_LED_Init()
{
    uint8_t i;
    uint8_t * pR = g_pVRAM;
    uint8_t * pG = g_pVRAM + NW_DISPLAY_WIDTH * NW_DISPLAY_HEIGHT;
    // uint8_t * pB = g_pVRAM + NW_DISPLAY_WIDTH * 2;

    /** Initialize bit mask table */
    for (i = 0; i < NW_DISPLAY_DEPTH; ++i)
        g_pBitMask[i] = 1 << i;

    /** Reset row and bit pointers */
    g_currentRow = 0;
    g_currentBit = 0;

    /** Zero-initialize all vmems */
    memset(g_pVRAM, 0x00, NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL);
    memset(g_pLineData, 0x00, NW_BITS_PER_LINE / 8);

    /** TESTONLY: Fill the first row in VRAM */
    for (i = 0; i < NW_DISPLAY_WIDTH; ++i)
    {
        pR[i] = i * g_maxIntensity / NW_DISPLAY_WIDTH;
        pG[i] = (NW_DISPLAY_WIDTH - i - 1) * g_maxIntensity / NW_DISPLAY_WIDTH;
    }

    /** Enable SPI */
    LL_SPI_Enable(SPI2);

    /** Prepare first line and latch */
    NW_LED_PrepareLine();
    NW_LED_SelectRowAndLatch();

    /** Startup TIM2 for timing and PWM generation */
    LL_TIM_EnableIT_UPDATE(TIM2);
    LL_TIM_EnableUpdateEvent(TIM2);
    LL_TIM_EnableCounter(TIM2);
}
