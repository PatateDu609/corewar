#include <libft.h>
#include "asm/parser.h"
#include "common/op.h"

bool is_word(const char *val)
{
	if (*val == HDR_PREFIX) // Header...
		val++;
	return is_label(val);
}
