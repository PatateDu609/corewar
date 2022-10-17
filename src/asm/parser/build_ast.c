#define _GNU_SOURCE
#include <stdio.h>
#include <libft.h>

#include "asm/parser.h"
#include "asm/ast.h"

bool build_ast(struct line *ln)
{
	struct ast_node *local_root = ln->ast = ast_new(AST_LINE);

	for (lst_node_token_t *node = ln->tokens->start; node; node = node->next)
	{
		enum ast_node_type ast_type = get_ast_type_from_tok(node->data.type);
		if (ast_type == AST_INCOMPLETE && !complete_node(local_root, node))
			return false;
		if (!ast_add_token(&local_root, &node))
			return false;
	}
	return true;
}