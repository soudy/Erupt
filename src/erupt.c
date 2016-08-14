/*
 * Copyright (C) 2015 soud
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "erupt.h"
#include <stdarg.h>

bool VERBOSE = false;
bool SHOW_TOKENS = false;
bool SHOW_AST = false;
char *TARGET_FILE = "unknown";
char *OUTPUT_NAME = "main";

void *smalloc(size_t size)
{
    void *chunk = malloc(size);

    if (chunk == NULL)
        erupt_fatal_error("failed to allocate memory");

    return chunk;
}

void *scalloc(size_t n, size_t size)
{
    void *chunk = calloc(n, size);

    if (chunk == NULL)
        erupt_fatal_error("failed to allocate memory");

    return chunk;
}

void set_output_name(char *filename)
{
    TARGET_FILE = filename;
    char *result = strdup(filename);

    /* get only the file name: tests/string_error.er -> string_error.er */
    if (strstr(result, "/"))
        for (char *p = strtok(result, "/"); p != NULL; p = strtok(NULL, "/"))
            result = p;

    /* remove extension (if it has one) for executable name */
    if (strstr(result, "."))
        OUTPUT_NAME = strtok(result, ".");
    else
        OUTPUT_NAME = result;
}

char *unquote(char *s)
{
    if (*s != '"')
        return s;

    char *unquoted = strdup(s);

    unquoted++;
    unquoted[strlen(unquoted) - 1] = 0;

    return unquoted;
}

void verbose_printf(const char *fmt, ...)
{
    if (!VERBOSE)
        return;

    va_list arg;
    va_start(arg, fmt);

    printf(BOLD "[*] " COLOR_RESET);
    vprintf(fmt, arg);

    va_end(arg);

    printf("\n");
}

void warning_printf(const char *m, size_t line, const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    if (line)
        fprintf(stderr, BOLD "%s:%zu:" COLOR_RESET " %s ", m, line,
                (BOLD YELLOW_TEXT "warning:" COLOR_RESET));
    else
        fprintf(stderr, BOLD "%s:" COLOR_RESET " %s ", m,
                (BOLD YELLOW_TEXT "warning:" COLOR_RESET));

    vfprintf(stderr, fmt, arg);

    va_end(arg);

    printf("\n");
}

void error_printf(const char *m, size_t line, const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    if (line)
        fprintf(stderr, BOLD "%s:%zu:" COLOR_RESET " %s ", m, line,
                (BOLD RED_TEXT "error:" COLOR_RESET));
    else
        fprintf(stderr, BOLD "%s:" COLOR_RESET " %s ", m,
                (BOLD RED_TEXT "error:" COLOR_RESET));

    vfprintf(stderr, fmt, arg);

    va_end(arg);

    printf("\n");
}

void fatal_error(const char *m, size_t line, const char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    if (line)
        fprintf(stderr, BOLD "%s:%zu:" COLOR_RESET " %s ", m, line,
                (BOLD RED_TEXT "fatal error:" COLOR_RESET));
    else
        fprintf(stderr, BOLD "%s:" COLOR_RESET " %s ", m,
                (BOLD RED_TEXT "fatal error:" COLOR_RESET));

    vfprintf(stderr, fmt, arg);

    va_end(arg);

    printf("\n");

    verbose_printf("error found, stopping compilation");

    exit(ERUPT_ERROR);
}
