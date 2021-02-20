/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_asm.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <ljerk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/13 15:26:57 by ljerk             #+#    #+#             */
/*   Updated: 2021/02/14 18:32:19 by ljerk            ###   ########.fr       */
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

void		parse_token(t_parser *parser, char **line)
{
	if (*(*line + parser->column) == ',' && ++parser->column)
		add_token(&parser->tokens, init_token(parser, SEPARATOR));
	else if (*(*line + parser->column) == '.')
		parse_symbols(parser, *line, \
			parser->column++, init_token(parser, COMMAND));
	else if (*(*line + parser->column) == '\"')
		parse_string(parser, line, parser->column, init_token(parser, STRING));
	else if (*(*line + parser->column) == '\n' && ++parser->column)
		add_token(&parser->tokens, init_token(parser, NEW_LINE));
	else if (*(*line + parser->column) == ':')
		parse_symbols(parser, *line, parser->column++,
					init_token(parser, INDIRECT_LABEL));
	else if (*(*line + parser->column) == '%' && ++parser->column)
	{
		if (*(*line + parser->column) == ':' && ++parser->column)
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
	char	*line;
	ssize_t	size;

	while (++parser->row
		&& !(parser->column = 0)
		&& (size = get_row(parser->fd, &line)) > 0)
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