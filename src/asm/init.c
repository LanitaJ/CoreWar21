#include "../../includes/asm.h"

t_parser	*init_parser(int fd)
{
	t_parser	*parser;

	if (!(parser = (t_parser *)ft_memalloc(sizeof(t_parser))))
		error_malloc();
	parser->fd = fd;
	parser->name = NULL;
	parser->comment = NULL;
	return (parser);
}

t_token		*init_token(t_parser *parser, t_type type)
{
	t_token	*token;

	if (!(token = (t_token *)ft_memalloc(sizeof(t_token))))
		kill("Error: Can`t init token");
	token->row = parser->row;
	token->data = NULL;
	if (type == SEPARATOR || type == NEW_LINE)
		token->column = parser->column - 1;
	else
		token->column = parser->column;
	token->next = NULL;
	token->type = type;
	return (token);
}