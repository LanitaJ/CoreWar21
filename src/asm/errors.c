#include "../../includes/asm.h"

void	error_lex(t_parser *parser)
{
	ft_dprintf(2, "Lexical error at [%03u:%03u]\n",
													parser->row,
													parser->column + 1);
	exit(1);
}

void	token_error(t_token *token)
{
	ft_dprintf(2, "Unexpected token \"%s\" %s at [%03u:%03u]\n",
														token->content,
														g_type[token->type],
														token->row,
														token->column + 1);
	exit(1);
}

void	label_error(t_label *label)
{
	t_mention	*mention;

	ft_dprintf(2, "Undeclared label \"%s\" is mentioned at:\n", label->name);
	mention = label->mentions;
	while (mention)
	{
		ft_dprintf(2, "\t— [%03u:%03u]\n", mention->row, mention->column + 1);
		mention = mention->next;
	}
	exit(1);
}

void	operator_error(t_token *token)
{
	ft_dprintf(2, "Unknown operator \"%s\"at [%03u:%03u]\n",
															token->content,
															token->row,
															token->column + 1);
	exit(1);
}

void	arg_type_error(t_op *op, int arg_num, t_token *token)
{
	ft_dprintf(2, "Invalid type of parameter #%d for instruction \"%s\""\
													" at [%03u:%03u]\n",
															arg_num + 1,
															op->name,
															token->row,
															token->column + 1);
	exit(1);
}
