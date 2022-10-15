#ifndef ASM_ERROR_H
#define ASM_ERROR_H

#include <stddef.h>

enum ln_error
{
	LN_ERR_SUCCESS,

	// Detectable at lexer step
	LN_ERR_STRAY_TOKEN,

	// Detectable at parser step
	LN_ERR_INSTRUCTION_NOT_FOUND,

	// Errors triggered by invalid parameter of instruction or header...
	LN_ERR_UNWANTED_REGISTRY,
	LN_ERR_UNWANTED_DIRECT_PARAM,
	LN_ERR_UNWANTED_INDIRECT_PARAM,
	LN_ERR_UNWANTED_VALUE,
	LN_ERR_UNWANTED_LABEL,
	LN_ERR_UNWANTED_STRING,

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