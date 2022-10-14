#define _GNU_SOURCE
#include <libft.h>
#include "asm/ast.h"

ast_t *ast_new_child(enum ast_node_type type, struct ast_node *parent)
{
	struct ast_node *node = ast_new(type);
	if (!node)
		return NULL;
	node->parent = parent;
	return node;
}

ast_t *ast_new(enum ast_node_type type)
{
	ast_t *ast = ft_calloc(1, sizeof *ast);

	if (!ast)
		return NULL;
	ast->type = type;
	return ast;
}

static ast_t *ast_double_new(ast_t *ast, token_t *tok, enum ast_node_type root_node)
{
	ast->type = root_node;
	ast->nb_childs = 2;
	ast->childs = ft_calloc(ast->nb_childs, sizeof *ast->childs);
	if (!ast->childs)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	ast->childs[0] = ast_new_child(AST_TAG, ast);
	ast->childs[1] = ast_new_child(AST_PARAMS, ast);

	if (!ast->childs[0] || !ast->childs[1])
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	ast->childs[0]->token = tok;

	return ast;
}

ast_t *ast_new_from_tok(token_t *tok)
{
	ast_t *ast = ft_calloc(1, sizeof *ast);

	if (!ast)
		return NULL;
	if (tok->type == TOK_INSTRUCTION || tok->type == TOK_HEADER)
		return ast_double_new(ast, tok,
			tok->type == TOK_INSTRUCTION ? AST_INSTRUCTION : AST_HEADER);
	ast->type = get_ast_type_from_tok(tok->type);
	ast->token = ast->type == AST_TERMINAL_VALUE ? tok : NULL;
	return ast;
}
