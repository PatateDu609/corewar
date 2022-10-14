#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_header(const char *val, size_t vlen)
{
	char *headers[] = {
		NAME_CMD_STRING,
		COMMENT_CMD_STRING,
	};
	size_t size = sizeof headers / sizeof *headers;

	for (size_t i = 0; i < size; i++)
	{
		size_t len = ft_strlen(headers[i]);
		if (vlen != len)
			continue;
		if (ft_strncmp(val, headers[i], len) == 0)
			return true;
	}
	return false;
}
