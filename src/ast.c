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

AstNode *create_int(int v)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_INT;
    node->int_num.v = v;

    return node;
}

AstNode *create_float(float v)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_FLOAT;
    node->float_num.v = v;

    return node;
}

AstNode *create_string(const char *v)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_STRING;
    node->string.v = strdup(v);

    return node;
}

AstNode *create_array(AstNodeList *values)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_ARRAY;
    node->array.values = values;

    return node;
}

AstNode *create_var(const char *name, bool mutable, AstNode *v)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_VAR;
    node->var.name = strdup(name);
    node->var.mutable = mutable;
    node->var.v = v;

    return node;
}

AstNode *create_fn_proto(const char *name, AstNodeList *args)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_PROTO;
    node->prototype.name = strdup(name);
    node->prototype.args = args;

    return node;
}

AstNode *create_struct(const char *name, AstNodeList *fields)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_STRUCT;
    node->struct_stmt.name = strdup(name);
    node->struct_stmt.fields = fields;

    return node;
}

AstNode *create_fn(AstNode *prototype, AstNodeList *body)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_FN;
    node->fn.prototype = prototype;
    node->fn.body = body;

    return node;
}

AstNode *create_call(const char *name, AstNodeList *args)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_CALL;
    node->call.name = strdup(name);

    node->call.args = args;

    return node;
}

AstNode *create_if(AstNode *condition, AstNodeList *true_body,
                   AstNodeList *false_body)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_IF;
    node->if_expr.condition = condition;
    node->if_expr.true_body = true_body;
    node->if_expr.false_body = false_body;

    return node;
}

AstNode *create_expr(AstOperator *operator, AstNode *lhs, AstNode *rhs)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_EXPR;
    node->expr.operator= operator;
    node->expr.lhs = lhs;
    node->expr.rhs = rhs;

    return node;
}

AstNode *create_return(AstNode *expr)
{
    AstNode *node = smalloc(sizeof(AstNode));

    node->type = TYPE_RETURN;
    node->return_expr.expr = expr;

    return node;
}

AstNodeList *create_node_list(void)
{
    AstNodeList *nl = smalloc(sizeof(AstNodeList));

    nl->node = NULL;
    nl->next = NULL;

    return nl;
}

void swap_lists(AstNodeList *a, AstNodeList *b)
{
    AstNodeList *tmp = a;
    a = b;
    b = tmp;
}

void destroy_node(AstNode *node)
{
    if (!node)
        return;

    switch (node->type) {
    case TYPE_INT:
    case TYPE_FLOAT: break;
    case TYPE_STRING:
        free(node->string.v);
        break;
    case TYPE_ARRAY:
        if (node->array.values)
            destroy_ast(node->array.values);
        break;
    case TYPE_VAR:
        free(node->var.name);

        if (node->var.v)
            destroy_node(node->var.v);
        break;
    case TYPE_PROTO:
        free(node->prototype.name);

        if (node->prototype.args)
            destroy_ast(node->prototype.args);
        break;
    case TYPE_STRUCT:
        free(node->struct_stmt.name);

        if (node->struct_stmt.fields)
            destroy_ast(node->struct_stmt.fields);
        break;
    case TYPE_FN:
        if (node->fn.prototype)
            destroy_node(node->fn.prototype);

        if (node->fn.body)
            destroy_ast(node->fn.body);
        break;
    case TYPE_CALL:
        free(node->call.name);

        if (node->call.args)
            destroy_ast(node->call.args);
        break;
    case TYPE_IF:
        if (node->if_expr.condition)
            destroy_node(node->if_expr.condition);

        if (node->if_expr.true_body)
            destroy_ast(node->if_expr.true_body);

        if (node->if_expr.false_body)
            destroy_ast(node->if_expr.false_body);
        break;
    case TYPE_EXPR:
        if (node->expr.lhs)
            destroy_node(node->expr.lhs);
        if (node->expr.rhs)
            destroy_node(node->expr.rhs);
        break;
    case TYPE_RETURN:
        if (node->return_expr.expr)
            destroy_node(node->return_expr.expr);
        break;
    }

    free(node);
}

void destroy_ast(AstNodeList *nl)
{
    if (!nl)
        return;

    do {
        destroy_node(nl->node);
    } while ((nl = nl->next));

    free(nl);
}

static void dump_node(AstNode *node)
{
    if (!node)
        return;

    switch (node->type) {
    case TYPE_INT: printf("int:\n\tvalue: %d\n", node->int_num.v); break;
    case TYPE_FLOAT: printf("float:\n\tvalue: %f\n", node->float_num.v); break;
    case TYPE_STRING: printf("string:\n\tvalue: %s\n", node->string.v); break;
    case TYPE_ARRAY:
        printf("array:\n\t");
        dump_node_list(node->array.values);
        break;
    case TYPE_VAR:
        printf("variable:\n\tname: %s\n\tmutable:%d\n",
               node->var.name, node->var.mutable);
        break;
    case TYPE_PROTO:
        printf("proto:\n\tname: %s\n", node->prototype.name);
        break;
    case TYPE_STRUCT:
        /* TODO */
        break;
    case TYPE_FN:
        printf("function:\n");

        if (node->fn.prototype)
            dump_node(node->fn.prototype);

        if (node->fn.body) {
            printf("body:\n");
            dump_node_list(node->fn.body);
        }
        break;
    case TYPE_CALL:
        /* TODO */
        break;
    case TYPE_IF:
        /* TODO */
        break;
    case TYPE_EXPR:
        /* TODO */
        break;
    case TYPE_RETURN:
        /* TODO */
        break;
    }
}

void dump_node_list(AstNodeList *nl)
{
    do {
        dump_node(nl->node);
    } while ((nl = nl->next));
}
