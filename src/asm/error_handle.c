#include "../../includes/asm.h"


void	error_malloc(void)
{
	ft_printf("{fd}Malloc error\n", 2);
	exit(1);
}

void	error_fd(void)
{
	ft_printf("{fd}Fd error\n", 2);
	exit(1);
}