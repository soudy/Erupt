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

#include "ast.h"
#include "erupt.h"
#include "minunit/minunit.h"

MU_TEST(number)
{
    AstNode *node = create_int(50);

    mu_assert(node->type == TYPE_INT, "node->type should be TYPE_INT");
    mu_assert(node->int_num.v == 50, "int_num.v doesn't match");

    destroy_node(node);
}

MU_TEST(var)
{
    AstNode *node = create_var("i", true, create_int(256));

    mu_assert(strcmp(node->var.name, "i") == 0, "var.name doesn't match");
    mu_assert(node->var.mutable == true, "var.mutable doesn't match");
    mu_assert(node->var.v->type == TYPE_INT, "var.v->type doesn't match");

    destroy_node(node);
}

MU_TEST(function)
{
    AstNode *node = create_fn_proto("main", NULL);

    mu_assert(node->type == TYPE_PROTO, "node->type should be TYPE_PROTO");
    mu_assert(strcmp(node->prototype.name, "main") == 0,
              "proto.name doesn't match");

    AstNode *function = create_fn(node, NULL);

    mu_assert(function->type == TYPE_FN, "function->type should be TYPE_FN");
    mu_assert(function->fn.prototype == node, "fn.prototype doesn't match");

    destroy_node(function);
}

MU_TEST_SUITE(test_suite)
{
    MU_RUN_TEST(number);
    MU_RUN_TEST(var);
    MU_RUN_TEST(function);
}

int main(int argc, char *argv[])
{
    MU_RUN_SUITE(test_suite);
    MU_REPORT();

    return 0;
}
