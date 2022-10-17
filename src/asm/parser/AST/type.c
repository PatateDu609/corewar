#include "asm/ast.h"
#include "common/op.h"
#include "asm/parser.h"
#include <libft.h>

enum ast_node_type get_ast_type_from_tok(enum token_type tok_type)
{
	switch (tok_type)
	{
	case TOK_SEPARATOR_CHAR:
		return AST_PARAMS;
	case TOK_LABEL_CHAR:
		return AST_LABEL;
	case TOK_DIRECT_CHAR:
		return AST_PARAM_DIRECT;
	case TOK_NUMBER:
	case TOK_STRING:
	case TOK_LABEL:
	case TOK_REGISTER:
		return AST_TERMINAL_VALUE;
	case TOK_WORD:
		return AST_INCOMPLETE; // Needs specification with next or previous node
	default:
		return AST_UNKNOWN;
	}
}

bool complete_node(ast_t *ast, lst_node_token_t *node)
{
	token_t *tok = &node->data;
	size_t len = ft_strlen(tok->value);
	if (tok->type != TOK_WORD)
		return false;

	switch (ast->type)
	{
		case AST_LINE: // Can be label only if token is followed by TOK_LABEL_CHAR
		{
			if (is_label(tok->value) && node->next && node->next->data.type == TOK_LABEL_CHAR)
				tok->type = TOK_LABEL; // No insertion is performed, it will be done in another function.
			else if (is_instruction(tok->value, len))
				tok->type = TOK_INSTRUCTION;
			else if (is_header(tok->value, len))
				tok->type = TOK_HEADER;
			break;
		}
		case AST_LABEL: // Is reached only in case of prefixed LABEL_CHAR
		{
			if (is_label(tok->value))
				tok->type = TOK_LABEL;
			break;
		}
		case AST_PARAMS:
		case AST_PARAM_DIRECT:
		{
			if (is_register(tok->value, len))
				tok->type = TOK_REGISTER;
			break;
		}
	default:
		tok->type = TOK_UNKNOWN;
	}
	return tok->type != TOK_UNKNOWN;
}