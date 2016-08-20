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

#include "token.h"

token_t *new_token(token_type_t type, char *value, size_t start, size_t end,
                   size_t line_n)
{
    token_t *token = smalloc(sizeof(token_t));

    token->type = type;

    token->value = value;
    token->line_n = line_n;

    token->start = start;
    token->end = end;

    token->prev = NULL;
    token->next = NULL;

    return token;
}

const char *token_str(token_t *token)
{
    const char *token_names[] = {
        "", "+", "+=", "-", "-=", "|", "|=", "^", "^=", "&", "&=", "~",
        "<<", "<<=", ">>", ">>=", "and", "or", "/", "/=", "*", "*=", "**",
        "**=", "%",  "%=", "!",  "!=",  "<", "<=", ">", ">=", "=", "==", "|>",
        "(", ")", "[", "]", "{", "}", ".", ";", ":", "=>", "->", ",",
        "integer", "float", "string", "identifier", "if", "unless", "else",
        "match", "module", "return", "use", "include", "error", "new line",
        "eof"
    };

    return token->type < TOKEN_COUNT ? token_names[token->type] : "???";
}

void dump_tokens(token_t *tok)
{
    do {
        printf("(%16s): %s\n", token_str(tok), tok->value);
    } while ((tok = tok->next));
}

void destroy_tokens(token_t *ts)
{
    if (!ts)
        return;

    do {
        free(ts->value);

        destroy_tokens(ts->prev);
    } while ((ts = ts->next));

    free(ts);
}
