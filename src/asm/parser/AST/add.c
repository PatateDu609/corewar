#define _GNU_SOURCE

#include "asm/ast.h"
#include "asm/parser.h"
#include <errno.h>
#include <stdio.h>

typedef bool (manager_fn)(struct line *, ast_t **, lst_node_token_t **);
static manager_fn manage_label_node;
static manager_fn manage_hdr_instr;
static manager_fn manage_direct_char;
static manager_fn manage_register;
static manager_fn manage_label_char;
static manager_fn manage_terminal_value_node;
static manager_fn manage_separator_char;

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

static bool manage_label_node(struct line *ln, ast_t **ast, lst_node_token_t **node)
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
		if ((*ast)->nb_childs != 0)
		{
			ln_add_error(ln, LN_ERR_UNEXPECTED_LABEL_DECL, (*node)->data.value);
			return false;
		}
		*ast = ast_new_child(*ast, AST_LABEL);
		new_node = ast_new_child(*ast, AST_TERMINAL_VALUE);

		new_node->token = &(*node)->data;
		*node = (*node)->next;
		*ast = tmp; // no need to do further rewinding since this is AST_LINE case
		return true;
	}
	ln_add_error(ln, LN_ERR_UNEXPECTED_LABEL, (*node)->data.value);
	return false;
}

static bool manage_hdr_instr(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;
	bool hdr = n->data.type == TOK_HEADER;

	if ((*ast)->type != AST_LINE)
	{
		ln_add_error(ln, hdr ? LN_ERR_UNEXPECTED_HEADER : LN_ERR_UNEXPECTED_INSTRUCTION, n->data.value);
		return false;
	}
	*ast = ast_new_child(*ast, hdr ? AST_HEADER : AST_INSTRUCTION);
	*ast = ast_new_child(*ast, AST_TAG);
	(*ast)->token = &n->data;
	*ast = (*ast)->parent;

	*ast = ast_new_child(*ast, AST_PARAMS);
	return true;
}

static bool manage_direct_char(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	if ((*ast)->type != AST_PARAMS)
	{
		ln_add_error(ln, LN_ERR_UNEXPECTED_DIRECT_PARAM, (*node)->data.value);
		return false;
	}
	*ast = ast_new_child(*ast, AST_PARAM_DIRECT);
	return true;
}

static bool manage_register(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	if ((*ast)->type != AST_PARAMS)
	{
		ln_add_error(ln, LN_ERR_UNEXPECTED_REGISTER, (*node)->data.value);
		return false;
	}
	*ast = ast_new_child(*ast, AST_PARAM_REGISTER);
	*ast = ast_new_child(*ast, AST_TERMINAL_VALUE);
	(*ast)->token = &(*node)->data;
	return rewind_ast(ast, AST_PARAMS);
}

static bool manage_label_char(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	(void)node;
	if ((*ast)->type != AST_PARAMS && (*ast)->type != AST_PARAM_DIRECT)
	{
		ln_add_error(ln, LN_ERR_UNEXPECTED_LABEL, (*node)->data.value);
		return false;
	}
	if ((*ast)->type == AST_PARAMS)
		*ast = ast_new_child(*ast, AST_PARAM_INDIRECT);
	*ast = ast_new_child(*ast, AST_LABEL);
	return true;
}

static bool manage_terminal_value_node(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;

	if ((*ast)->type != AST_PARAMS && (*ast)->type != AST_PARAM_DIRECT)
	{
		ln_add_error(ln, LN_ERR_UNEXPECTED_VALUE, (*node)->data.value);
		return false;
	}
	if ((*ast)->type == AST_PARAMS && (*ast)->parent->type != AST_HEADER)
		*ast = ast_new_child(*ast, AST_PARAM_INDIRECT);
	*ast = ast_new_child(*ast, AST_TERMINAL_VALUE);
	(*ast)->token = &n->data;

	return rewind_ast(ast, AST_PARAMS);
}

static bool manage_separator_char(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	lst_node_token_t *n = *node;
	bool is_valid = true;

	if ((*ast)->type != AST_PARAMS)
		is_valid = false;
	if (is_valid && !n->prev)
		is_valid = false;
	if (is_valid)
	{
		enum token_type prev_type = n->prev->data.type;
		is_valid = prev_type == TOK_NUMBER
				|| prev_type == TOK_STRING
				|| prev_type == TOK_REGISTER
				|| (prev_type == TOK_LABEL &&
					n->prev->prev &&
					n->prev->prev->data.type == TOK_LABEL_CHAR
				);
	}

	if (!is_valid)
		ln_add_error(ln, LN_ERR_UNEXPECTED_SEPARATOR_CHAR, (*node)->data.value);
	return is_valid;
}

bool ast_add_token(struct line *ln, ast_t **ast, lst_node_token_t **node)
{
	if (!ast || !*ast || !node || !*node)
		return false;
	enum token_type type = (*node)->data.type;

	static manager_fn *manager[] = {
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
	return manager[type](ln, ast, node);
}