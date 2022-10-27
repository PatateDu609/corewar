#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_register(const char *val, size_t len)
{
	static bool _plen = false;
	static size_t plen;

	if (!_plen)
		plen = ft_strlen(REG_PREFIX);

	if (len <= plen)
		return false;

	if (ft_strncmp(REG_PREFIX, val, plen))
		return false;

	val += plen;
	if (!is_number(val))
		return false;

	int reg = ft_atoi(val);
	return (REG_MIN <= reg && reg <= REG_NUMBER);
}
