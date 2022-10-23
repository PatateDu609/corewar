#ifndef ASM_PARSER_H
#define ASM_PARSER_H

#include "../common/define.h"
#include "./file.h"
#include "./error.h"

struct line;
typedef struct
{
	enum token_type {
		TOK_UNKNOWN = -1,
		TOK_DEFAULT = 0, // Default value affected before contextual analysis

		TOK_SEPARATOR_CHAR,
		TOK_WHITESPACE,

		TOK_DIRECT_CHAR,
		TOK_LABEL_CHAR,

		TOK_NUMBER,
		TOK_WORD,
		TOK_STRING,

		// Affected by ast construction (Subcases of TOK_WORD)
		TOK_HEADER,
		TOK_INSTRUCTION,
		TOK_LABEL,
		TOK_REGISTER,
	} type;

	char *value;
	struct line *ln;
} token_t;


#include <string.h>
// Linked list of tokens... Do not comment nor delete any of the 3 following lines.
#define STC42_LST_T token_t
#define STC42_LST_PRETTY_NAME token
#include <STC42/list.h>

typedef struct ast_node ast_t;
struct line
{
	char *original;
	size_t ln_nb;

	size_t nb_errors;
	struct error *errors;

	lst_token_t *tokens;
	ast_t *ast;
};

struct parser
{
	struct asm_file *input;

	size_t nb; // Number of lines
	struct line *lns;
};

bool parse(struct parser *p);
char **split_lines(char *content);
bool tokenize(struct line *line); // lexer
bool build_ast(struct line *ln); // actual parser
bool is_valid(struct line *ln); // Checks if the generated AST is valid

void free_tokens(struct lst_token_t *tokens);

char *dump_token_type(enum token_type type);
void dump_tokens(char *ast_filename, char *line, lst_token_t *toks);

enum token_type get_token_type(char *val, struct line *ln);

bool char_is_whitespace(char c);

bool is_string(const char *str, size_t len);
bool is_word(const char *str);
bool is_label(const char *val);
bool is_number(const char *str);
bool is_register(const char *val, size_t len);
bool is_header(const char *val, size_t len);
bool is_instruction(const char *val);
bool is_whitespace(const char *val, size_t vlen);

#endif
