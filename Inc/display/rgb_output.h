#ifndef NW_RGB_OUTPUT_H
#define NW_RGB_OUTPUT_H

#include "display/display_info.h"
#include "task/task.h"

/**
 * MAIN:  |INIT--------------------------|LOOP----------------------------------------------------------------------------| ...
 *        |<Prep line0 bit0><start timer>|<Prep line0 bit1, set oc> |<Prep line0 bit2, set oc> |<Prep line0 bit3, set oc> | ...
 * TIMER:                                |--------------------------|INT-----------------------|INT-----------------------| ...
 *                                       |<display line0 bit0>      |<display line0 bit1>      |<display line0 bit2>      | ...
 *                                                                  |<row-latch>               |<row-latch>               | ...
 */

extern uint8_t g_pVRAM[NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL];
extern uint8_t g_currentRow;

void NW_LED_PrepareLine();
void NW_LED_SelectRowAndLatch();
void NW_LED_SendLine();
uint8_t NW_LED_GetCurrentBit();
static inline uint8_t NW_LED_GetCurrentRow()
{
    return g_currentRow;
}
static inline uint8_t * NW_LED_GetVRAMAddr()
{
    return g_pVRAM;
}

NW_TaskId NW_LED_GetPrepareTask();
NW_TaskId NW_LED_CreatePrepareTask();

void NW_LED_Init();

#endif
