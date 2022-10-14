#include "asm/parser.h"
#include "common/op.h"

bool is_string(const char *str, size_t len)
{
	return (str[0] == '"' || str[0] == '\'') && str[len - 1] == str[0];
}
