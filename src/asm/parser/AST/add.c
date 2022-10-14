#define _GNU_SOURCE

#include "asm/ast.h"
#include "asm/parser.h"
#include <errno.h>
#include <stdio.h>

static bool node_can_be_added(ast_t *ast, struct ast_node *node)
{
	enum ast_node_type parent_type = ast->type;
	enum ast_node_type node_type = node->type;

	if (node_type == AST_TERMINAL_VALUE)
	{
		return parent_type == AST_LABEL ||
			parent_type == AST_PARAM_DIRECT ||
			parent_type == AST_PARAM_INDIRECT ||
			parent_type == AST_PARAM_REGISTER;
	}
	if (node_type == AST_PARAMS)
	{
		// Should never go in there, this is managed in upstream functions.
		return parent_type == AST_HEADER || parent_type == AST_INSTRUCTION;
	}
	if (parent_type == AST_PARAMS)
	{
		return node_type == AST_PARAM_DIRECT ||
			node_type == AST_PARAM_INDIRECT || // should never happen, it is managed in another function.
			node_type == AST_PARAM_REGISTER; // should never happen, it is managed in another function.
	}
	if (parent_type == AST_PARAM_DIRECT)
		return node_type == AST_LABEL;
	if (parent_type == AST_LINE)
	{
		return node_type == AST_HEADER ||
			node_type == AST_INSTRUCTION ||
			node_type == AST_LABEL;
	}

	return false;
}

static void __add(ast_t *ast, struct ast_node *node)
{
	ast->childs = realloc(ast->childs, (++ast->nb_childs) * sizeof *ast->childs);
	if (!ast->childs)
	{
		dprintf(2, "Error: realloc: %s.\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	node->parent = ast;
	ast->childs[ast->nb_childs - 1] = node;
}

static bool manage_params(ast_t *ast, struct ast_node *node)
{
	if (ast->type != AST_PARAMS)
		return false;
	if (node->type != AST_TERMINAL_VALUE && node->type != AST_LABEL)
		return false;
	if (ast->parent->type == AST_HEADER) // This should never segfault (parent must be defined for PARAMS nodes)
	{
		__add(ast, node);
		return true;
	}
	if (ast->parent->type == AST_INSTRUCTION)
	{
		enum ast_node_type new_type = AST_PARAM_INDIRECT;
		if (node->token && node->token->type == TOK_REGISTER)
			new_type = AST_PARAM_REGISTER;
		struct ast_node *n = ast_new(new_type);
		__add(n, node);
		__add(ast, n);
		return true;
	}
	return false;
}

bool ast_add_node(ast_t *ast, struct ast_node *node)
{
	if (!ast || !node)
		return false;
	if (manage_params(ast, node))
		return true;
	if (!node_can_be_added(ast, node))
		return false;
	__add(ast, node);
	return true;
}