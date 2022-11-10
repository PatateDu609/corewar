#include <libft.h>
#include "asm/encoder.h"
#include <stdio.h>
#include "common/op.h"

static void print_header(struct encoder *enc)
{
	t_header *hdr = &enc->result->hdr;
	char *title[] = {
		"", // For the first two lines...
		"Program size",
		"Magic (big endian)",
		"Magic (little endian)",
	};
	int longest = 0;

	for (size_t i = 0, size = sizeof title / sizeof *title; i < size; i++)
	{
		int len = ft_strlen(title[i]);
		longest = longest < len ? len : longest;
	}

	longest += 2;

	printf("Assembling %s:\n", enc->p->input->filename);
	printf("\033[1;31m%-*s\033[0m%s\n", longest, title[0], hdr->prog_name);
	printf("\033[1;31m%-*s\033[0m%s\n\n", longest, title[0], hdr->comment);

	printf("\033[1;31m%-*s\033[0m%u\n", longest, title[1], hdr->prog_size);
	printf("\033[1;31m%-*s\033[0m%#010x\n", longest, title[2], hdr->magic);
	printf("\033[1;31m%-*s\033[0m%#010x\n", longest, title[3], bswap_32(hdr->magic));
}


static bool print_enc_errors(struct encoder *enc)
{
	for (size_t i = 0; i < enc->p->nb; i++)
	{
		struct line *ln = enc->p->lns + i;
		if (ln->nb_errors)
			print_errors(ln);
	}
	return false;
}

static void print_labels(struct encoder *enc)
{
	if (!enc->nb_labels)
		printf("No label found.\n");
	int col_sizes[] = { 20, 15, 15 };
	printf("\033[1m%-*s| %-*s| %-*s\033[0m\n",
		col_sizes[0], "Name",
		col_sizes[1], "Offset",
		col_sizes[2], "Line number");
	printf("\033[1m%-*s| %-*s| %-*s\033[0m\n",
		col_sizes[0], "",
		col_sizes[1], "",
		col_sizes[2], "");
	for (size_t i = 0; i < enc->nb_labels; i++)
	{
		printf("%-*s\033[1m|\033[0m", col_sizes[0], enc->labels[i].name);

		char offset[col_sizes[1]];
		snprintf(offset, sizeof offset, "%0*X", 4 * 2, enc->labels[i].off);
		char offset_str[128];
		snprintf(offset_str, sizeof offset_str, "%2.2s %2.2s %2.2s %2.2s",
			offset, offset + 2, offset + 4, offset + 6);
		printf(" %-*s\033[1m|\033[0m", col_sizes[1], offset_str);

		printf(" %-*zu\n", col_sizes[2], enc->labels[i].ln->ln_nb);
	}
}

bool encode(struct parser *p)
{
	struct encoder enc;

	enc.p = p;
	if (!init_encoder(&enc))
		return print_enc_errors(&enc);
	print_header(&enc);
	printf("\n");
	print_labels(&enc);

	fill_encoder(&enc);
	return true;
}