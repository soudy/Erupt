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

#include "parser.h"
#include "erupt.h"

static parser_t *create_parser(const char *target, token_t *tokens);
static ast_node_t *parse_top_level(parser_t *p);
static ast_node_t *parse_function(parser_t *p);
static ast_node_t *parse_expression(parser_t *p);
static ast_node_t *parse_if(parser_t *p);
static ast_node_t *parse_import(parser_t *p);
static token_t *peek(parser_t *p);
static token_t *eat(parser_t *p);
static bool is(parser_t *p, token_type_t type);

parser_t *parse(lexer_t *l)
{
    parser_t *p = create_parser(l->target, l->ts);
    ast_node_t *node = NULL;

    verbose_printf("generating abstract syntax tree");

    do {
        node = parse_top_level(p);

        eat(p);
    } while (!is(p, _EOF));

    return p;
}

/*
 *   function
 * | expression
 * | import
 * | if
 */
static ast_node_t *parse_top_level(parser_t *p)
{
    switch (p->token->type) {
    case _EOF:
    case NEW_LINE:
        return NULL;
    case IDENT:
        if (peek(p)->type == IDENT) {
            return parse_function(p);
        }

        return parse_expression(p);
    case IF:
        return parse_if(p);
    case USE:
    case INCLUDE:
        return parse_import(p);
    default:
        parser_file_error(p, "unexpected %s", token_str(p->token));
        return NULL;
    }
}

/*
 * id args? '=>' body
 */
static ast_node_t *parse_function(parser_t *p)
{
    return NULL;
}

static ast_node_t *parse_expression(parser_t *p)
{
    return NULL;
}

/*
 * ('if' | 'unless') expression block
 * ('elsif' block)*
 * ('else' block)?
 */
static ast_node_t *parse_if(parser_t *p)
{
    return NULL;
}

/*
 *   'use' id
 * | 'include' id
 */
static ast_node_t *parse_import(parser_t *p)
{
    return NULL;
}

static token_t *peek(parser_t *p)
{
    return p->token->next;
}

static token_t *eat(parser_t *p)
{
    p->token = p->token->next;
    return p->token;
}

static bool is(parser_t *p, token_type_t type)
{
    return p->token->type == type;
}

static parser_t *create_parser(const char *target, token_t *tokens)
{
    parser_t *p = smalloc(sizeof(parser_t));

    p->target = strdup(target);
    p->ast = create_node_list();
    p->token = tokens;

    p->failed = false;

    return p;
}

void destroy_parser(parser_t *p)
{
    if (!p)
        return;

    verbose_printf("destroying parser");

    destroy_ast(p->ast);

    free(p->target);

    free(p);

    verbose_printf("destroyed parser");
}
