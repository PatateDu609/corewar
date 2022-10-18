#ifndef ASM_ERROR_H
#define ASM_ERROR_H

#include <stddef.h>

enum ln_error
{
	LN_ERR_SUCCESS,

	// Detectable at lexer step
	LN_ERR_STRAY_TOKEN,

	// Detectable at parser step
	LN_ERR_UNKNOWN_INSTRUCTION,
	LN_ERR_UNKNOWN_HEADER,

	// Errors triggered by invalid parameter of instruction or header...
	LN_ERR_UNEXPECTED_REGISTER,
	LN_ERR_UNEXPECTED_DIRECT_PARAM,
	LN_ERR_UNEXPECTED_INDIRECT_PARAM,
	LN_ERR_UNEXPECTED_VALUE,
	LN_ERR_UNEXPECTED_LABEL,
	LN_ERR_UNEXPECTED_LABEL_DECL,
	LN_ERR_UNEXPECTED_STRING,
	LN_ERR_UNEXPECTED_INSTRUCTION,
	LN_ERR_UNEXPECTED_HEADER,
	LN_ERR_UNEXPECTED_SEPARATOR_CHAR,

	LN_ERR_INVALID_LABEL,
	LN_ERR_CANT_COMPLETE_WORD,

	LN_ERR_WRONG_NUMBER_ARGUMENTS,
};

struct error {
	size_t err_chr;
	enum ln_error err;
	char *str;
};

struct line;
void ln_add_error(struct line *ln, enum ln_error type, char *val);

void print_errors(struct line *ln);

#include "parser.h"

#endif