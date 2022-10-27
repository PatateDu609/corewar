#include <asm/parser.h>
#include <stdio.h>

#define BOLD "\e[1m"
#define RED "\e[38;5;124m"
#define BLUE "\e[38;5;20m"
#define GREEN "\e[38;5;46m"
#define RESET "\e[0m"

char *dump_token_type(enum token_type type)
{
	switch (type)
	{
	case TOK_DEFAULT:
		return "Default";
	case TOK_SEPARATOR_CHAR:
		return "Parameter separator";
	case TOK_WHITESPACE:
		return "Whitespace (space or tab)";
	case TOK_DIRECT_CHAR:
		return "Direct value char";
	case TOK_LABEL_CHAR:
		return "Label char";
	case TOK_HEADER:
		return "Header";
	case TOK_REGISTER:
		return "Register";
	case TOK_NUMBER:
		return "Number";
	case TOK_WORD:
		return "Word";
	case TOK_INSTRUCTION:
		return "Instruction";
	case TOK_STRING:
		return "String";
	case TOK_LABEL:
		return "Label";
	default:
		return "Unknown";
	}
}

void dump_tokens(char *ast_filename, char *line, lst_token_t *toks)
{
	printf("-----------------------------------------------------------------\n");
	printf("\n");

	if (ast_filename)
		printf(RED BOLD "AST dot file:" RESET BOLD " %s" RESET "\n", ast_filename);
	if (line)
		printf(RED "Original line:" RESET " %s\n\n", line);
	for (lst_node_token_t *node = toks->start; node; node = node->next)
	{
		token_t data = node->data;
		printf(BOLD BLUE "node: " RESET BOLD "%s\n" RESET, dump_token_type(data.type));
		if (data.value)
			printf(BOLD BLUE "value: " RESET "%s\n", data.value);

		if (node->next)
			printf("\n");
	}
}
