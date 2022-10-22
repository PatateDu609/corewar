#include "asm/ast.h"

void free_ast(ast_t *ast)
{
	if (!ast)
		return ;
	for (size_t i = 0; i < ast->nb_childs; i++)
		free_ast(ast->childs[i]);
	free(ast->childs);
	free(ast);
}