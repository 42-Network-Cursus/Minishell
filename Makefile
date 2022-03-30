NAME		=	minishell

PARS		=	main.c \
				parsing/parser.c \
				parsing/ft_strtok.c \
				parsing/ft_strtok_utils.c \
				parsing/utils.c \
				parsing/parser_utils.c \
				parsing/check_env_var.c \
				parsing/bin_check.c

EXE			=	exec/exec.c\
				exec/pipes.c

BIN			=	builtins/echo.c \
				builtins/cd.c \
				builtins/export_unset.c \
				builtins/pwd_env.c \
				builtins/utils.c 
				
LIBFT		= ./libft/

NAMELFT		= ./libft/libft.a

OBJS		=	$(PARS:c=o) $(EXE:c=o) $(BIN:c=o)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror  -g3 \
				-fsanitize=address -g

INC			= 	-I./inc/


all:			$(OBJS) $(NAME)

%.o:		%.c
				@echo "Compiling $<"
				@$(CC) $(CFLAGS) -c $(INC) $< -o $(<:c=o)
				@echo "Compiling OK!"

$(NAME):	$(OBJS)
				@make all -s -C ./libft
				@echo "Linking $^"
				@cc $(CFLAGS) ${NAMELFT} -lreadline -L/Users/$(USER)/.brew/opt/readline/lib -I/Users/$(USER)/.brew/opt/include/readline/ -ltermcap $^ -o $@ 
				@echo "Executable created!"

clean:
				@echo "Cleaning objects..."
				@rm -f $(OBJS) $(BONUS_OBJS)
				@echo "Cleaned up!"

fclean:		clean
				@make fclean -s -C ./libft
				@echo "Removing executable..."
				@rm -f $(NAME)
				@echo "Removed!"

re:			fclean all

.PHONY:		all clean fclean re