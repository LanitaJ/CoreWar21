/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_asm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <ljerk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 15:26:57 by ljerk             #+#    #+#             */
/*   Updated: 2021/02/13 16:45:01 by ljerk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/asm.h"

void	kill(char *s)
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

char	*join_str(char **str1, char **str2)
{
	char *result;

	if (!(result = ft_strjoin(*str1, *str2)))
		kill("ERROR: Initializing string error");
	ft_strdel(str1);
	ft_strdel(str2);
	return (result);
}

void	update_parser_position(t_parser *parser, const char *row)
{
	unsigned i;

	i = ++(parser->column);
	while (row[i] && row[i] != '\"')
	{
		if (row[i] == '\n')
		{
			parser->row++;
			parser->column = 0;
		}
		else
			parser->column++;
		i++;
	}
}

void	parse_symbols(t_parser *parser, char *row, unsigned start, t_token *token)
{
	unsigned int col;

	col = parser->column;
	token->column = start;
	while (ft_strchr(LABEL_CHARS, row[parser->column]) && row[parser->column])
		parser->column++;
	if ((parser->column - col) && ++parser->column
		&& row[parser->column] == LABEL_CHAR)
	{
		token->type = LABEL;
		token->data = token_content(parser, row, start);
		add_token(&parser->tokens, token);
	}
	else if (is_delimiter(row[parser->column]) && parser->column - col)
	{
		token->data = token_content(parser, row, start);
		if (token->type == INDIRECT)
			token->type = (is_register(token->data)) ? REGISTER : OPERATOR;
		add_token(&parser->tokens, token);
	}
	else
		error_lex(parser);
}

void	parse_num(t_parser* parser, char* row, unsigned int start, t_token* token)
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
		add_token(&parser->tokens, token);
	}
	else if (token->type != DIRECT)
	{
		parser->column = start;
		parse_symbols(parser, row, start, token);
	}
	else
		error_lex(parser);
}

void	parse_string(t_parser *parser, char **row, unsigned start, t_token *token)
{
	char	*end;
	ssize_t	size;
	char	*str;

	token->column = start;
	size = 1;
	while (!(end = ft_strchr(&((*row)[start + 1]), '\"'))
		&& (size = get_row(parser->fd, &str)) > 0)
		*row = join_str(row, &str);
	update_parser_position(parser, *row);
	if (size == 0)
		error_lex(parser);
	if (size == -1)
		kill("ERROR: Can\'t read file");
	if (!(token->data = ft_strsub(*row, start, end + 1 - &((*row)[start]))))
		kill("ERROR: Initializing string error");
	if (end - parser->column != *row)
		upgrade_row(row, end - parser->column);
	parser->column++;
	add_token(&parser->tokens, token);
}

void		parse_token(t_parser *parser, char **line)
{
	if (*(*line + parser->column) == SEPARATOR_CHAR && ++parser->column)
		add_token(&parser->tokens, init_token(parser, SEPARATOR));
	else if (*(*line + parser->column) == '.')
		parse_symbols(parser, *line,
			parser->column++, init_token(parser, COMMAND));
	else if (*(*line + parser->column) == '\"')
		parse_string(parser, line, parser->column, init_token(parser, STRING));
	else if (*(*line + parser->column) == '\n' && ++parser->column)
		add_token(&parser->tokens, init_token(parser, NEW_LINE));
	else if (*(*line + parser->column) == LABEL_CHAR)
		parse_symbols(parser, *line, parser->column++,
					init_token(parser, INDIRECT_LABEL));
	else if (*(*line + parser->column) == DIRECT_CHAR && ++parser->column)
	{
		if (*(*line + parser->column) == LABEL_CHAR && ++parser->column)
			parse_symbols(parser, *line, 
				parser->column - 2, init_token(parser, DIRECT_LABEL));
		else
			parse_num(parser, *line,
				parser->column - 1, init_token(parser, DIRECT));
	}
	else
		parse_num(parser, *line, parser->column, init_token(parser, INDIRECT));
}

void		parse_asm(t_parser *parser)
{
	size_t	size;
	char	*line;

	while ((size = get_row(parser->fd, &line)) > 0
		&& !(parser->column = 0) && ++parser->row)
	{
		while(line[parser->column])
		{
			skip_whitespaces(line, parser);
			skip_comment(parser, line);
			if (line[parser->column])
				parse_token(parser, &line);
		}
		ft_strdel(&line);
	}
	if (size == -1)
		kill("ERROR: Can\'t read file");
	add_token(&(parser->tokens), init_token(parser, END));
}