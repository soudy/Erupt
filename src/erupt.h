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

#ifndef ERUPT_H
#define ERUPT_H

#ifndef _GNU_SOURCE
# define _GNU_SOURCE /* strndup */
#endif

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERUPT_VERSION "0.0.4"
#define MAX_FILE_SIZE 512 * 512

#define ERUPT_OK 0
#define ERUPT_ERROR -1

#define erupt_error(...) error_printf("erupt", 0, ##__VA_ARGS__)
#define file_error(file, ...) error_printf(file, ##__VA_ARGS__)
#define erupt_fatal_error(...) fatal_error("erupt", 0, ##__VA_ARGS__)
#define file_fatal_error(file, ...) fatal_error(file, ##__VA_ARGS__)
#define erupt_warning(...) warning_printf("erupt", 0, ##__VA_ARGS__)
#define file_warning(file, ...) warning_printf(file, ##__VA_ARGS__)

/* no colors for windows */
#ifdef _WIN32
# define COLOR_RESET
# define BOLD
# define RED_TEXT
# define GREEN_TEXT
# define YELLOW_TEXT
#else
# define COLOR_RESET "\033[0m"
# define BOLD "\033[1m"
# define RED_TEXT "\033[31;1m"
# define GREEN_TEXT "\033[32;1m"
# define YELLOW_TEXT "\033[33;1m"
#endif

extern bool VERBOSE;
extern bool SHOW_TOKENS;
extern bool SHOW_AST;
extern char *OUTPUT_NAME;
extern char *TARGET_FILE;

void *smalloc(size_t size);
void *scalloc(size_t n, size_t size);
void set_output_name(const char *filename);
char *unquote(const char *s);
void verbose_printf(const char *fmt, ...);
void warning_printf(const char *m, size_t line, const char *fmt, ...);
void error_printf(const char *m, size_t line, const char *fmt, ...);
void fatal_error(const char *m, size_t line, const char *fmt, ...);

#endif /* !ERUPT_H */
