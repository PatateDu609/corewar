#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool check_prefix_base(const char **str, uint8_t base_size)
{
	static const char *prefixes[] = {
		[2] = "0b",
		[8] = "0",
		[10] = "",
		[16] = "0x",
	};

	const char *prefix = prefixes[base_size];
	size_t plen = ft_strlen(prefix);
	if (plen >= ft_strlen(*str))
		return false;
	if (ft_strncmp(prefix, *str, plen))
		return false;
	*str += plen;
	return true;
}

bool __is_number(const char *str, const char *base, uint8_t base_size)
{
	char b[16];
	static char *signs = "+-";
	ft_strlcpy(b, base, base_size + 1);

	while (ft_strchr(signs, *str) && *str)
		str++;
	if (!*str)
		return false;
	if (!check_prefix_base(&str, base_size))
		return false;

	for (; *str; str++)
		if (!ft_strchr(b, ft_tolower(*str)))
			return false;
	return true;
}

bool is_number(const char *str)
{
	static char *set = "0123456789abcdef";

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
