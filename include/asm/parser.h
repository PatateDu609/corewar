#ifndef ASM_PARSER_H
#define ASM_PARSER_H

#include "../common/define.h"
#include "./file.h"

typedef struct
{
	enum token_type {
		TOK_UNKNOWN = -1,
		TOK_DEFAULT = 0, // Default value affected before contextual analysis

		TOK_SEPARATOR_CHAR,
		TOK_DIRECT_CHAR,
		TOK_LABEL_CHAR,

		TOK_HEADER,
		TOK_REGISTER,
		TOK_NUMBER,
		TOK_WORD,
		TOK_INSTRUCTION,
		TOK_STRING,
	} type;

	char *value;
} token_t;


#include <string.h>
// Linked list of tokens... Do not comment nor delete any of the 3 following lines.
#define STC42_LST_T token_t
#define STC42_LST_PRETTY_NAME token
#include <STC42/list.h>

enum ln_error
{
	LN_ERR_SUCCESS,

	// Detectable at lexer step
	LN_ERR_UNKNOWN_CHARACTER,
	LN_ERR_WRONG_LABEL,
	LN_ERR_WRONG_NUMBER,

	// Detectable at parser step
	LN_ERR_INSTRUCTION_NOT_FOUND,
};

struct line
{
	char *original;
	size_t ln_nb;

	size_t err_chr;
	enum ln_error err;

	lst_token_t *tokens;
};

struct parser
{
	struct asm_file *input;

	size_t nb; // Number of lines
	struct line *lns;
};

void lexer(struct parser *p);
bool tokenize(char *line, lst_token_t *toks);

void dump_tokens(char *line, lst_token_t *toks);

enum token_type get_token_type(char *val);

bool is_string(const char *str, size_t len);
bool is_word(const char *str);
bool is_number(const char *str);
bool is_register(const char *val, size_t len);
bool is_header(const char *val, size_t len);
bool is_instruction(const char *val, size_t vlen);

#endif
