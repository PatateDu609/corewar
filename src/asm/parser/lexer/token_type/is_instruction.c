#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_instruction(const char *val, size_t vlen)
{
	size_t size = sizeof op_tab / sizeof *op_tab;

	for (size_t i = 0; i < size; i++)
	{
		size_t len = ft_strlen(op_tab[i].name);

		if (vlen != len)
			continue;
		if (ft_strncmp(val, op_tab[i].name, len) == 0)
			return true;
	}
	return false;
}
