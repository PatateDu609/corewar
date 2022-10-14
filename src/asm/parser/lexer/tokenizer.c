#include <asm/parser.h>
#include <libft.h>
#include <common/op.h>

bool char_is_special(char c)
{
	return c == SEPARATOR_CHAR || c == LABEL_CHAR || c == DIRECT_CHAR;
}

static ssize_t get_tok_len(const char *line)
{
	if (char_is_special(line[0]))
		return 1;

	char quote = (*line == '"' || *line == '\'') ? *line : 0;
	ssize_t len = quote != 0;

	for (; line[len]; len++)
	{
		if (quote)
		{
			if (quote == line[len])
				continue;
			if (ENABLE_ESCAPE && line[len] == ESCAPE_CHAR)
			{
				len++;
				if (!line[len])
					return -1;
			}

			continue;
		}
		if (line[len] == ' '
			|| line[len] == '\t'
			|| line[len] == COMMENT_CHAR
			|| char_is_special(line[len]))
			break;
	}
	return len;
}

bool tokenize(struct line *ln)
{
	if (!ln || !ln->original)
		return false;

	ln->tokens = lst_token_new();
	if (!ln->tokens)
	{
		dprintf(2, "Error: Cannot allocate a new list.\n");
		exit(EXIT_FAILURE);
	}
	char *line = ln->original;

	char *true_line = ft_strtrim(line, " \t");
	if (!true_line)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
	if (!*true_line)
	{
		free(true_line);
		return false;
	}
	while (*true_line)
	{
		token_t tok = { .value = NULL, .type = TOK_DEFAULT};

		if (*true_line == '#')
			break;
		ssize_t tok_len = get_tok_len(true_line);
		if (!tok_len)
		{
			free(true_line);
			return false;
		}

		tok.ln = ln;
		tok.value = ft_strndup(true_line, tok_len);
		tok.type = get_token_type(tok.value);
		if (tok.type == TOK_STRING)
		{
			tok.value[tok_len - 1] = 0;
			ft_memmove(tok.value, tok.value + 1, tok_len - 1);
		}

		if (!lst_token_push_back(ln->tokens, tok))
		{
			dprintf(2, "Error: Cannot push a new token to list.\n");
			exit(EXIT_FAILURE);
		}
		line = ft_strtrim(true_line + tok_len, " \t");
		if (!line)
		{
			dprintf(2, "Error: Cannot allocate memory.\n");
			exit(EXIT_FAILURE);
		}
		free(true_line);
		true_line = line;
	}
	return true;
}
