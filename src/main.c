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

#include <getopt.h>
#include <sys/stat.h>

#include "erupt.h"
#include "parser.h"

static int get_options(int argc, char *argv[]);
static char *read_path(const char *path);
static char *read_file(const FILE *handler);

const char *USAGE =
    "Usage: erupt [options] file\n"
    "       -o, --output\n"
    "               set output path (default: main)\n"
    "       -v, --version\n"
    "               show version\n"
    "       -V, --verbose\n"
    "               verbose mode\n"
    "       -t, --tokens\n"
    "               show generated token stream\n"
    "       -a, --ast\n"
    "               show nodes of the generated AST\n"
    "       -h, --help\n"
    "               show this\n";

int main(int argc, char *argv[])
{
    char *source = NULL;

    if (get_options(argc, argv) != ERUPT_OK)
        return ERUPT_ERROR;

    if (!(optind < argc))
        erupt_fatal_error("an input file is required");

    if (strcmp(argv[optind], "-") == 0) {
        set_output_name("stdin");
        source = read_file(stdin);
    } else {
        set_output_name(argv[optind]);
        source = read_path(argv[optind]);
    }

    /* lexical analysis */
    Lexer *lexer = lex(TARGET_FILE, source);

    if (SHOW_TOKENS)
        dump_tokens(lexer->ts);

    free(source);

    if (lexer->failed) {
        destroy_lexer(lexer);
        erupt_fatal_error("lexer error(s) occured, stopping compilation.");
    }

    /* parsing */
    Parser *parser = parse(lexer);

    if (SHOW_AST)
        dump_node_list(parser->ast);

    destroy_lexer(lexer);
    destroy_ast(parser->ast);

    if (parser->failed)
        erupt_fatal_error("parser error(s) occured, stopping compilation.");

    return ERUPT_OK;
}

static int get_options(int argc, char *argv[])
{
    const struct option long_options[] = {
        { "output"  , required_argument , NULL , 'o' },
        { "version" , no_argument       , NULL , 'v' },
        { "verbose" , no_argument       , NULL , 'V' },
        { "tokens"  , no_argument       , NULL , 't' },
        { "ast"     , no_argument       , NULL , 'a' },
        { "help"    , no_argument       , NULL , 'h' },
        { 0         , 0                 , 0    , 0 }
    };
    int choice = 0;

    while (1) {
        int option_index = 0;

        choice = getopt_long(argc, argv, "o:vVtnh", long_options,
                             &option_index);

        if (choice == -1)
            break;

        switch (choice) {
        case 'v':
            fprintf(stderr, "Erupt %s\n", ERUPT_VERSION);
            return ERUPT_ERROR;
        case 'o':
            OUTPUT_NAME = optarg;
            break;
        case 'h':
            fputs(USAGE, stderr);
            return ERUPT_ERROR;
        case 'V':
            VERBOSE = true;
            break;
        case 't':
            SHOW_TOKENS = true;
            break;
        case 'a':
            SHOW_AST = true;
            break;
        default:
            fputs(USAGE, stderr);
            return ERUPT_ERROR;
        }
    }

    return ERUPT_OK;
}

static char *read_path(const char *path)
{
    struct stat s;
    FILE *handler = fopen(path, "r");

    stat(path, &s);

    if (!handler)
        erupt_fatal_error("couldn't read file '%s'", path);

    if (S_ISDIR(s.st_mode))
        erupt_fatal_error("'%s' is a directory", path);

    return read_file(handler);
}

static char *read_file(const FILE *handler)
{
    char *buffer = NULL;
    size_t read = 0;

    buffer = smalloc(MAX_FILE_SIZE);
    read = fread(buffer, sizeof(char), MAX_FILE_SIZE, handler);

    buffer[read] = '\0';

    fclose(handler);

    return buffer;
}
