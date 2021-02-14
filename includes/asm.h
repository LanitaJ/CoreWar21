#include "../ft_printf/includes/ft_printf.h"
#include "op.h"
# include <errno.h>

# define MAX_STATEMENT_SIZE 14

# define REG_CHAR 'r'

# define COMMAND_CHAR '.'

typedef enum
{
	false,
	true
}	t_bool;

typedef enum
{
	COMMAND,
	STRING,
	LABEL,
	OPERATOR,
	REGISTER,
	DIRECT,
	DIRECT_LABEL,
	INDIRECT,
	INDIRECT_LABEL,
	SEPARATOR,
	NEW_LINE,
	END
}	t_type;

static char				*g_type[] = {
	"COMMAND",
	"STRING",
	"LABEL",
	"OPERATOR",
	"REGISTER",
	"DIRECT",
	"DIRECT_LABEL",
	"INDIRECT",
	"INDIRECT_LABEL",
	"SEPARATOR",
	"NEW_LINE",
	"END"
};

typedef struct			s_token
{
	char				*data;
	t_type				type;
	unsigned int		row;
	unsigned int		column;
	struct s_token		*next;
}						t_token;

typedef struct	s_op
{
	char		*name;
	uint8_t		code;
	uint8_t		args_num;
	t_bool 		args_types_code;
	uint8_t		args_types[3];
	uint8_t		t_dir_size;
}				t_op;

typedef struct			s_mention
{
	unsigned			row;
	unsigned			column;
	int32_t				pos;
	int32_t				op_pos;
	size_t				size;
	struct s_mention	*next;
}						t_mention;

typedef struct			s_label
{
	char				*name;
	int32_t				op_pos;
	t_mention			*mentions;
	struct s_label		*next;
}						t_label;

typedef struct			s_parser
{
	int					fd;
	char				*name;
	char				*comment;
	unsigned int		row;
	unsigned int		column;
	t_token				*tokens;
	int32_t				pos;
	int32_t				op_pos;
	char				*code;
	int32_t				code_size;
	t_label				*labels;
}						t_parser;

static t_op		g_op[16] = {
		{
				.name = "live",
				.code = 0x01,
				.args_num = 1,
				.args_types_code = false,
				.args_types = {T_DIR, 0, 0},
				.t_dir_size = 4,
		},
		{
				.name = "ld",
				.code = 0x02,
				.args_num = 2,
				.args_types_code = true,
				.args_types = {T_DIR | T_IND, T_REG, 0},
				.t_dir_size = 4,
		},
		{
				.name = "st",
				.code = 0x03,
				.args_num = 2,
				.args_types_code = true,
				.args_types = {T_REG, T_REG | T_IND, 0},
				.t_dir_size = 4,
		},
		{
				.name = "add",
				.code = 0x04,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG, T_REG, T_REG},
				.t_dir_size = 4,
		},
		{
				.name = "sub",
				.code = 0x05,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG, T_REG, T_REG},
				.t_dir_size = 4,
		},
		{
				.name = "and",
				.code = 0x06,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG | T_DIR | T_IND, T_REG | T_DIR | T_IND, T_REG},
				.t_dir_size = 4,
		},
		{
				.name = "or",
				.code = 0x07,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG | T_DIR | T_IND, T_REG | T_DIR | T_IND, T_REG},
				.t_dir_size = 4,
		},
		{
				.name = "xor",
				.code = 0x08,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG | T_DIR | T_IND, T_REG | T_DIR | T_IND, T_REG},
				.t_dir_size = 4,
		},
		{
				.name = "zjmp",
				.code = 0x09,
				.args_num = 1,
				.args_types_code = false,
				.args_types = {T_DIR, 0, 0},
				.t_dir_size = 2,
		},
		{
				.name = "ldi",
				.code = 0x0A,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG | T_DIR | T_IND, T_REG | T_DIR, T_REG},
				.t_dir_size = 2,
		},
		{
				.name = "sti",
				.code = 0x0B,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG, T_REG | T_DIR | T_IND, T_REG | T_DIR},
				.t_dir_size = 2,
		},
		{
				.name = "fork",
				.code = 0x0C,
				.args_num = 1,
				.args_types_code = false,
				.args_types = {T_DIR, 0, 0},
				.t_dir_size = 2,
		},
		{
				.name = "lld",
				.code = 0x0D,
				.args_num = 2,
				.args_types_code = true,
				.args_types = {T_DIR | T_IND, T_REG, 0},
				.t_dir_size = 4,
		},
		{
				.name = "lldi",
				.code = 0x0E,
				.args_num = 3,
				.args_types_code = true,
				.args_types = {T_REG | T_DIR | T_IND, T_REG | T_DIR, T_REG},
				.t_dir_size = 2,
		},
		{
				.name = "lfork",
				.code = 0x0F,
				.args_num = 1,
				.args_types_code = false,
				.args_types = {T_DIR, 0, 0},
				.t_dir_size = 2,
		},
		{
				.name = "aff",
				.code = 0x10,
				.args_num = 1,
				.args_types_code = true,
				.args_types = {T_REG, 0, 0},
				.t_dir_size = 4,
		}
};

t_parser				*init_parser(int fd);
void					parse_asm(t_parser *parser);
t_token					*init_token(t_parser *parser, t_type type);
void					add_token(t_token **chain, t_token *new);
void					skip_whitespaces(const char *row, t_parser *parser);
void					skip_comment(t_parser *parser, const char *row);
int						is_register(const char *arg);
int						is_delimiter(int c);
int						is_whitespace(int c);
void					error_fd(void);
void					error_malloc(void);
void					kill(char *s);
char					*token_content(t_parser *parser, const char *row, unsigned start);
int						get_row(const int fd, char **row);

void					error_lex(t_parser *parser);
void					upgrade_row(char **row, char *ptr);

t_label					*init_label(char *name, int op_pos);
t_mention				*init_mention(t_parser *parser, t_token *token, size_t size);

void					process_info(t_parser *parser, t_token **cur);
void					process_asm_code(t_parser *parser, t_token **cur);
int8_t					process_arg(t_parser *parser, t_token **current, t_op *op, int arg_num);
void					replace_mentions(t_parser *file);
void					int32_to_bytecode(char *data, int32_t pos, int32_t value, size_t size);
void					write_bytecode_file(int fd, t_parser *parser);
void					update_types_code(int8_t *types_code, int8_t type, int arg_num);
t_op					*get_op(char *name);
void					update_code_buff(t_parser *parser);
void					add_label(t_label **list, t_label *new);
void					add_mention(t_mention **list, t_mention *new);
t_label					*find_label(t_label *list, char *name);

void					label_error(t_label *label);
void					token_error(t_token *token);
void					operator_error(t_token *token);
void					name_error();
void					comment_error();
void					arg_type_error(t_op *op, int arg_num, t_token *token);

char					*ft_strchrs(const char *s, int c);