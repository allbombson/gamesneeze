#include "../includes.hpp"

/* print Logs in green */
void Log::log(logLevel level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    switch (level) {
        case LOG: fputs("\x1b[32m[LOG] ", stdout); break;
        case WARN: fputs("\x1b[33m[WARN] ", stdout); break;
        case ERR: fputs("\x1b[31m[ERR] ", stdout); break;
    }
    vprintf(fmt, args);
    fputs("\x1b[0m\n", stdout);
    va_end(args);
}