NAME = asm

.PHONY: all clean fclean re

ASM_DIR = ./src/asm
#как сделать короче?
SRC = ./src/asm/asm.c
#как создавать О файлы в той же папке?

LIB_DIR = ./ft_printf/
LIBFT = $(LIB_DIR)libftprintf.a

FLAGS = -g -Wall -Wextra -Werror

OBJ = $(patsubst %.c,%.o,$(SRC))

INCLUDE = ./includes/asm.h

all: $(NAME)

%.o: %.c $(INCLUDE)
	gcc $(FLAGS) -c $<

$(NAME): $(OBJ)
	$(MAKE) -C $(LIB_DIR)
	gcc -o $(NAME) $(FLAGS) $(OBJ) $(LIBFT)

clean:
	@$(MAKE) clean -C $(LIBFT)
	@/bin/rm -f $(OBJ)

fclean: clean
	@$(MAKE) fclean -C $(LIBFT)
	@/bin/rm -f $(NAME)
	
re: fclean all
