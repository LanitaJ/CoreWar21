#include "../../includes/asm.h"
#include <string.h>
#include <stdio.h>

int		check_name(char *filename)
{
	int len;

	len = strlen(filename);
	if (filename && filename[len - 2] == '.' && filename[len - 1] == 's')
		return (1);
	else
		return (0);
}

void	assemble()
{
	printf("5");
}

void	print_help(void)
{
	ft_printf("Error in file openning. \nUsage:\n./asm filename.s");
}

int		main(int argc, char **argv)
{
	if (argc == 2 && check_name(*(argv + 1)))
		assemble();
	else
		return (0);
		//print_help();
	return (0);
}