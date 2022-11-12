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
		const char *name;
		int32_t off;
		struct line *ln;
	} *labels;

	struct parser *p;
};

bool encode(struct parser *p);
bool init_encoder(struct encoder *enc);
void fill_encoder(struct encoder *enc);
bool dump_to_file(struct encoder *enc);

size_t get_instr_size(ast_t *ast);

size_t get_pcode_size(t_op *op);
uint32_t get_pcode(uint8_t *encoded, ast_t *params, uint32_t local_offset);

uint32_t dump_direct_param(ast_t *param, struct encoder *enc, t_op *op, uint32_t local_offset, uint32_t offset);
void dump_indirect_param(ast_t *param, struct encoder *enc, uint32_t local_offset, uint32_t offset);

bool check_label(struct encoder *enc, struct line *ln, char *name);
void add_label(struct encoder *enc, struct line *ln, const char *name, const uint32_t *prog_size);
struct label *lookup_label(struct encoder *enc, char *name);

#endif