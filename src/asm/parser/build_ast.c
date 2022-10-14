#define _GNU_SOURCE
#include <stdio.h>
#include <libft.h>

#include "asm/parser.h"
#include "asm/ast.h"

bool build_ast(struct line *ln)
{
	struct ast_node *local_root = ln->ast = ast_new(AST_LINE);
	if (!ln->ast)
	{
		dprintf(2, "Error: Cannot create new AST.\n");
		exit(EXIT_FAILURE);
	}

	struct ast_node *saved = NULL, *params = NULL;
	bool label_line = false;
	bool labelled = false;
	for (lst_node_token_t *node = ln->tokens->start; node; node = node->next)
	{
		struct ast_node *n = ast_new_from_tok(&node->data);
		if (!n)
		{
			dprintf(2, "Error: Cannot create new AST node.\n");
			exit(EXIT_FAILURE);
		}
		if (n->type == AST_PARAMS)
		{
			if (params)
			{
				local_root = params; // Rewind to params...
				free(n);
				continue;
			}
			params = n;
		}

		if (saved && saved->type == AST_TERMINAL_VALUE && n->type == AST_LABEL)
			label_line = true;
		if (saved && !ast_add_node(n, saved))
		{
			dprintf(2, "BIG ERROR: Couldn't add saved node to new node.\n");
			return false;
		}
		else if (saved)
			saved = NULL;
		if (!ast_add_node(local_root, n))
			saved = n; // Trying next node... (should work in the case of label declaration)

		if (label_line && !labelled)
		{
			local_root = ln->ast;
			labelled = true;
			continue;
		}
		if (n->type == AST_HEADER || n->type == AST_INSTRUCTION)
			local_root = params = n->childs[1];
		if (n->type == AST_PARAM_DIRECT
			|| n->type == AST_PARAM_REGISTER
			|| n->type == AST_LABEL
			|| (n->type == AST_PARAM_INDIRECT && n->childs[0]->type == AST_LABEL))
			local_root = n; // Should only happen when going through parameters
	}
	return true;
}