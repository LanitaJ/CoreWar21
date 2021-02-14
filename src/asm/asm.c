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

char	*replace_extension(char *filename, char *old, char *new)
{
	char	*basename;

	basename = ft_strsub(filename, 0, ft_strlen(filename) - ft_strlen(old));
	if (!basename)
		kill("ERROR: Initializing string error");
	if (!(filename = ft_strjoin(basename, new)))
		kill("ERROR: Initializing string error");
	ft_strdel(&basename);
	return (filename);
}

void	assemble(char *filename)
{
	int			fd;
	t_parser	*parser;
	t_token		*cur;
	
	if ((fd = open(filename, O_RDONLY)) == -1)
		error_fd();
	parser = init_parser(fd);
	parse_asm(parser);
	cur = parser->tokens;
	process_info(parser, &cur);
	process_asm_code(parser, &cur);
	replace_mentions(parser);
	filename = replace_extension(filename, ".s", ".cor");
	if ((fd = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644)) == -1)
		kill("ERROR: Can\'t create file");
	write_bytecode_file(fd, parser);
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