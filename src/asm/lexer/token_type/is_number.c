#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool __is_number(const char *str, const char *base, uint8_t base_size)
{
	char b[16];
	ft_strlcpy(b, base, base_size);

	for (; *str; str++)
		if (!ft_strchr(b, ft_tolower(*str)))
			return false;
	return true;
}

bool is_number(const char *str)
{
	char *set = "0123456789abcdef";

	if (__is_number(str, set, 2))
		return true;
	if (__is_number(str, set, 8))
		return true;
	if (__is_number(str, set, 10))
		return true;
	if (__is_number(str, set, 16))
		return true;
	return false;
}
