/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_funcs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <ljerk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 18:20:02 by ljerk             #+#    #+#             */
/*   Updated: 2021/02/14 18:55:54 by ljerk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		add_token(&parser->tokens, token);
	}
	else if ((parser->column - column) && is_delimiter(row[parser->column]))
	{
		token->data = get_token_content(parser, row, start);
		if (token->type == INDIRECT)
			token->type = (is_register(token->data)) ? REGISTER : OPERATOR;
		add_token(&parser->tokens, token);
	}
	else
		error_lex(parser);
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

void	parse_num(t_parser *parser,
					char *row,
					unsigned start,
					t_token *token)
{
	unsigned int column;

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