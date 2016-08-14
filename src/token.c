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

Token *new_token(TokenType type, char *value, size_t start, size_t end,
                 size_t line_n)
{
    Token *tok = smalloc(sizeof(Token));

    tok->type = type;

    tok->value = value;
    tok->line_n = line_n;

    tok->start = start;
    tok->end = end;

    tok->prev = NULL;
    tok->next = NULL;

    return tok;
}

char *token_to_string(TokenType type)
{
    const char *token_names[] = {
        "", "+", "+=", "-", "-=", "|", "|=", "^", "^=", "&", "&=", "~",
        "<<", "<<=", ">>", ">>=", "and", "or", "/", "/=", "*", "*=", "**",
        "**=", "%",  "%=", "!",  "!=",  "<", "<=", ">", ">=", "=", "==", "|>",
        "(", ")", "[", "]", "{", "}", ".", ";", ":", "=>", "->", ",",
        "integer", "float", "string", "identifier", "if", "unless", "else",
        "match", "module", "return", "error", "new line", "eof"
    };

    return type < TOKEN_COUNT ? token_names[type] : "???";
}

bool is(Token *tok, TokenType type)
{
    return tok->type == type;
}

void dump_tokens(Token *tok)
{
    do {
        printf("(%16s): %s\n", token_to_string(tok->type), tok->value);
    } while ((tok = tok->next));
}

void destroy_tokens(Token *ts)
{
    if (!ts)
        return;

    do {
        free(ts->value);

        destroy_tokens(ts->prev);
    } while ((ts = ts->next));

    free(ts);
}
