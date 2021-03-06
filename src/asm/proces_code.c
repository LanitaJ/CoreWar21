#include "../../includes/asm.h"

static void		process_label(t_parser *parser, t_token **cur)
{
	t_label	*label;
	char	*name;

	if (!(name = ft_strsub((*cur)->data,0, ft_strlen((*cur)->data) - 1)))
		kill("ERROR: Initializing string error");;
	if (!(label = find_label(parser->labels, name)))
		add_label(&(parser->labels), init_label(name, parser->op_pos));
	if (label && label->op_pos == -1)
		label->op_pos = parser->pos;
	ft_strdel(&name);
}

static int8_t	process_args(t_parser *parser, t_token **cur, t_op *op)
{
	int		arg_num;
	int8_t	types_code;
	int8_t	type;

	arg_num = 0;
	types_code = 0;
	while (arg_num < op->args_num)
	{
		if ((*cur)->type >= REGISTER && (*cur)->type <= INDIRECT_LABEL)
		{
			type = process_arg(parser, cur, op, arg_num);
			update_types_code(&types_code, type, arg_num);
			(*cur) = (*cur)->next;
		}
		else
			token_error((*cur));
		if (arg_num++ != op->args_num - 1)
		{
			if ((*cur)->type != SEPARATOR)
				token_error((*cur));
			(*cur) = (*cur)->next;
		}
	}
	return (types_code);
}

static void		process_operator(t_parser *parser, t_token **cur)
{
	t_op	*op;
	int8_t	types_code;

	if ((op = get_op((*cur)->data)))
	{
		parser->code[parser->pos++] = op->code;
		(*cur) = (*cur)->next;
		if (op->args_types_code)
			parser->pos++;
		types_code = process_args(parser, cur, op);
		if (op->args_types_code)
			parser->code[parser->op_pos + 1] = types_code;
	}
	else
		operator_error((*cur));
}

void			process_asm_code(t_parser *parser, t_token **current)
{
	while ((*current)->type != END)
	{
		if (parser->pos >= parser->code_size)
			update_code_buff(parser);
		parser->op_pos = parser->pos;
		if ((*current)->type == LABEL)
		{
			process_label(parser, current);
			(*current) = (*current)->next;
		}
		if ((*current)->type == OPERATOR)
			process_operator(parser, current);
		if ((*current)->type == NEW_LINE)
			(*current) = (*current)->next;
		else
			token_error((*current));
	}
}
