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

ast_node_t *create_int(int v)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_INT;
    node->int_num.v = v;

    return node;
}

ast_node_t *create_float(float v)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_FLOAT;
    node->float_num.v = v;

    return node;
}

ast_node_t *create_string(const char *v)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_STRING;
    node->string.v = strdup(v);

    return node;
}

ast_node_t *create_array(ast_node_list_t *values)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_ARRAY;
    node->array.values = values;

    return node;
}

ast_node_t *create_var(const char *name, bool mutable, ast_node_t *v)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_VAR;
    node->var.name = strdup(name);
    node->var.mutable = mutable;
    node->var.v = v;

    return node;
}

ast_node_t *create_fn_proto(const char *name, ast_node_list_t *args)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_PROTO;
    node->prototype.name = strdup(name);
    node->prototype.args = args;

    return node;
}

ast_node_t *create_struct(const char *name, ast_node_list_t *fields)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_STRUCT;
    node->struct_stmt.name = strdup(name);
    node->struct_stmt.fields = fields;

    return node;
}

ast_node_t *create_fn(ast_node_t *prototype, ast_node_list_t *body)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_FN;
    node->fn.prototype = prototype;
    node->fn.body = body;

    return node;
}

ast_node_t *create_call(const char *name, ast_node_list_t *args)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_CALL;
    node->call.name = strdup(name);

    node->call.args = args;

    return node;
}

ast_node_t *create_if(ast_node_t *condition, ast_node_list_t *true_body,
                   ast_node_list_t *false_body)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_IF;
    node->if_expr.condition = condition;
    node->if_expr.true_body = true_body;
    node->if_expr.false_body = false_body;

    return node;
}

ast_node_t *create_expr(ast_operator_t *operator, ast_node_t *lhs, ast_node_t *rhs)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_EXPR;
    node->expr.operator= operator;
    node->expr.lhs = lhs;
    node->expr.rhs = rhs;

    return node;
}

ast_node_t *create_return(ast_node_t *expr)
{
    ast_node_t *node = smalloc(sizeof(ast_node_t));

    node->type = TYPE_RETURN;
    node->return_expr.expr = expr;

    return node;
}

ast_node_list_t *create_node_list(void)
{
    ast_node_list_t *nl = smalloc(sizeof(ast_node_list_t));

    nl->node = NULL;
    nl->next = NULL;

    return nl;
}

void swap_lists(ast_node_list_t *a, ast_node_list_t *b)
{
    ast_node_list_t *tmp = a;
    a = b;
    b = tmp;
}

void destroy_node(ast_node_t *node)
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

void destroy_ast(ast_node_list_t *nl)
{
    if (!nl)
        return;

    do {
        destroy_node(nl->node);
    } while ((nl = nl->next));

    free(nl);
}

static void dump_node(ast_node_t *node)
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

void dump_node_list(ast_node_list_t *nl)
{
    do {
        dump_node(nl->node);
    } while ((nl = nl->next));
}
