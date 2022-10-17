#include <libft.h>
#include "asm/parser.h"

static char lf = '\n';

static size_t count_lines(char *content, size_t len)
{
	size_t ln_cnt = content[len - 1] != lf;
	for (size_t i = 0; i < len; i++)
		ln_cnt += content[i] == lf;
	return ln_cnt;
}

static inline ssize_t distance_lf(char *content)
{
	char *ptr = ft_strchr(content, lf);
	return ptr ? ptr - content : -1;
}

char **split_lines(char *content)
{
	size_t len = ft_strlen(content);
	size_t ln_cnt = count_lines(content, len);
	char **lines = ft_calloc(ln_cnt + 1, sizeof *lines);
	if (!lines)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < ln_cnt; i++)
	{
		size_t ln_len = distance_lf(content);
		lines[i] = ft_strndup(content, ln_len);
		if (!lines[i])
		{
			dprintf(2, "Error: Cannot allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		content += ln_len + 1;
	}
	return lines;
}