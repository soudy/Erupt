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

#ifndef AST_H
#define AST_H

#include <limits.h>
#include <stdint.h>

#include "lexer.h"

typedef struct ast_operator_t ast_operator_t;
typedef struct ast_int_t ast_int_t;
typedef struct ast_float_t ast_float_t;
typedef struct ast_string_t ast_string_t;
typedef struct ast_list_t ast_list_t;
typedef struct ast_var_t ast_var_t;
typedef struct ast_prototype_t ast_prototype_t;
typedef struct ast_struct_t ast_struct_t;
typedef struct ast_function_t ast_function_t;
typedef struct ast_call_t ast_call_t;
typedef struct ast_if_t ast_if_t;
typedef struct ast_expr_t ast_expr_t;
typedef struct ast_node_t ast_node_t;
typedef struct ast_node_list_t ast_node_list_t;
typedef struct ast_return_t ast_return_t;

typedef enum { ASSOC_LEFT, ASSOC_RIGHT, ASSOC_NONE } ast_assoc_t;

struct ast_operator_t {
    token_type_t symbol;
    int precedence;
    ast_assoc_t assoc;
    bool unary;
};

struct ast_node_t;
struct ast_node_list_t;

struct ast_int_t {
    int v;
};

struct ast_float_t {
    float v;
};

struct ast_string_t {
    char *v;
};

struct ast_list_t {
    ast_node_list_t *values;
};

struct ast_var_t {
    char *name;
    bool mutable;
    ast_node_t *v;
};

struct ast_prototype_t {
    char *name;
    ast_node_list_t *args;
};

struct ast_struct_t {
    char *name;
    ast_node_list_t *fields;
};

struct ast_function_t {
    ast_node_t *prototype;
    ast_node_list_t *body;
};

struct ast_call_t {
    char *name;
    ast_node_list_t *args;
};

struct ast_if_t {
    ast_node_t *condition;
    ast_node_list_t *true_body;
    ast_node_list_t *false_body;
};

struct ast_expr_t {
    ast_operator_t *operator;

    ast_node_t *lhs;
    ast_node_t *rhs;
};

struct ast_return_t {
    ast_node_t *expr;
};

struct ast_node_t {
    enum {
        TYPE_INT,
        TYPE_FLOAT,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_VAR,
        TYPE_PROTO,
        TYPE_STRUCT,
        TYPE_FN,
        TYPE_CALL,
        TYPE_IF,
        TYPE_EXPR,
        TYPE_RETURN
    } type;

    union {
        ast_int_t int_num;
        ast_float_t float_num;
        ast_string_t string;
        ast_list_t array;
        ast_var_t var;
        ast_prototype_t prototype;
        ast_struct_t struct_stmt;
        ast_function_t fn;
        ast_call_t call;
        ast_if_t if_expr;
        ast_expr_t expr;
        ast_return_t return_expr;
    };
};

struct ast_node_list_t {
    ast_node_t *node;
    ast_node_list_t *next;
};

ast_node_t *create_int(int v);
ast_node_t *create_float(float v);
ast_node_t *create_string(const char *v);
ast_node_t *create_array(ast_node_list_t *values);
ast_node_t *create_var(const char *name, bool mutable, ast_node_t *v);
ast_node_t *create_fn_proto(const char *name, ast_node_list_t *args);
ast_node_t *create_struct(const char *name, ast_node_list_t *fields);
ast_node_t *create_fn(ast_node_t *prototype, ast_node_list_t *body);
ast_node_t *create_call(const char *name, ast_node_list_t *args);
ast_node_t *create_if(ast_node_t *condition, ast_node_list_t *true_body,
                   ast_node_list_t *false_body);
ast_node_t *create_expr(ast_operator_t *operator, ast_node_t *lhs,
                        ast_node_t *rhs);
ast_node_t *create_return(ast_node_t *expr);
ast_node_list_t *create_node_list(void);
void swap_lists(ast_node_list_t *a, ast_node_list_t *b);
void destroy_node(ast_node_t *node);
void destroy_ast(ast_node_list_t *nl);
void dump_node_list(ast_node_list_t *nl);

#endif /* !AST_H */
