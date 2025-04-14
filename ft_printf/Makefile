NAME = libftprintf.a
SRC =   ft_printf.c \
		ft_putchr_pt.c \
		ft_putstr_pt.c \
		ft_base.c \
		ft_putptr_pt.c
		

OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -Wextra -Werror
LIBC = ar rc

$(NAME): $(OBJ)
	$(LIBC) $(NAME) $(OBJ)

all: $(NAME)

clean:
	rm -f $(OBJ) 

fclean: clean
	rm -f $(NAME)

 re:fclean all

.PHONY : all clean fclean re