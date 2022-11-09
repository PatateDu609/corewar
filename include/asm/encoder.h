#ifndef ASM_ENCODER_H
#define ASM_ENCODER_H

#include <stddef.h>
#include <inttypes.h>
#include "./parser.h"
#include "../common/op.h"
#include "../common/define.h"

struct encoded {
	t_header hdr;
	uint8_t assembly[];
};

struct encoder
{
	char *outfile;

	struct encoded *result;

	size_t nb_labels;
	struct label {
		char *name;
		uint32_t off;
		struct line *ln;
	} *labels;

	struct parser *p;
};

bool encode(struct parser *p);
bool init_encoder(struct encoder *enc);

size_t get_instr_size(ast_t *ast);

#endif