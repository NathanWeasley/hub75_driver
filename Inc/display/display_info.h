#ifndef NW_DISPLAY_INFO_H
#define NW_DISPLAY_INFO_H

#include <stdint.h>

#define NW_DESIRED_FPS          (60)

#define NW_DISPLAY_WIDTH        (64)
#define NW_DISPLAY_HEIGHT       (32)
#define NW_DISPLAY_CHANNEL      (3)         ///< 1 for grayscale, 3 for RGB
#define NW_DISPLAY_DEPTH        (5)         ///< Color depth in bits
#define NW_DISPLAY_SCAN_DIVIDER (16)        ///< 1/16 scan

#define NW_FIELD_PER_FRAME      (NW_DISPLAY_SCAN_DIVIDER)
#define NW_BITS_PER_LINE        (NW_DISPLAY_WIDTH*NW_DISPLAY_CHANNEL*(NW_DISPLAY_HEIGHT/NW_DISPLAY_SCAN_DIVIDER))
#define NW_BYTES_PER_FRAME      (NW_DISPLAY_WIDTH*NW_DISPLAY_HEIGHT*NW_DISPLAY_CHANNEL)

#endif
