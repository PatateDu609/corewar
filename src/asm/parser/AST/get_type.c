#include "asm/ast.h"
#include "common/op.h"
#include "asm/parser.h"

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
	case TOK_REGISTER:
	case TOK_NUMBER:
	case TOK_STRING:
	case TOK_WORD:
		return AST_TERMINAL_VALUE;
	default:
		return AST_UNKNOWN;
	}
}