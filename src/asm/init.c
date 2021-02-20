#include "../../includes/asm.h"

t_parser	*init_parser(int fd)
{
	t_parser	*parser;

	if (!(parser = (t_parser *)ft_memalloc(sizeof(t_parser))))
		error_malloc();
	parser->name = NULL;
	parser->comment = NULL;
	parser->fd = fd;
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

t_label		*init_label(char *name, int op_pos)
{
	t_label	*label;

	if (!(label = (t_label *)ft_memalloc(sizeof(t_label))))
		kill("Error: Can`t init label");
	if (!(label->name = ft_strdup(name)))
		kill("ERROR: Initializing string error");
	label->op_pos = op_pos;
	label->tag = NULL;
	label->next = NULL;
	return (label);
}

t_tag	*init_tag(t_parser *parser, t_token *token, size_t size)
{
	t_tag	*tag;

	if (!(tag = (t_tag *)ft_memalloc(sizeof(t_tag))))
		kill("Error: Can`t init tag");
	tag->row = token->row;
	tag->column = token->column;
	tag->pos = parser->pos;
	tag->op_pos = parser->op_pos;
	tag->size = size;
	tag->next = NULL;
	return (tag);
}
