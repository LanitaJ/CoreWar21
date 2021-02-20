/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljerk <ljerk@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/14 18:21:48 by ljerk             #+#    #+#             */
/*   Updated: 2021/02/14 18:21:50 by ljerk            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/asm.h"

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
	t_tag	*tag;

	ft_printf("Undeclared label \"%s\" is taged at:\n", label->name);
	tag = label->tag;
	while (tag)
	{
		ft_printf("\t— [%03u:%03u]\n", tag->row, tag->column + 1);
		tag = tag->next;
	}
	exit(1);
}