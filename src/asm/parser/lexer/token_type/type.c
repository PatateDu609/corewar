#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

enum token_type get_token_type(char *val, struct line *ln)
{
	size_t len = ft_strlen(val);
	enum token_type type = TOK_UNKNOWN;

	if (len == 1)
	{
		switch(val[0])
		{
		case SEPARATOR_CHAR:
			type = TOK_SEPARATOR_CHAR;
			break;
		case LABEL_CHAR:
			type = TOK_LABEL_CHAR;
			break;
		case DIRECT_CHAR:
			type = TOK_DIRECT_CHAR;
			break;
		default:
			type = TOK_UNKNOWN; // If file is valid, should never get there...
		}
	}
	if (is_string(val, len))
		return TOK_STRING;
	if (is_word(val))
		type = TOK_WORD;
	// if (is_header(val, len))
	// 	type = TOK_HEADER;
	// if (is_register(val, len))
	// 	type = TOK_REGISTER;
	if (is_number(val))
		type = TOK_NUMBER;
	// if (is_instruction(val, len))
	// 	type = TOK_INSTRUCTION;
	if (type == TOK_UNKNOWN)
		ln_add_error(ln, LN_ERR_STRAY_TOKEN, val);
	return type;
}
