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

#ifndef TOKEN_H
#define TOKEN_H

#include "erupt.h"

typedef enum {
    PLUS = 1,     /* + */
    PLUS_EQ,      /* += */
    MIN,          /* - */
    MIN_EQ,       /* -= */
    B_OR,         /* | */
    B_OR_EQ,      /* |= */
    B_XOR,        /* ^ */
    B_XOR_EQ,     /* ^= */
    B_AND,        /* & */
    B_AND_EQ,     /* &= */
    B_NOT,        /* ~ */
    L_SHIFT,      /* << */
    L_SHIFT_EQ,   /* <<= */
    R_SHIFT,      /* >> */
    R_SHIFT_EQ,   /* >>= */
    AND,          /* and */
    OR,           /* or */
    SLASH,        /* / */
    SLASH_EQ,     /* /= */
    STAR,         /* * */
    STAR_EQ,      /* *= */
    STAR_STAR,    /* ** */
    STAR_STAR_EQ, /* **= */
    MOD,          /* % */
    MOD_EQ,       /* %= */
    BANG,         /* ! */
    BANG_EQ,      /* != */
    LT,           /* < */
    LT_EQ,        /* <= */
    GT,           /* > */
    GT_EQ,        /* >= */
    EQ,           /* = */
    EQ_EQ,        /* == */
    PIPE,         /* |> */

    L_PAREN,    /* ( */
    R_PAREN,    /* ) */
    L_BRACKET,  /* [ */
    R_BRACKET,  /* ] */
    L_BRACE,    /* { */
    R_BRACE,    /* } */
    DOT,        /* . */
    SEMI_COLON, /* ; */
    COLON,      /* : */
    FAT_ARROW,  /* => */
    ARROW,      /* -> */
    COMMA,      /* , */

    INT,    /* 1234 */
    FLOAT,  /* 3.14 */
    STRING, /* "Hello world" */

    IDENT,    /* ident */
    IF,       /* if */
    UNLESS,   /* unless */
    ELSE,     /* else */
    MATCH,    /* match */
    MODULE,   /* module */
    RETURN,   /* return */
    USE,      /* use */
    INCLUDE,  /* include */

    ERROR,
    NEW_LINE,
    _EOF,

    TOKEN_COUNT
} token_type_t;

typedef struct token {
    token_type_t type;

    char *value;
    size_t line_n;

    size_t start;
    size_t end;

    struct token *prev;
    struct token *next;
} token_t;

token_t *new_token(token_type_t type, char *value, size_t start, size_t end,
                   size_t line_n);
void dump_tokens(token_t *tok);
const char *token_str(token_t *token);
void destroy_tokens(token_t *ts);

#endif /* !TOKEN_H */
