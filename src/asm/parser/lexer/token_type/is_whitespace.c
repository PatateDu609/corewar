#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool char_is_whitespace(char c)
{
	return c == ' ' || c == '\t';
}

bool is_whitespace(const char *val, size_t vlen)
{
	for (size_t i = 0; i < vlen; i++)
	{
		if (!char_is_whitespace(val[i]))
			return false;
	}
	return true;
}