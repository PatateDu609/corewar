#include <libft.h>
#include "common/op.h"
#include "asm/parser.h"

bool is_label(const char *val)
{
	if (!ft_strchr(LABEL_CHARS, *val) && !ft_isdigit(*val))
		return false;
	val++;
	for (; *val; val++)
		if (!ft_strchr(LABEL_CHARS, *val))
			return false;
	return true;
}