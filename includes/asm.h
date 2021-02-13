#include "../ft_printf/includes/ft_printf.h"
#include "op.h"
# include <errno.h>

# define MAX_STATEMENT_SIZE 14

# define REG_CHAR 'r'

# define COMMAND_CHAR '.'

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

typedef struct			s_parser
{
		int				fd;
		char			*name;
		char			*comment;
		unsigned int	row;
		unsigned int	column;
		t_token			*token;
}						t_parser;

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

char					*ft_strchrs(const char *s, int c);