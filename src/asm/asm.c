#include "../../includes/asm.h"


int		check_name(char *filename)
{
	int len;

	len = ft_strlen(filename);
	if (filename && filename[len - 2] == '.' && filename[len - 1] == 's')
		return (1);
	else
		return (0);
}

void	assemble(char *filename)
{
	int	fd;
	t_parser *parser;
	
	if ((fd = open(filename, O_RDONLY)) == -1)
		error_fd();
	parser = init_parser(fd);
	parse_asm(parser);
}

void	print_help(void)
{
	ft_printf("Error in file openning. \nUsage:\n./asm filename.s");
}

int		main(int argc, char **argv)
{
	if (argc == 2 && check_name(*(argv + 1)))
		assemble(*(argv + 1));
	else
		print_help();
	return (0);
}