#include "asm/parser.h"
#include "asm/ast.h"
#include "asm/error.h"
#include "common/op.h"
#include <libft.h>

static void basic_checks(struct line *ln, ast_t *ast)
{
	bool hdr_line = false;

	for (size_t i = 0; i < ast->nb_childs; i++)
	{
		if (ast->childs[i]->type == AST_HEADER)
			hdr_line = true;
	}
	if (hdr_line && ast->nb_childs > 1)
		ln_add_error(ln, LN_ERR_HDR_NO_LABEL_EXPECTED, NULL);
	if (!hdr_line) // These errors should never be triggered since they are managed by the AST construction
	{
		if (ast->nb_childs > 2)
			ln_add_error(ln, LN_ERR_WRONG_CONSTRUCTION, NULL);
		if (!(ast->nb_childs == 1 &&
			  ast->childs[0]->type == AST_INSTRUCTION) &&
			!(ast->nb_childs == 2 &&
			  ast->childs[0]->type == AST_LABEL &&
			  ast->childs[1]->type == AST_INSTRUCTION))
			ln_add_error(ln, LN_ERR_WRONG_CONSTRUCTION, NULL);
	}
}

static void check_construction(struct line *ln, ast_t *ast)
{
	if (ast->nb_childs != 2)
		ln_add_error(ln, LN_ERR_WRONG_CONSTRUCTION, NULL);
	if (ast->childs[0]->type != AST_TAG)
		ln_add_error(ln, LN_ERR_WRONG_CONSTRUCTION, NULL);
	if (ast->childs[1]->type != AST_PARAMS)
		ln_add_error(ln, LN_ERR_WRONG_CONSTRUCTION, NULL);
}

static void check_hdr(struct line *ln, char *hdr, ast_t *params)
{
	if (params->nb_childs != 1)
		ln_add_error(ln, LN_ERR_WRONG_NUMBER_ARGUMENTS, hdr);
	if (params->nb_childs)
	{
		if (params->childs[0]->token && params->childs[0]->token->type == TOK_STRING)
			ln_add_error(ln, LN_ERR_EXPECTED_STRING, params->childs[0]->token->value);
	}
}

static bool check_exact_parameters(t_param_type expected, enum ast_node_type actual)
{
	switch (expected)
	{
	case PARAM_DIRECT:
		return actual == AST_PARAM_DIRECT;
	case PARAM_INDIRECT:
		return actual == AST_PARAM_INDIRECT;
	case PARAM_REGISTER:
		return actual == AST_PARAM_REGISTER;
	default:
		return false;
	}
}

static bool check_param(t_param_type expected, enum ast_node_type actual)
{
	if (expected & PARAM_DIRECT && check_exact_parameters(PARAM_DIRECT, actual))
		return true;
	if (expected & PARAM_INDIRECT && check_exact_parameters(PARAM_INDIRECT, actual))
		return true;
	if (expected & PARAM_REGISTER && check_exact_parameters(PARAM_REGISTER, actual))
		return true;
	return false;
}

static enum ln_error get_error_from_param(enum ast_node_type type)
{
	switch(type)
	{
	case AST_PARAM_DIRECT:
		return LN_ERR_UNEXPECTED_DIRECT_PARAM;
	case AST_PARAM_INDIRECT:
		return LN_ERR_UNEXPECTED_INDIRECT_PARAM;
	case AST_PARAM_REGISTER:
		return LN_ERR_UNEXPECTED_REGISTER;
	default:
		return -1;
	}
}

static void check_instr(struct line *ln, char *tag, ast_t *params)
{
	t_op *op = get_op_by_name(tag);

	if (params->nb_childs != (size_t)op->nb_params)
	{
		ln_add_error(ln, LN_ERR_WRONG_NUMBER_ARGUMENTS, tag);
		return ;
	}
	for (int i = 0; i < op->nb_params; i++)
		if (!check_param(op->param_types[i], params->childs[i]->type))
			ln_add_error(ln, get_error_from_param(params->childs[i]->type), tag);
}

bool is_valid(struct line *ln)
{
	ast_t *ast = ln->ast;

	if (ast->type != AST_LINE)
		ln_add_error(ln, LN_ERR_WRONG_ROOT_TYPE, NULL);
	basic_checks(ln, ast);
	if (ln->nb_errors)
		return false;
	for (size_t i = 0; i < ast->nb_childs; i++)
	{
		ast_t *child = ast->childs[i];

		if (child->type == AST_INSTRUCTION || child->type == AST_HEADER)
		{
			check_construction(ln, child);
			if (ln->nb_errors)
				return false;
			char *tag = child->childs[0]->token->value;
			ast_t *params = child->childs[1];
			if (child->type == AST_INSTRUCTION)
				check_instr(ln, tag, params);
			else
				check_hdr(ln, tag, params);
		}
	}
	return !ln->nb_errors;
}