#include "../../includes/asm.h"

int		is_whitespace(int c)
{
	return (c == '\t' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}

int		is_delimiter(int c)
{
		return (c == '\0' || c == '\n' || c == ';'
			|| is_whitespace(c) || c == '.' || c == '\"'
			|| c == '%' || c == ',' || c == '#');
}

int		is_register(const char *arg)
{
	int	i;

	i = 0;
	if ( ft_strlen(arg) < 4 && ft_strlen(arg) > 1 && arg[i] == 'r')
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