#include "display/rgb_output.h"

uint8_t g_pVRAM[NW_DISPLAY_HEIGHT * NW_DISPLAY_WIDTH * NW_DISPLAY_CHANNEL];
uint8_t g_pLineData[NW_BITS_PER_LINE / 8];

void NW_LED_Prepare_Line(uint8_t row)
{
    
}
