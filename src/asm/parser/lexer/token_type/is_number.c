#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

static char *base_set = "0123456789abcdef";

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

static bool is_number_checker(const char *str, uint8_t base_size)
{
	char b[16];
	static char *signs = "+-";

	ft_strlcpy(b, base_set, base_size + 1);
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
	if (is_number_checker(str, 2))
		return true;
	if (is_number_checker(str, 8))
		return true;
	if (is_number_checker(str, 10))
		return true;
	if (is_number_checker(str, 16))
		return true;
	return false;
}

static int32_t dump_value(const char *str, size_t b)
{
	char base[16];
	ft_strlcpy(base, base_set, b + 1);
	int32_t value = 0;

	for (; *str; str++)
	{
		char *c = ft_strchr(base, *str);

		if (!c)
			return value;
		value = (int32_t)(value * b + (c - base));
	}

	return value;
}

int32_t parse_number(const char *str)
{
	int32_t value = 0;
	int8_t sign = 1;

	for (; *str; str++)
	{
		if (ft_isdigit(*str))
			break;
		if (*str == '-')
			sign *= -1;
		else if (*str != '+')
			return 0;
	}

	if (check_prefix_base(&str, 2))
		value = dump_value(str, 2);
	else if (check_prefix_base(&str, 8))
		value = dump_value(str, 8);
	else if (check_prefix_base(&str, 16))
		value = dump_value(str, 16);
	else
		value = dump_value(str, 10);
	return value * sign;
}