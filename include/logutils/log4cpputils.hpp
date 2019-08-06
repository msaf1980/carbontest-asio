/*
 * Macros for generic logging with printf-like format string
 */

#define _LOG_EMERG(__logger, ...)  { (__logger).emerg(__VA_ARGS__); }
#define _LOG_FATAL(__logger, ...)  { (__logger).fatal(__VA_ARGS__); }
#define _LOG_ALERT(__logger, ...)  { (__logger).alert(__VA_ARGS__); }
#define _LOG_CRIT(__logger, ...)  { (__logger).crit(__VA_ARGS__); }
#define _LOG_ERROR(__logger, ...)  { (__logger).error(__VA_ARGS__); }
#define _LOG_WARN(__logger, ...)  { (__logger).warn(__VA_ARGS__); }
#define _LOG_NOTICE(__logger, ...)  { (__logger).notice(__VA_ARGS__); }
#define _LOG_INFO(__logger, ...)  { (__logger).info(__VA_ARGS__); }
#define _LOG_DEBUG(__logger, ...)  { (__logger).debug(__VA_ARGS__); }

#define _LOG_ERROR_ERRNO(__logger, format, __errno, ...) { \
    char err_buf[1024]; \
    strerror_r((__errno), err_buf, sizeof(err_buf)); \
    (__logger)->error(format, err_buf, __VA_ARGS__); \
}

#define _LOG_FATAL_ERRNO(__logger, format, __errno, ...) { \
    char err_buf[1024]; \
    strerror_r((__errno), err_buf, sizeof(err_buf)); \
    (__logger)->fatal(format, err_buf, __VA_ARGS__); \
}
