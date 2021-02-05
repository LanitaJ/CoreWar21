#include "../../includes/asm.h"

int		is_whitespace(int c)
{
	return (c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

void	skip_whitespaces(t_parser *parser, const char *row)
{
	while (is_whitespace(row[parser->column]))
		parser->column++;
}

void	skip_comment(t_parser *parser, const char *row)
{
	if (row[parser->column] == COMMENT_CHAR
		|| row[parser->column] == ALT_COMMENT_CHAR)
		while (row[parser->column] && row[parser->column] != '\n')
			parser->column++;
}

int		is_register(const char *arg)
{
	int	i;

	i = 0;
	if (ft_strlen(arg) >= 2 && ft_strlen(arg) <= 3 && arg[i] == REG_CHAR)
	{
		i++;
		while (ft_isdigit(arg[i]))
			i++;
		return (!arg[i] && ft_atoi(&arg[1]) > 0);
	}
	return (0);
}
