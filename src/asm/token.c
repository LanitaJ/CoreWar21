#include "../../includes/asm.h"

void		add_token(t_token **chain, t_token *new)
{
	t_token	*cur;

	if (chain)
	{
		if (*chain)
		{
			cur = *chain;
			while (cur->next)
				cur = cur->next;
			if (cur->type == NEW_LINE && new->type == NEW_LINE)
				ft_memdel((void **)&new);
			else
				cur->next = new;
		}
		else
		{
			if (new->type == NEW_LINE)
				ft_memdel((void **)&new);
			else
				*chain = new;
		}
	}
}

char	*token_content(t_parser *parser, const char *row, unsigned start)
{
	char	*cnt;

	if (!(cnt = ft_strsub(row, start, parser->column - start)))
		kill("ERROR: Initializing string error");
	return (cnt);
}
