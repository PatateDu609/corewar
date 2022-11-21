#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

enum token_type get_token_type(char *val, struct line *ln)
{
	size_t len = ft_strlen(val);
	enum token_type type = TOK_UNKNOWN;

	if (len == 1)
	{
		switch (val[0])
		{
			case SEPARATOR_CHAR:
				type = TOK_SEPARATOR_CHAR;
				break;
			case LABEL_CHAR:
				type = TOK_LABEL_CHAR;
				break;
			case '%':
				type = TOK_PERCENT; // Will be specified by the AST_GENERATION
				break;
			case '+':
				type = TOK_PLUS;
				break;
			case '-':
				type = TOK_MINUS;
				break;
			case '*':
				type = TOK_STAR;
				break;
			case '/':
				type = TOK_SLASH;
				break;
			case '(':
				type = TOK_OPEN_PARENTHESIS;
				break;
			case ')':
				type = TOK_CLOSE_PARENTHESIS;
				break;
			default:
				type = TOK_UNKNOWN; // If file is valid, should never get there...
		}
	}
	if (is_string(val, len))
		return TOK_STRING;
	if (is_whitespace(val, len))
		type = TOK_WHITESPACE;
	if (is_word(val))
		type = TOK_WORD;
	if (is_number(val))
		type = TOK_NUMBER;
	if (type == TOK_UNKNOWN)
		ln_add_error(ln, LN_ERR_STRAY_TOKEN, val);
	return type;
}
