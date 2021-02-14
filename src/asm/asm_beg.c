#include "asm.h"

static void	process_name(t_parser *parser, t_token **cur)
{
	if ((*cur)->type == STRING)
	{
		if (!(parser->name = ft_strsub((*cur)->data,
									   1, ft_strlen((*cur)->data) - 2)))
			kill("ERROR: Initializing string error");
		if (ft_strlen(parser->name) > PROG_NAME_LENGTH)
			name_error();
		(*cur) = (*cur)->next;
	}
	else
		token_error(*cur);
}

static void	process_comment(t_parser *parser, t_token **cur)
{
	if ((*cur)->type == STRING)
	{
		if (!(parser->comment = ft_strsub((*cur)->data,
										  1, ft_strlen((*cur)->data) - 2)))
			kill("ERROR: Initializing string error");
		if (ft_strlen(parser->comment) > COMMENT_LENGTH)
			comment_error();
		(*cur) = (*cur)->next;
	}
	else
		token_error(*cur);
}

void		process_info(t_parser *parser, t_token **cur)
{
	while (!parser->name || !parser->comment)
	{
		if ((*cur)->type == COMMAND
			&& !parser->name
			&& !ft_strcmp((*cur)->data, ".name"))
		{
			(*cur) = (*cur)->next;
			process_name(parser, cur);
			if ((*cur)->type != NEW_LINE)
				token_error(*cur);
		}
		else if ((*cur)->type == COMMAND
				 && !parser->comment
				 && !ft_strcmp((*cur)->data, ".comment"))
		{
			(*cur) = (*cur)->next;
			process_comment(parser, cur);
			if ((*cur)->type != NEW_LINE)
				token_error(*cur);
		}
		else
			token_error(*cur);
		(*cur) = (*cur)->next;
	}
}

