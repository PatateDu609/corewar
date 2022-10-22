#define _GNU_SOURCE
#include <libft.h>
#include "asm/error.h"
#include <stdio.h>

static char *dump_ln_error(enum ln_error err)
{
	switch(err)
	{
	case LN_ERR_SUCCESS:
		return "Success";
	case LN_ERR_UNKNOWN_INSTRUCTION:
		return "Unkown instruction";
	case LN_ERR_UNKNOWN_HEADER:
		return "Unkown header";
	case LN_ERR_UNEXPECTED_REGISTER:
		return "Was not expecting a register";
	case LN_ERR_UNEXPECTED_DIRECT_PARAM:
		return "Was not expecting a direct parameter";
	case LN_ERR_UNEXPECTED_INDIRECT_PARAM:
		return "Was not expecting an indirect parameter";
	case LN_ERR_UNEXPECTED_VALUE:
		return "Was not expecting a value";
	case LN_ERR_UNEXPECTED_LABEL:
		return "Was not expecting a label char";
	case LN_ERR_UNEXPECTED_LABEL_DECL:
		return "Was not expecting a label declaration";
	case LN_ERR_UNEXPECTED_STRING:
		return "Was not expecting a string";
	case LN_ERR_UNEXPECTED_INSTRUCTION:
		return "Was not expecting an instruction";
	case LN_ERR_UNEXPECTED_HEADER:
		return "Was not expecting a header";
	case LN_ERR_INVALID_LABEL:
		return "Invalid label";
	case LN_ERR_CANT_COMPLETE_WORD:
		return "Can't determine word type";
	case LN_ERR_WRONG_NUMBER_ARGUMENTS:
		return "Wrong number of arguments given";
	case LN_ERR_UNEXPECTED_SEPARATOR_CHAR:
		return "Was not expecting a parameter separator";
	case LN_ERR_WRONG_ROOT_TYPE:
		return "Wrong root type";
	case LN_ERR_HDR_NO_LABEL_EXPECTED:
		return "No label is expected on header lines";
	case LN_ERR_EXPECTED_STRING:
		return "Expected string parameter";
	case LN_ERR_EXPECTED_DIRECT_PARAM:
		return "Expected direct parameter";
	case LN_ERR_EXPECTED_INDIRECT_PARAM:
		return "Expected indirect parameter";
	case LN_ERR_EXPECTED_REGISTER:
		return "Expected register parameter";
	default:
		return "Unkown error";
	}
}

void print_errors(struct line *ln)
{
	printf("\033[1;31mError:\033[0m \033[33mLine %zu\033[0m: %s\n", ln->ln_nb, ln->original);
	for (size_t i = 0; i < ln->nb_errors; i++)
	{
		dprintf(2, "\n - ");
		size_t len = ft_strlen(ln->errors[i].str);
		if (ln->errors[i].err == LN_ERR_STRAY_TOKEN)
		{
			char *str = len == 1 ? "character" : "token";
			dprintf(2, "Stray %1$s %3$c%2$s%3$c",
				str, ln->errors[i].str, len == 1 ? '\'' : '"');
		}
		else if (ln->errors[i].err == LN_ERR_UNEXPECTED_DIRECT_PARAM ||
				ln->errors[i].err == LN_ERR_UNEXPECTED_LABEL ||
				ln->errors[i].err == LN_ERR_UNEXPECTED_SEPARATOR_CHAR ||
				ln->errors[i].str == NULL)
			dprintf(2, "%s.", dump_ln_error(ln->errors[i].err));
		else
			dprintf(2, "\033[1;35m%s\033[0m: %s.", ln->errors[i].str, dump_ln_error(ln->errors[i].err));
		dprintf(2, "\n");
	}
}