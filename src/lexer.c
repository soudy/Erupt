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

#include "lexer.h"

static lexer_t *create_lexer(const char *target, const char *source);
static char current(lexer_t *l);
static char peek(lexer_t *l);
static char eat(lexer_t *l);
static void emit(lexer_t *l, token_type_t type);
static bool is_number(char c);
static bool is_ident(char c);
static bool is_whitespace(char c);
static void read_string(lexer_t *l, char id);
static void read_number(lexer_t *l);
static void read_ident(lexer_t *l);
static void read_comment(lexer_t *l);
static void switch_eq(lexer_t *l, token_type_t tok_a, token_type_t tok_b);
static char *current_token(lexer_t *l);

const Keyword keywords[] = {
    { "and"    , AND },
    { "or"     , OR },
    { "module" , MODULE },
    { "return" , RETURN },
    { "if"     , IF },
    { "match"  , MATCH },
    { "unless" , UNLESS },
    { "else"   , ELSE },
};

lexer_t *lex(const char *target, const char *source)
{
    char c;
    lexer_t *l = create_lexer(target, source);

    verbose_printf("starting lexical analysis");

    while (peek(l) != '\0') {
        l->start = l->pos;

        c = eat(l);

        switch (c) {
        case '+': switch_eq(l, PLUS, PLUS_EQ); break;
        case '/': switch_eq(l, SLASH, SLASH_EQ); break;
        case '^': switch_eq(l, B_XOR, B_XOR_EQ); break;
        case '&': switch_eq(l, B_AND, B_AND_EQ); break;
        case '!': switch_eq(l, BANG, BANG_EQ); break;
        case '~': emit(l, B_NOT); break;
        case '(': emit(l, L_PAREN); break;
        case ')': emit(l, R_PAREN); break;
        case '[': emit(l, L_BRACKET); break;
        case ']': emit(l, R_BRACKET); break;
        case '{': emit(l, L_BRACE); break;
        case '}': emit(l, R_BRACE); break;
        case '.': emit(l, DOT); break;
        case ':': emit(l, COLON); break;
        case ';': emit(l, SEMI_COLON); break;
        case ',': emit(l, COMMA); break;
        case '|':
            if (peek(l) == '>') {
                eat(l);
                emit(l, PIPE); /* |> */
                break;
            }
            switch_eq(l, B_OR, B_OR_EQ); /* | or |= */
            break;
        case '%':
            if (peek(l) == '%') {
                eat(l);
                read_comment(l);
                break;
            }
            switch_eq(l, MOD, MOD_EQ);
            break;
        case '=':
            if (peek(l) == '>') {
                eat(l);
                emit(l, FAT_ARROW); /* => */
                break;
            }
            switch_eq(l, EQ, EQ_EQ); /* = or == */
            break;
        case '-':
            if (peek(l) == '>') {
                eat(l);
                emit(l, ARROW); /* -> */
                break;
            }
            switch_eq(l, MIN, MIN_EQ); /* - or -= */
            break;
        case '<':
            if (peek(l) == '<') {
                eat(l);
                switch_eq(l, L_SHIFT, L_SHIFT_EQ); /* << or <<= */
                break;
            }
            switch_eq(l, LT, LT_EQ); /* < or <= */
            break;
        case '>':
            if (peek(l) == '>') {
                eat(l);
                switch_eq(l, R_SHIFT, R_SHIFT_EQ); /* >> or >>= */
                break;
            }
            switch_eq(l, GT, GT_EQ); /* > or >= */
            break;
        case '*':
            if (peek(l) == '*') {
                eat(l);
                switch_eq(l, STAR_STAR, STAR_STAR_EQ); /* ** or **= */
                break;
            }
            switch_eq(l, STAR, STAR_EQ); /* * or *= */
            break;
        case '\n':
            ++l->line_n;
            emit(l, NEW_LINE);
            break;
        case ' ':
        case '\t':
        case '\r':
            while (is_whitespace(peek(l)))
                eat(l);
            break;
        case '"':
        case '\'':
            read_string(l, c);
            break;
        default:
            if (is_ident(c)) {
                read_ident(l);
                break;
            }

            if (is_number(c)) {
                read_number(l);
                break;
            }

            emit(l, ERROR);
            file_error(l->target, l->line_n, "erroneous token '%c'", c);
            l->failed = true;
        }
    }

    l->start = l->pos;
    emit(l, _EOF);

    verbose_printf("lexical analysis done");

    return l;
}

static lexer_t *create_lexer(const char *target, const char *source)
{
    lexer_t *l = smalloc(sizeof(lexer_t));

    l->target = target;
    l->source = source;
    l->line_n = 1;
    l->start = 0;
    l->pos = 0;
    l->failed = false;
    l->ts = NULL;
    l->tail = NULL;

    return l;
}

static char current(lexer_t *l)
{
    return l->source[l->pos - 1];
}

static char peek(lexer_t *l)
{
    return l->source[l->pos];
}

static char eat(lexer_t *l)
{
    /* assign before advancing, avoiding fencepost errors */
    char c = peek(l);

    ++l->pos;

    return c;
}

static void emit(lexer_t *l, token_type_t type)
{
    char *value = current_token(l);
    token_t *token = new_token(type, value, l->start, l->pos, l->line_n);

    token->prev = NULL;
    token->next = NULL;

    if (l->tail == NULL)
        l->ts = token;
    else
        l->tail->next = token;

    l->tail = token;
}

static bool is_number(char c)
{
    return (c >= '0' && c <= '9') || c == '.';
}

static bool is_ident(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' ||
            c == '!' || c == '?';
}

static bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

static void read_string(lexer_t *l, char id)
{
    while (eat(l) != id) {
        /* allow escaped quotation chars in strings */
        if (current(l) == '\\')
            eat(l);

        if (current(l) == '\n')
            l->line_n++;

        if (peek(l) == EOF)
            file_fatal_error(l->target, l->line_n,
                             "unexpected EOF while scanning string");
    }

    emit(l, STRING);
}

static void read_number(lexer_t *l)
{
    token_type_t type = INT;

    while (is_number(peek(l))) {
        if (peek(l) == '.')
            type = FLOAT;

        eat(l);
    }

    emit(l, type);
}

static void read_ident(lexer_t *l)
{
    while (is_ident(peek(l)) || is_number(peek(l)))
        eat(l);

    /* the read ident might be a reserved keyword */
    const char *ident = current_token(l);
    size_t n_keywords = sizeof keywords / sizeof keywords[0];

    for (size_t i = 0; i < n_keywords; ++i) {
        if (strcmp(ident, keywords[i].name) == 0) {
            emit(l, keywords[i].type);
            return;
        }
    }

    emit(l, IDENT);
}

static void read_comment(lexer_t *l)
{
    while (1) {
        if (peek(l) == EOF)
            break;

        if (peek(l) == '\n') {
            eat(l);
            ++l->line_n;
            break;
        }

        eat(l);
    }
}

static void switch_eq(lexer_t *l, token_type_t tok_a, token_type_t tok_b)
{
    if (peek(l) == '=') {
        eat(l);
        emit(l, tok_b);
        return;
    }

    emit(l, tok_a);
}

static char *current_token(lexer_t *l)
{
    char *value = strndup(l->source + l->start, l->pos);

    value[l->pos - l->start] = '\0';

    return value;
}

void destroy_lexer(lexer_t *l)
{
    if (!l)
        return;

    if (l->ts)
        destroy_tokens(l->ts);

    free(l->tail);

    free(l);
    verbose_printf("destroyed lexer");
}
