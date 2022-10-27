#define _GNU_SOURCE
#include <libft.h>
#include "asm/ast.h"

ast_t *ast_new_child(struct ast_node *parent, enum ast_node_type type)
{
	struct ast_node *node = ast_new(type);
	if (!parent)
		return node;

	node->parent = parent;
	parent->childs = realloc(parent->childs,
		(++parent->nb_childs) * sizeof *parent->childs);
	if (!parent->childs)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	parent->childs[parent->nb_childs - 1] = node;
	return node;
}

ast_t *ast_new(enum ast_node_type type)
{
	ast_t *ast = ft_calloc(1, sizeof *ast);

	if (!ast)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	ast->type = type;
	return ast;
}

ast_t *ast_new_from_tok(token_t *tok)
{
	ast_t *ast = ast_new(get_ast_type_from_tok(tok->type));

	ast->token = (ast->type == AST_TERMINAL_VALUE || ast->type == AST_TAG)
		? tok : NULL;
	return ast;
}
