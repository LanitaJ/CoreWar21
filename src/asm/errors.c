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

void	name_error()
{
	ft_printf("Champion name too long (Max length %u)\n", PROG_NAME_LENGTH);
	exit(1);
}

void	comment_error()
{
	ft_printf("Champion comment too long (Max length %u)\n",
			   COMMENT_LENGTH);
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

void	arg_type_error(t_op *op, int arg_num, t_token *token)
{
	ft_printf("Invalid type of parameter #%d for instruction \"%s\""\
													" at [%03u:%03u]\n",
			   arg_num + 1,
			   op->name,
			   token->row,
			   token->column + 1);
	exit(1);
}

void	label_error(t_label *label)
{
	t_mention	*mention;

	ft_printf("Undeclared label \"%s\" is mentioned at:\n", label->name);
	mention = label->mentions;
	while (mention)
	{
		ft_printf("\t— [%03u:%03u]\n", mention->row, mention->column + 1);
		mention = mention->next;
	}
	exit(1);
}