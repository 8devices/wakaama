/*
 * MIT License
 *
 * Copyright (c) 2017 8devices
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <assert.h>
#include <stdarg.h>

#include "logging.h"

int logging_init(unsigned char logging_level)
{
    if (logging_level > LOG_LEVEL_TRACE) {
        log_message(LOG_LEVEL_WARN, "Failed to initialise log level \"%c\".\n", logging_level + '0');
        log_message(LOG_LEVEL_WARN, "Log level must be a number:\n");
        log_message(LOG_LEVEL_WARN, "0 - FATAL\n1 - ERROR\n2 - WARN\n3 - INFO\n4 - DEBUG\n5 - TRACE\n");

        return -1;
    };

    current_level = logging_level;
    log_message(LOG_LEVEL_INFO, "Logging level set to %hhu\n", logging_level);

    return 0;
}

int log_message(unsigned char logging_level, char* format, ...)
{
    va_list arg_ptr;
    va_start(arg_ptr, format);

    if (logging_level <= current_level)
    {
        if (logging_level <= LOG_LEVEL_ERROR)
        {
            vfprintf(stderr, format, arg_ptr);
        }
        else
        {
            vfprintf(stdout, format, arg_ptr);
        }
        return 0;
    }

    va_end(arg_ptr);

    return -1;
}
