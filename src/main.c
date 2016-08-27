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

static int eval(const char *path, char *source);
static char *generate_output_name(const char *filename);
static int get_options(int argc, char *argv[]);
static char *read_path(const char *path);
static char *read_file(FILE *handler);

bool SHOW_TOKENS = false;
bool SHOW_AST = false;
char *OUTPUT_NAME;

void usage()
{
    fputs(
        "Usage: erupt [options] file\n"
        "       -o, --output\n"
        "               set output path (default: main)\n"
        "       -v, --version\n"
        "               show version\n"
        "       -V, --verbose\n"
        "               verbose mode\n"
        "       -T, --tokens\n"
        "               show generated tokens\n"
        "       -A, --ast\n"
        "               show nodes of the generated AST\n"
        "       -h, --help\n"
        "               show this\n",
        stderr
    );
    exit(ERUPT_ERROR);
}

int main(int argc, char *argv[])
{
    char *source = NULL;
    const char *target = NULL;

    if (get_options(argc, argv) != ERUPT_OK)
        return ERUPT_ERROR;

    if (!(optind < argc)) {
        erupt_fatal_error("an input file is required");
        return ERUPT_ERROR;
    }

    /* if given target file is '-' read from stdin */
    if (strcmp(argv[optind], "-") == 0) {
        target = "stdin";
        source = read_file(stdin);
    } else {
        target = argv[optind];
        source = read_path(argv[optind]);
    }

    /*
     * if no output name is given, make the output name the name of the file
     * without extension and folder structure.
     */
    if (!OUTPUT_NAME)
        OUTPUT_NAME = generate_output_name(target);

    /* finally, evaluate the file */
    return eval(target, source);
}

static int eval(const char *path, char *source)
{
    /* lexical analysis */
    lexer_t *lexer = lex(path, source);

    if (SHOW_TOKENS)
        dump_tokens(lexer->tokens);

    free(source);

    if (lexer->failed) {
        destroy_lexer(lexer);
        erupt_fatal_error("lexer error(s) occured, stopping compilation.");

        return ERUPT_LEX_ERROR;
    }

    /* parsing */
    parser_t *parser = parse(lexer);

    if (SHOW_AST)
        dump_node_list(parser->ast);

    if (parser->failed) {
        destroy_parser(parser);
        destroy_lexer(lexer);
        erupt_fatal_error("parser error(s) occured, stopping compilation.");

        return ERUPT_PARSER_ERROR;
    }

    destroy_parser(parser);
    destroy_lexer(lexer);

    return ERUPT_OK;
}

static char *generate_output_name(const char *filename)
{
    char *result = strdup(filename);

    /* get only the file name: tests/string_error.er -> string_error.er */
    if (strstr(result, "/")) {
        for (char *p = strtok(result, "/"); p != NULL; p = strtok(NULL, "/"))
            result = p;
    }

    /* remove extension if it has one */
    return strstr(result, ".") ? strtok(result, ".") : result;
}

static int get_options(int argc, char *argv[])
{
    struct option long_options[] = {
        { "output"  , required_argument , NULL , 'o' },
        { "version" , no_argument       , NULL , 'v' },
        { "verbose" , no_argument       , NULL , 'V' },
        { "tokens"  , no_argument       , NULL , 'T' },
        { "ast"     , no_argument       , NULL , 'A' },
        { "help"    , no_argument       , NULL , 'h' },
        { 0         , 0                 , 0    , 0 }
    };
    int choice = 0;

    while (1) {
        int option_index = 0;

        choice = getopt_long(argc, argv, "o:vVTAh", long_options,
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
            usage();
            break;
        case 'V':
            VERBOSE = true;
            break;
        case 'T':
            SHOW_TOKENS = true;
            break;
        case 'A':
            SHOW_AST = true;
            break;
        default:
            usage();
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

static char *read_file(FILE *handler)
{
    char *buffer = smalloc(MAX_FILE_SIZE);
    size_t read = 0;

    read = fread(buffer, sizeof(char), MAX_FILE_SIZE, handler);

    buffer[read] = '\0';

    fclose(handler);

    return buffer;
}
