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
#include "erupt.h"
#include "minunit/minunit.h"
#include "token.h"

MU_TEST(tokens)
{
    Lexer *lexer = lex(
        "test",
        "+ += - -= | |= ^ ^= & &= ~ << <<= >> >>= and or / /= * *= ** **= % %= "
        "! != < <= > >= = == |> () [] {} . ; : => -> , 1000 3.14 \"string\" 's'"
        "identifier if unless else match module return"
    );

    TokenType expected[] = {
        PLUS, PLUS_EQ, MIN, MIN_EQ, B_OR, B_OR_EQ, B_XOR, B_XOR_EQ, B_AND,
        B_AND_EQ, B_NOT, L_SHIFT, L_SHIFT_EQ, R_SHIFT, R_SHIFT_EQ, AND, OR,
        SLASH, SLASH_EQ, STAR, STAR_EQ, STAR_STAR, STAR_STAR_EQ, MOD,
        MOD_EQ, BANG, BANG_EQ, LT, LT_EQ, GT, GT_EQ, EQ, EQ_EQ, PIPE, L_PAREN,
        R_PAREN, L_BRACKET,  R_BRACKET, L_BRACE, R_BRACE, DOT, SEMI_COLON,
        COLON, FAT_ARROW, ARROW, COMMA, INT, FLOAT, STRING, STRING, IDENT, IF,
        UNLESS, ELSE, MATCH, MODULE, RETURN, _EOF
    };

    for (int i = 0; !is(lexer->ts, _EOF); i++) {
        mu_assert(is(lexer->ts, expected[i]), "mismatched token");
        lexer->ts = lexer->ts->next;
    }

    destroy_lexer(lexer);
}

MU_TEST(comments)
{
    Lexer *lexer = lex("test", "%% I should be ignored!");
    mu_assert(is(lexer->ts, _EOF), "comment gets tokenized");

    destroy_lexer(lexer);
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(tokens);
    MU_RUN_TEST(comments);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return 0;
}
