#include "asm/parser.h"

static void free_token(token_t tok)
{
	free(tok.value);
}

void free_tokens(struct lst_token_t *tokens)
{
	if (!tokens)
		return ;
	tokens->destroy_data = free_token;
	lst_token_free(tokens);
}