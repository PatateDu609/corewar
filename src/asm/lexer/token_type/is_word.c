#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_word(const char *val)
{
	if (!ft_strchr(LABEL_CHARS, *val++) && !ft_isdigit(*val))
		return false;
	for (; *val; val++)
		if (!ft_strchr(LABEL_CHARS, *val++))
			return false;
	return true;
}
