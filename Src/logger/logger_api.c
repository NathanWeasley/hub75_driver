#include "logger/logger_api.h"
#include "stm32f1xx.h"
#include "stm32f1xx_hal_uart.h"
#include "usart.h"
#include <stdarg.h>
#include <string.h>

char loglevel = LOGGER_INFO;
char logbuf[LOGGER_BUFFER_SIZE] = "";

void report(logger_level_e level, const char * fmt, ...)
{
    va_list valist;
    int bytes2send = 0;

    if (level < loglevel)
        return;

    va_start(valist, fmt);
    bytes2send = snprintf(logbuf, LOGGER_BUFFER_SIZE, fmt, valist);
    va_end(valist);

    if (bytes2send > 0)
    {
        HAL_UART_Transmit(&huart2, logbuf, bytes2send, bytes2send / 20 + 1);
        HAL_UART_Transmit(&huart2, LOGGER_CRLF, 2, 1);
    }
}
