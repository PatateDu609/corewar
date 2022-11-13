#include "asm/encoder.h"
#include <stdio.h>
#include <libft.h>

bool check_label(struct encoder *enc, struct line *ln, char *name)
{
	for (size_t i = 0; i < enc->nb_labels; i++)
	{
		if (!ft_strcmp(enc->labels[i].name, name))
		{
			ln_add_error(ln, LN_ERR_LABEL_DOUBLE_DECLARATION, name);
			return false;
		}
	}
	return true;
}

void add_label(struct encoder *enc, struct line *ln, const char *name, const uint32_t *prog_size)
{
	enc->labels = realloc(enc->labels, (++enc->nb_labels) * sizeof *enc->labels);

	if (!enc->labels)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	enc->labels[enc->nb_labels - 1] = (struct label) { .ln = ln, .off = (int32_t)*prog_size, .name = name };
}

struct label *lookup_label(struct encoder *enc, char *name)
{
		size_t len = ft_strlen(name);
	for (size_t i = 0; i < enc->nb_labels; i++)
	{
		size_t current_len = ft_strlen(enc->labels[i].name);
		if (len != current_len)
			continue;
		if (!ft_strncmp(name, enc->labels[i].name, len))
			return enc->labels + i;
	}
	return NULL;
}