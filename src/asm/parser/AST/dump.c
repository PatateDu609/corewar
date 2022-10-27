#define _GNU_SOURCE

#include "asm/ast.h"
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <libft.h>

char *dump_ast_node_type(enum ast_node_type type)
{
	switch(type)
	{
	case AST_LINE:
		return "Line";
	case AST_INSTRUCTION:
		return "Instruction";
	case AST_LABEL:
		return "Label";
	case AST_HEADER:
		return "Header";
	case AST_PARAM_DIRECT:
		return "Direct";
	case AST_PARAM_INDIRECT:
		return "Indirect";
	case AST_PARAM_REGISTER:
		return "Register";
	case AST_PARAMS:
		return "Params";
	case AST_TAG:
		return "Tag";
	case AST_TERMINAL_VALUE:
		return "Terminal value";
	default:
		return "Unknown";
	}
}

static void print_node(int fd, struct ast_node *node)
{
	dprintf(fd, "\t%2$c%1$p%2$c [\n", node, '"');

	dprintf(fd, "\t\tlabel=<\n");
	dprintf(fd, "\t\t<TABLE BORDER=%1$c0%1$c CELLBORDER=%1$c1%1$c CELLSPACING=%1$c0%1$c>\n", '"');

	dprintf(fd, "\t\t\t<TR>\n");
	char *str = dump_ast_node_type(node->type);
	char *tok_str = node->token ? dump_token_type(node->token->type) : "";
	size_t width = ft_strlen(str) * 10;
	size_t tok_str_len = ft_strlen(tok_str);
	width = width > tok_str_len ? width : tok_str_len;
	if (tok_str_len && width < 100)
		width = 100;
	if (width > 110)
		width = 110;

	if (node->token)
	{
		dprintf(fd, "\t\t\t\t<TD WIDTH=\"%zu\"><B>%s</B></TD>\n", width, str);
		dprintf(fd, "\t\t\t\t<TD ROWSPAN=%1$c2%1$c WIDTH=%1$c120%1$c>%2$s</TD>\n",
			'"', node->token->value ? node->token->value : "NULL");
	}
	else
		dprintf(fd, "\t\t\t\t<TD WIDTH=\"%zu\" HEIGHT=\"30\"><B>%s</B></TD>\n", width, str);
	dprintf(fd, "\t\t\t</TR>\n");
	if (node->token)
	{
		dprintf(fd, "\t\t\t<TR>\n");
		dprintf(fd, "\t\t\t\t<TD><I>%s</I></TD>\n", tok_str);
		dprintf(fd, "\t\t\t</TR>\n");
	}
	dprintf(fd, "\t\t</TABLE>>\n");

	dprintf(fd, "\t]\n");
}

static void dump_node(int fd, struct ast_node *node)
{
	if (!node || fd == -1)
		return ;
	print_node(fd, node);

	if (!node->nb_childs)
		return ;
	for (size_t i = 0; i < node->nb_childs; i++)
		dprintf(fd, "\t\"%p\" -- \"%p\"\n", node, node->childs[i]);
	for (size_t i = 0; i < node->nb_childs; i++)
		dump_node(fd, node->childs[i]);
}

void dump_ast(char *filename, struct line *ln, const char *asm_file)
{
	ast_t *ast = ln->ast;

	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		dprintf(2, "Error: %s: %s.\n", filename, strerror(errno));
		exit(EXIT_FAILURE);
	}

	dprintf(fd, "// line %zu of file %s\n", ln->ln_nb, asm_file);
	dprintf(fd, "// %s\n", ln->original);
	dprintf(fd, "graph G {\n");
	dprintf(fd, "\tnode [ shape=plaintext ]\n\n");
	dump_node(fd, ast);
	dprintf(fd, "}");

	close(fd);
}