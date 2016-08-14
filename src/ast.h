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

typedef struct AstOperator AstOperator;
typedef struct AstInt AstInt;
typedef struct AstFloat AstFloat;
typedef struct AstString AstString;
typedef struct AstArray AstArray;
typedef struct AstVar AstVar;
typedef struct AstPrototype AstPrototype;
typedef struct AstStruct AstStruct;
typedef struct AstFunc AstFunc;
typedef struct AstCall AstCall;
typedef struct AstIf AstIf;
typedef struct AstExpr AstExpr;
typedef struct AstNode AstNode;
typedef struct AstNodeList AstNodeList;
typedef struct AstReturn AstReturn;

typedef enum { ASSOC_LEFT, ASSOC_RIGHT, ASSOC_NONE } AstAssoc;

struct AstOperator {
    TokenType symbol;
    int precedence;
    AstAssoc assoc;
    bool unary;
};

struct AstNode;
struct AstNodeList;

struct AstInt {
    int v;
};

struct AstFloat {
    float v;
};

struct AstString {
    char *v;
};

struct AstArray {
    AstNodeList *values;
};

struct AstVar {
    char *name;
    bool mutable;
    AstNode *v;
};

struct AstPrototype {
    char *name;
    AstNodeList *args;
};

struct AstStruct {
    char *name;
    AstNodeList *fields;
};

struct AstFunc {
    AstNode *prototype;
    AstNodeList *body;
};

struct AstCall {
    char *name;
    AstNodeList *args;
};

struct AstIf {
    AstNode *condition;
    AstNodeList *true_body;
    AstNodeList *false_body;
};

struct AstExpr {
    AstOperator *operator;

    AstNode *lhs;
    AstNode *rhs;
};

struct AstReturn {
    AstNode *expr;
};

struct AstNode {
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
        AstInt int_num;
        AstFloat float_num;
        AstString string;
        AstArray array;
        AstVar var;
        AstPrototype prototype;
        AstStruct struct_stmt;
        AstFunc fn;
        AstCall call;
        AstIf if_expr;
        AstExpr expr;
        AstReturn return_expr;
    };
};

struct AstNodeList {
    AstNode *node;
    AstNodeList *next;
};

AstNode *create_int(int v);
AstNode *create_float(float v);
AstNode *create_string(const char *v);
AstNode *create_array(AstNodeList *values);
AstNode *create_var(const char *name, bool mutable, AstNode *v);
AstNode *create_fn_proto(const char *name, AstNodeList *args);
AstNode *create_struct(const char *name, AstNodeList *fields);
AstNode *create_fn(AstNode *prototype, AstNodeList *body);
AstNode *create_call(const char *name, AstNodeList *args);
AstNode *create_if(AstNode *condition, AstNodeList *true_body,
                   AstNodeList *false_body);
AstNode *create_expr(AstOperator *operator, AstNode *lhs, AstNode *rhs);
AstNode *create_return(AstNode *expr);
AstNodeList *create_node_list(void);
void swap_lists(AstNodeList *a, AstNodeList *b);
void destroy_node(AstNode *node);
void destroy_ast(AstNodeList *nl);
void dump_node_list(AstNodeList *nl);

#endif /* !AST_H */
