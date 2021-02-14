#include "asm.h"

char	*get_token_content(t_parser *parser, const char *row, unsigned start)
{
	char	*content;

	if (!(content = ft_strsub(row, start, parser->column - start)))
		kill("ERROR: Initializing string error");
	return (content);
}
