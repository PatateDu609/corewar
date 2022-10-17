#define _GNU_SOURCE

#include "asm/ast.h"
#include "asm/parser.h"
#include <errno.h>
#include <stdio.h>

static bool rewind_ast(ast_t **ast, enum ast_node_type wanted_type)
{
	while ((*ast)->type != wanted_type)
	{
		*ast = (*ast)->parent;
		if (!*ast)
			return false;
	}
	return true;
}

static bool manage_label_node(ast_t **ast, lst_node_token_t **node)
{
	struct ast_node *tmp = *ast;
	struct ast_node *new_node;

	if ((*ast)->type == AST_LABEL) // prefixed LABEL_CHAR (only in case of PARAMS ast node)
	{
		new_node = ast_new_child(*ast, AST_TERMINAL_VALUE);

		new_node->token = &(*node)->data;
		return rewind_ast(ast, AST_PARAMS);
	}
	if ((*ast)->type == AST_LINE && (*node)->next && (*node)->next->data.type == TOK_LABEL_CHAR)
	{
		*ast = ast_new_child(*ast, AST_LABEL);
		new_node = ast_new_child(*ast, AST_TERMINAL_VALUE);

		new_node->token = &(*node)->data;
		*node = (*node)->next;
		*ast = tmp; // no need to do further rewinding since this is AST_LINE case
		return true;
	}
	return false;
}

bool manage_hdr_instr(ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;

	if ((*ast)->type != AST_LINE)
		return false;
	*ast = ast_new_child(*ast, n->data.type == TOK_HEADER ? AST_HEADER : AST_INSTRUCTION);
	*ast = ast_new_child(*ast, AST_TAG);
	(*ast)->token = &n->data;
	*ast = (*ast)->parent;

	*ast = ast_new_child(*ast, AST_PARAMS);
	return true;
}

bool manage_direct_char(ast_t **ast, lst_node_token_t **node)
{
	(void)node;
	if ((*ast)->type != AST_PARAMS)
		return false;
	*ast = ast_new_child(*ast, AST_PARAM_DIRECT);
	return true;
}

bool manage_register(ast_t **ast, lst_node_token_t **node)
{
	if ((*ast)->type != AST_PARAMS)
		return false;
	*ast = ast_new_child(*ast, AST_PARAM_REGISTER);
	*ast = ast_new_child(*ast, AST_TERMINAL_VALUE);
	(*ast)->token = &(*node)->data;
	return rewind_ast(ast, AST_PARAMS);
}

bool manage_label_char(ast_t **ast, lst_node_token_t **node)
{
	(void)node;
	if ((*ast)->type != AST_PARAMS && (*ast)->type != AST_PARAM_DIRECT)
		return false;
	if ((*ast)->type == AST_PARAMS)
		*ast = ast_new_child(*ast, AST_PARAM_INDIRECT);
	*ast = ast_new_child(*ast, AST_LABEL);
	return true;
}

bool manage_terminal_value_node(ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;

	if ((*ast)->type != AST_PARAMS && (*ast)->type != AST_PARAM_DIRECT && (*ast)->type != AST_HEADER)
		return false;
	if ((*ast)->type == AST_PARAMS && (*ast)->parent->type != AST_HEADER)
		*ast = ast_new_child(*ast, AST_PARAM_INDIRECT);
	*ast = ast_new_child(*ast, AST_TERMINAL_VALUE);
	(*ast)->token = &n->data;

	return rewind_ast(ast, AST_PARAMS);
}

bool manage_separator_char(ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;

	return (*ast)->type == AST_PARAMS && n->prev && n->prev->data.type != TOK_SEPARATOR_CHAR;
}

typedef bool (*manager_fn)(ast_t **, lst_node_token_t **);

bool ast_add_token(ast_t **ast, lst_node_token_t **node)
{
	if (!ast || !*ast || !node || !*node)
		return false;
	enum token_type type = (*node)->data.type;

	static manager_fn manager[] = {
		[TOK_HEADER] = manage_hdr_instr,
		[TOK_INSTRUCTION] = manage_hdr_instr,

		[TOK_SEPARATOR_CHAR] = manage_separator_char,
		[TOK_DIRECT_CHAR] = manage_direct_char,

		[TOK_LABEL_CHAR] = manage_label_char,
		[TOK_LABEL] = manage_label_node,

		[TOK_REGISTER] = manage_register,
		[TOK_NUMBER] = manage_terminal_value_node,
		[TOK_STRING] = manage_terminal_value_node,
	};

	if (type < 0 || (size_t)type >= (sizeof manager / sizeof *manager) || !manager[type])
		return false;
	return manager[type](ast, node);
}