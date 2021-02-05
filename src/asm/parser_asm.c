#include "../../includes/asm.h"


void	parse_symbols(t_parser *parser,
					char *row,
					unsigned start,
					t_token *token)
{
	unsigned column;

	token->column = start;
	column = parser->column;
	while (row[parser->column]
		&& ft_strchr(LABEL_CHARS, row[parser->column]))
		parser->column++;
	if ((parser->column - column) && row[parser->column] == LABEL_CHAR
		&& ++parser->column)
	{
		token->data = get_token_content(parser, row, start);
		token->type = LABEL;
		add_token(&parser->token, token);
	}
	else if ((parser->column - column) && is_delimiter(row[parser->column]))
	{
		token->data = get_token_content(parser, row, start);
		if (token->type == INDIRECT)
			token->type = (is_register(token->data)) ? REGISTER : OPERATOR;
		add_token(&parser->token, token);
	}
	else
		lexical_error(parser);
}

void	parse_num(t_parser *parser,
					char *row,
					unsigned start,
					t_token *token)
{
	unsigned column;

	token->column = start;
	if (row[parser->column] == '-')
		parser->column++;
	column = parser->column;
	while (ft_isdigit(row[parser->column]))
		parser->column++;
	if ((parser->column - column)
		&& (token->type == DIRECT || is_delimiter(row[parser->column])))
	{
		token->data = get_token_content(parser, row, start);
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
	if (line[parser->column] == SEPARATOR_CHAR && ++parser->column)
		add_token(&parser->token, init_token(parser, SEPARATOR));
	//else if (line[parser->column] == '\"')
		//parse_string( , init_token(parser, STRING));
	else if (line[parser->column] == NEW_LINE)
		add_token(&parser->token, init_token(parser, NEW_LINE));
	else if (*(*line + parser->column) == DIRECT_CHAR && ++parser->column)
	{
		if (*(*line + parser->column) == LABEL_CHAR && ++parser->column)
			parse_symbols(parser, *line, parser->column - 2, init_token(parser, DIRECT_LABEL));
		else
			parse_num(parser, *line, parser->column - 1, init_token(parser, DIRECT));
	}	
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
			skip_whitespaces(parser, line);
			skip_comment(parser, line);
			if (line[parser->column])
				parse_token(parser, &line);
		}
		ft_strdel(&line);
	}
}