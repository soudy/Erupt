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

    ERROR,
    NEW_LINE,
    _EOF,

    TOKEN_COUNT
} TokenType;

typedef struct token {
    TokenType type;

    char *value;
    size_t line_n;

    size_t start;
    size_t end;

    struct token *prev;
    struct token *next;
} Token;

#define NULL_TOKEN (Token) { ERROR, NULL, 0, 0, 0, NULL, NULL }

Token *new_token(TokenType type, const char *value, size_t start, size_t end,
                 size_t line_n);
void dump_tokens(Token *tok);
char *token_to_string(TokenType type);
bool is(Token *tok, TokenType type);
void destroy_tokens(Token *ts);

#endif /* !TOKEN_H */
