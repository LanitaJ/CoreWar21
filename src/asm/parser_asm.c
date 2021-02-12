#include "../../includes/asm.h"

void	kill(char *s)s
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

char	*token_content(t_parser *parser, const char *row, unsigned start)
{
	char	*cnt;

	if (!(cnt = ft_strsub(row, start, parser->column - start)))
		kill("ERROR: Can\'t initialize string");
	return (cnt);
}

void	parse_symbols(t_parser *parser, char *row, unsigned start, t_token *token)
{
	unsigned int col;

	col = parser->column;
	token->column = start;
	while (ft_strchr(LABEL_CHARS, row[parser->column])
		&& row[parser->column])
		parser->column++;
	if ((parser->column - col) && ++parser->column
		&& row[parser->column] == LABEL_CHAR)
	{
		token->type = LABEL;
		token->data = token_content(parser, row, start);
		add_token(&parser->token, token);
	}
	else if ((parser->column - col) && is_delimiter(row[parser->column]))
	{
		token->data = token_content(parser, row, start);
		if (token->type == INDIRECT)
			token->type = (is_register(token->data)) ? REGISTER : OPERATOR;
		add_token(&parser->token, token);
	}
	else
		error_lex(parser);
}

void	parse_num(t_parser* parser,
					char* row,
					unsigned int start,
					t_token* token)
{
	unsigned int col;

	token->column = start;
	if (row[parser->column] == '-')
		parser->column++;
	col = parser->column;
	while (ft_isdigit(row[parser->column]))
		parser->column++;
	if ((parser->column - col)
		&& (token->type == DIRECT || is_delimiter(row[parser->column])))
	{
		token->data = token_content(parser, row, start);
		add_token(&parser->token, token);
	}
	else if (token->type != DIRECT)
	{
		parser->column = start;
		parse_symbols(parser, row, start, token);
	}
	else
		lexical_error(parser);
}

void		parse_token(t_parser *parser, char **line)
{
	if (*(*row + parser->column) == SEPARATOR_CHAR && ++parser->column)
		add_token(&parser->tokens, init_token(parser, SEPARATOR));
	else if (*(*row + parser->column) == '.')
		parse_symbols(parser, *row,
			parser->column++, init_token(parser, COMMAND));
	else if (*(*row + parser->column) == '\"')
		parse_str(parser, row, parser->column, init_token(parser, STRING));
	else if (*(*row + parser->column) == '\n' && ++parser->column)
		add_token(&parser->tokens, init_token(parser, NEW_LINE));
	else if (*(*row + parser->column) == LABEL_CHAR)
		parse_symbols(parser, *row, parser->column++,
					init_token(parser, INDIRECT_LABEL));
	else if (*(*row + parser->column) == DIRECT_CHAR && ++parser->column)
	{
		if (*(*row + parser->column) == LABEL_CHAR && ++parser->column)
			parse_symbols(parser, *row, 
				parser->column - 2, init_token(parser, DIRECT_LABEL));
		else
			parse_num(parser, *row,
				parser->column - 1, init_token(parser, DIRECT));
	}
	else
		parse_num(parser, *row, parser->column, init_token(parser, INDIRECT));
}

void		parse_asm(t_parser *parser)
{
	size_t	size;
	char	*line;

	while (++parser->row && !(parser->column = 0) && 
			(size = get_next_line(parser->fd, &line) > 0))
	{
		line = ft_strjoin(line, "\n");
		ft_printf("%d %s", size, line);
		while(line[parser->column])
		{
			skip_whitespaces(line, parser);
			skip_comment(parser, line);
			if (line[parser->column])
				parse_token(parser, &line);
		}
		ft_strdel(&line);
	}
}