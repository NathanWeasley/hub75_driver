#ifndef LOGGER_API_H
#define LOGGER_API_H

#include <stdint.h>
#include <stdio.h>

#define LOGGER_BUFFER_SIZE      (1024)
#define LOGGER_CR               "\r"
#define LOGGER_LF               "\n"
#define LOGGER_CRLF             "\r\n"

typedef enum
{
    LOGGER_VERBOSE = 0,
    LOGGER_INFO,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL,

    LOGGER_LEVEL_CNT,
    LOGGER_FORCE
} logger_level_e;

extern char loglevel;
extern char logbuf[LOGGER_BUFFER_SIZE];

inline void configLoggerLevel(logger_level_e newlevel)
{
    loglevel = newlevel;
}

void report(logger_level_e level, const char * fmt, ...);

#endif
