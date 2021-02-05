#ifndef LOGGER_API_H
#define LOGGER_API_H

#include <stdint.h>
#include <stdio.h>

#define LOGGER_BUFFER_SIZE      (1024)
#define LOGGER_CR               "\r"
#define LOGGER_LF               "\n"
#define LOGGER_CRLF             "\r\n"

#define LOGGER_VERBOSE          (0)
#define LOGGER_INFO             (1)
#define LOGGER_WARNING          (2)
#define LOGGER_ERROR            (3)
#define LOGGER_FATAL            (4)
#define LOGGER_FORCE            (5)

#define LOGGER_VERBOSE_STR      "[V] "
#define LOGGER_INFO_STR         "[I] "
#define LOGGER_WARNING_STR      "[W] "
#define LOGGER_ERROR_STR        "[E] "
#define LOGGER_FATAL_STR        "[F] "
#define LOGGER_FORCE_STR        "[P] "

typedef uint8_t NW_LoggerLevel;

extern char loglevel;
extern char logbuf[LOGGER_BUFFER_SIZE];

inline void NW_Logger_ConfigLevel(NW_LoggerLevel newlevel)
{
    loglevel = newlevel;
}

void NW_Logger_Report(NW_LoggerLevel level, const char * fmt, ...);

#endif
