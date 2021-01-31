#ifndef NW_RGB_OUTPUT_H
#define NW_RGB_OUTPUT_H

#include "display/display_info.h"

extern uint8_t g_pVRAM[NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH];

void NW_LED_Prepare_Line(uint8_t row);
void NW_LED_Send_Line();

#endif
