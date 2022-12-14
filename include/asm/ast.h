#ifndef ASM_AST_H
#define ASM_AST_H

#include "./parser.h"

enum ast_node_type
{
	AST_INCOMPLETE = -2,
	AST_UNKNOWN = -1,
	AST_LINE,

	AST_HEADER,
	AST_LABEL,
	AST_INSTRUCTION,

	AST_PARAMS,
	AST_PARAM_DIRECT,
	AST_PARAM_INDIRECT,
	AST_PARAM_REGISTER,

	AST_TAG, // Special terminal value used to show special names (in instructions or headers)
	AST_TERMINAL_VALUE, // Can be string, number, or name (register, label name)
};

typedef struct ast_node
{
	enum ast_node_type type;

	size_t nb_childs;
	struct ast_node **childs;
	struct ast_node *parent;

	token_t *token; // Points out to the token in the token list from the tokenizer.
} ast_t;

ast_t *ast_new(enum ast_node_type type);
ast_t *ast_new_from_tok(token_t *tok);
ast_t *ast_new_child(struct ast_node *parent, enum ast_node_type type);

void free_ast(ast_t *ast);

bool complete_node(struct line *ln, ast_t *ast, lst_node_token_t *node);

bool ast_add_token(struct line *ln, ast_t **ast, lst_node_token_t **node);

enum ast_node_type get_ast_type_from_tok(enum token_type tok_type);
void dump_ast(char *filename, struct line *ln, const char *asm_file);

#endif
