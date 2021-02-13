#include "../../includes/asm.h"

int		is_whitespace(int c)
{
	return (c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

void	skip_whitespaces(const char *row, t_parser *parser)
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

int		is_delimiter(int c)
{
	if (c == '\0' || c == '\n' || c == ALT_COMMENT_CHAR
			|| is_whitespace(c) || c == COMMAND_CHAR || c == '\"'
			|| c == DIRECT_CHAR || c == SEPARATOR_CHAR || c == COMMENT_CHAR)
			return (1);
	else
		return (0);
}

int		is_register(const char *arg)
{
	int	i;

	i = 0;
	if ( ft_strlen(arg) < 4 && ft_strlen(arg) > 1 && arg[i] == REG_CHAR)
	{
		i++;
		while (ft_isdigit(arg[i]))
			i++;
		return (ft_atoi(&arg[1]) > 0 && !arg[i]);
	}
	return (0);
}

void	upgrade_row(char **row, char *ptr)
{
	char *new;

	if (!(new = ft_strdup(ptr)))
		kill("ERROR: Initializing string error");
	ft_strdel(row);
	*row = new;
}