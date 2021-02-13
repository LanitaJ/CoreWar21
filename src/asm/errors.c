#include "../../includes/asm.h"

void	error_lex(t_parser *parser)
{
	ft_printf("{fd}Lexical error at [%03u:%03u]\n", 2,
													parser->row,
													parser->column + 1);
	exit(1);
}

void	token_error(t_token *token)
{
	ft_printf("{fd}Unexpected token \"%s\" %s at [%03u:%03u]\n", 2,
														token->data,
														g_type[token->type],
														token->row,
														token->column + 1);
	exit(1);
}



void	operator_error(t_token *token)
{
	ft_printf("{fd}Unknown operator \"%s\"at [%03u:%03u]\n", 2,
															token->data,
															token->row,
															token->column + 1);
	exit(1);
}
