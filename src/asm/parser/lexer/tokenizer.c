#include <asm/parser.h>
#include <libft.h>
#include <common/op.h>

enum char_mode {
	MODE_DEF,
	MODE_SQ,
	MODE_DQ,
	MODE_WS,
};

enum tok_ret_error {
	TOK_RET_FATAL = -2,
	TOK_RET_MINOR_ERROR,
	TOK_RET_SUCCESS,
};

#define MISSING_DQ (-1)
#define MISSING_SQ (-2)

static inline bool char_is_special(char c)
{
	return c == SEPARATOR_CHAR || c == DIRECT_CHAR || c == LABEL_CHAR || c == COMMENT_CHAR ||
		c == '+' || c == '-' || c == '*' || c == '(' || c == ')';
}

static void setup_tokenizer(struct line *ln)
{
	ln->tokens = lst_token_new();
	if (!ln->tokens)
	{
		dprintf(2, "Error: Cannot allocate memory.\n");
		exit(EXIT_FAILURE);
	}
}

static enum char_mode get_char_mode(char c)
{
	switch(c)
	{
	case ' ':
	case '\t':
		return MODE_WS;
	case '\'':
		return MODE_SQ;
	case '"':
		return MODE_DQ;
	default:
		return MODE_DEF;
	}
}

static ssize_t toklen(char *line)
{
	ssize_t len = 0;

	if (char_is_special(line[len]))
		return 1;

	enum char_mode mode = get_char_mode(line[len]);
	if (mode == MODE_SQ || mode == MODE_DQ)
		len++;

	for (; line[len]; len++)
	{
		switch (mode)
		{
		case MODE_WS:
			if (!char_is_whitespace(line[len]))
				return len;
			break;
		case MODE_DQ:
			if (line[len] == '"')
				return len + 1;
			break;
		case MODE_SQ:
			if (line[len] == '\'')
				return len + 1;
			break;
		default:
			if (char_is_special(line[len]) || char_is_whitespace(line[len]) || line[len] == COMMENT_CHAR)
				goto exit_loop;
		}
	}
exit_loop:
	if (mode == MODE_DQ)
		return MISSING_DQ;
	else if (mode == MODE_SQ)
		return MISSING_SQ;
	return len;
}

static void manage_special_types(token_t *token, size_t len)
{
	switch (token->type)
	{
	case TOK_STRING:
		token->value[len - 1] = 0;
		ft_memmove(token->value, token->value + 1, len - 1);
		break;
	case TOK_WHITESPACE:
		free(token->value);
		token->value = NULL;
		break;
	default:
		break;
	}
}

static enum tok_ret_error set_token(struct line *ln, token_t *token, char **line)
{
	ssize_t len = toklen(*line);

	if (len < 0)
	{
		enum ln_error err = len == MISSING_DQ
							? LN_ERR_MISSING_DOUBLE_QUOTE
							: LN_ERR_MISSING_SINGLE_QUOTE;
		ln_add_error(ln, err, NULL);
		return TOK_RET_FATAL;
	}
	token->value = ft_strndup(*line, len);
	token->type = get_token_type(token->value, ln);
	*line += len;

	if (token->type == TOK_UNKNOWN)
		return TOK_RET_MINOR_ERROR;
	manage_special_types(token, len);
	return TOK_RET_SUCCESS;
}

bool tokenize(struct line *ln)
{
	if (!ln || !ln->original)
		return false;
	setup_tokenizer(ln);
	char *line = ln->original;
	token_t token = { .ln = ln, .type = TOK_DEFAULT, .value = NULL };
	bool res = true;

	while (*line)
	{
		if (*line == COMMENT_CHAR)
			return res;
		enum tok_ret_error tok_res = set_token(ln, &token, &line);
		if (tok_res == TOK_RET_FATAL)
			return false;
		if (tok_res == TOK_RET_MINOR_ERROR)
			res = false;
		if (!lst_token_push_back(ln->tokens, token))
		{
			dprintf(2, "Error: Cannot push a new element into the list.\n");
			exit(EXIT_FAILURE);
		}
	}
	return res;
}