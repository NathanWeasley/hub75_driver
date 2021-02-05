#include "logger/logger_api.h"
#include "usart.h"
#include <stdarg.h>
#include <string.h>

#define NW_GET_LOG_LEVEL_STR()

char loglevel = LOGGER_INFO;
char logbuf[LOGGER_BUFFER_SIZE] = "";

void NW_Logger_Report(NW_LoggerLevel level, const char * fmt, ...)
{
    va_list valist;
    int bytes2send = 0;

    if (level < loglevel)
        return;

    va_start(valist, fmt);
    bytes2send = vsnprintf(logbuf, LOGGER_BUFFER_SIZE - 2, fmt, valist);
    va_end(valist);
    strcat(logbuf, LOGGER_CRLF);

    if (bytes2send > 2)
        NW_UART_Send_DMA(logbuf, bytes2send+2);
}
