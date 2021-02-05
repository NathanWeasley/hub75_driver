#include "display/rgb_output.h"
#include "logger/logger_api.h"
#include "spi.h"

#include <string.h>

#define NW_ROW_GROUP_CNT    (NW_DISPLAY_HEIGHT / NW_DISPLAY_SCAN_DIVIDER)
#define NW_ROW_JUMP         (NW_DISPLAY_WIDTH * NW_DISPLAY_SCAN_DIVIDER)

uint8_t g_pBitMask[NW_DISPLAY_DEPTH];
uint8_t g_pVRAM[NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL];
uint8_t g_pLineData[NW_BITS_PER_LINE / 8];
uint8_t g_currentRow = 0;
uint8_t g_currentBit = 0;

NW_TaskId g_rowPrepareTaskId = NW_InvalidTask;

void NW_LED_PrepareLine()
{
    uint8_t i, j, k, bmask = g_pBitMask[g_currentBit];
    uint8_t *pRRow, *pGRow, *pBRow, *pLine = g_pLineData;
    uint32_t rowbias = NW_DISPLAY_WIDTH * (uint32_t)g_currentRow;

    /** Process row group */

    pRRow = g_pVRAM + rowbias;
    pGRow = g_pVRAM + rowbias + NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH;
    pBRow = g_pVRAM + rowbias + NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * 2;
    for (i = 0; i < NW_ROW_GROUP_CNT; ++i)
    {
        /** TODO: Adaption for flexible channel count */
        
        for (j = 0, k = 0x01; j < NW_DISPLAY_WIDTH; ++j, ++pRRow)
        {
            *pLine |= (*pRRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
                ++pLine;
        }

        for (j = 0, k = 0x01; j < NW_DISPLAY_WIDTH; ++j, ++pGRow)
        {
            *pLine |= (*pGRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
                ++pLine;
        }

        for (j = 0, k = 0x01; j < NW_DISPLAY_WIDTH; ++j, ++pBRow)
        {
            *pLine |= (*pBRow & bmask) ? k : 0x00;          ///< Fill corresponding bit in line data
            if ((k <<= 1) == 0)
                ++pLine;
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


    /** Preload PWM OC counter */
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

    /** Initialize bit mask table */
    for (i = 0; i < NW_DISPLAY_DEPTH; ++i)
        g_pBitMask[i] = 1 << i;
    
    /** Reset row and bit pointers */
    g_currentRow = 0;
    g_currentBit = 0;

    /** Zero-initialize all vmems */
    memset(g_pVRAM, 0x00, NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL);
    memset(g_pLineData, 0x00, NW_BITS_PER_LINE / 8);
}
