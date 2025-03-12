NAME = mini

# Source and include directories
SRC_DIR = src/
INC_DIR = include/

# Source files with path
SRC = $(addprefix $(SRC_DIR), env.c ft_matrix.c ft_str_utils.c ft_strings.c inizializer.c lexer_envar.c lexer.c lexerquotes.c lexerutils.c mainmini.c safefunct.c tokens.c signals.c tokenhandler.c bi_handlebi.c redirs.c parser.c command_utils.c pipes.c heredoc.c bi_export.c bi_pwd_cd.c bi_unset.c bi_echo.c pipe_executor.c exitcode.c)
OBJ = $(SRC:.c=.o)

# Compiler and flags
CC = gcc
INCLUDES = -I$(INC_DIR)
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)
LDFLAGS = -lreadline

# Colors for echo
BOLD = \033[1m
RED = \033[31m
NO_COLOR = \033[0m
ORANGE = \033[38;5;214m
YELLOW = \033[33m
GREEN = \033[32m
BLUE = \033[34m
INDIGO = \033[38;5;54m
VIOLET = \033[35m
BLINK = \033[5m

# Main target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)$(RED)❤ $(ORANGE)❤ $(YELLOW)👋 "\
	"$(RED)C$(ORANGE)r$(YELLOW)e$(GREEN)a$(BLUE)t$(INDIGO)i$(VIOLET)n$(RED)g "\
	"$(ORANGE)->"\
	" $(YELLOW)m$(GREEN)i$(BLUE)n$(INDIGO)i$(VIOLET)🐚 "\
	"$(VIOLET)👋 $(RED)❤ $(ORANGE)❤$(NO_COLOR)"

# Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

all: $(NAME) minipd

# ASCII art
minipd:
	@echo -e ' '
	@echo -e '$(RED)$(BOLD)                       t    EW:        ,ft  t$(NO_COLOR)'
	@echo -e '$(ORANGE)$(BOLD)           ..       :  Ej   E##;       t#E  Ej$(NO_COLOR)'
	@echo -e '$(YELLOW)$(BOLD)          ,W,     .Et  E#,  E###t      t#E  E#,$(NO_COLOR)'
	@echo -e '$(GREEN)$(BOLD)         t##,    ,W#t  E#t  E#fE#f     t#E  E#t$(NO_COLOR)'
	@echo -e '$(BLUE)$(BOLD)        L###,   j###t  E#t  E#t D#G    t#E  E#t$(NO_COLOR)'
	@echo -e '$(INDIGO)$(BOLD)      .E#j##,  G#fE#t  E#t  E#t  f#E.  t#E  E#t$(NO_COLOR)'
	@echo -e '$(VIOLET)$(BOLD)     ;WW; ##,:K#i E#t  E#t  E#t   t#K: t#E  E#t$(NO_COLOR)'
	@echo -e '$(RED)$(BOLD)    j#E.  ##f#W,  E#t  E#t  E#t    ;#W,t#E  E#t$(NO_COLOR)'
	@echo -e '$(ORANGE)$(BOLD)  .D#L    ###K:   E#t  E#t  E#t     :K#D#E  E#t$(NO_COLOR)'
	@echo -e '$(YELLOW)$(BOLD) :K#t     ##D.    E#t  E#t  E#t      .E##E  E#t$(NO_COLOR)'
	@echo -e '$(GREEN)$(BOLD) ...      #G      ..   E#t  ..         G#E  E#t$(NO_COLOR)'
	@echo -e '$(BLUE)$(BOLD)          j            ,;.              fE  ,;. $(NO_COLOR)'
	@echo -e ' '

.PHONY: clean fclean re all minipd