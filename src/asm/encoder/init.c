#define _GNU_SOURCE
#include <libft.h>
#include "asm/encoder.h"
#include "asm/ast.h"

static char *get_outfile(struct parser *p)
{
	char *outfile = ft_strdup(p->input->filename);
	size_t len = ft_strlen(outfile);
	char *new_ext = "cor";
	char *old_ext = "s";
	size_t new_ext_len = ft_strlen(new_ext);
	size_t old_ext_len = ft_strlen(old_ext);

	if (!outfile)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}

	outfile = realloc(outfile, (len + (new_ext_len - old_ext_len) + 1) * sizeof *outfile);
	if (!outfile)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	ft_memcpy(outfile + len - old_ext_len, new_ext, new_ext_len);
	outfile[len + (new_ext_len - old_ext_len)] = 0;
	return outfile;
}

static bool check_label(struct encoder *enc, struct line *ln, char *name)
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

static void add_label(struct encoder *enc, struct line *ln, char *name, const uint32_t *prog_size)
{
	enc->labels = realloc(enc->labels, (++enc->nb_labels) * sizeof *enc->labels);

	if (!enc->labels)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	enc->labels[enc->nb_labels - 1] = (struct label) { .ln = ln, .off = *prog_size, .name = name };
}

static void fill_hdr(struct encoded *encoded, char *tag, char *param)
{
	if (!ft_strcmp(tag, NAME_CMD_STRING))
		ft_strlcpy(encoded->hdr.prog_name, param, PROG_NAME_LENGTH);
	else if (!ft_strcmp(tag, COMMENT_CMD_STRING))
		ft_strlcpy(encoded->hdr.comment, param, COMMENT_LENGTH);
}

static void resize_prog(struct encoder *enc)
{
	size_t new_size = sizeof enc->result->hdr;
	new_size += enc->result->hdr.prog_size * sizeof *enc->result->assembly;

	enc->result = realloc(enc->result, new_size);
	if (!enc->result)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
}

bool init_encoder(struct encoder *enc)
{
	enc->nb_labels = 0;
	enc->labels = NULL;
	enc->outfile = get_outfile(enc->p);
	enc->result = ft_calloc(1, sizeof *enc->result);
	enc->result->hdr.magic = bswap_32(COREWAR_EXEC_MAGIC);
	uint32_t *prog_size = &enc->result->hdr.prog_size;
	*prog_size = 0;

	bool res = true;
	for (size_t i = 0; i < enc->p->nb; i++)
	{
		struct line *ln = enc->p->lns + i;
		ast_t *ast = ln->ast;
		if (!ast)
			continue;
		ast = ast->childs[0];
		if (ast->type == AST_INSTRUCTION)
		{
			size_t instr_size = get_instr_size(ast);
			*prog_size += instr_size;
			printf("Instruction has size of %zu\n", instr_size);
		}
		else if (ast->type == AST_LABEL)
		{
			char *name = ast->childs[0]->token->value;
			if (check_label(enc, ln, name))
				add_label(enc, ln, name, prog_size);
			else
				res = false;
			if (ast->parent->nb_childs == 2)
			{
				size_t instr_size = get_instr_size(ast->parent->childs[1]);
				*prog_size += instr_size;
				printf("Instruction has size of %zu\n", instr_size);
			}
		}
		else if (ast->type == AST_HEADER)
		{
			char *tag = ast->childs[0]->token->value;
			char *param = ast->childs[1]->childs[0]->token->value;
			fill_hdr(enc->result, tag, param);
		}
	}
	if (res && *prog_size)
		resize_prog(enc);
	return res;
}