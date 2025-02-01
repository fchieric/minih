NAME = mini

# Source and include directories
SRC_DIR = src/
INC_DIR = include/

# Source files with path
SRC = $(addprefix $(SRC_DIR), mainmini.c safefunct.c)
OBJ = $(SRC:.c=.o)

# Compiler and flags
CC = gcc
INCLUDES = -I$(INC_DIR)
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lreadline

# Colors for echo
BOLD = \033[1m
RED = \033[31m
NO_COLOR = \033[0m
RED = \033[31m
ORANGE = \033[38;5;214m
YELLOW = \033[33m
GREEN = \033[32m
BLUE = \033[34m
INDIGO = \033[38;5;54m
VIOLET = \033[35m

# Main target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)Creating  -> $(RED)$(NAME)$(NO_COLOR)"

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
	@echo -e '$(RED)                       t    EW:        ,ft  t$(NO_COLOR)'
	@echo -e '$(ORANGE)           ..       :  Ej   E##;       t#E  Ej$(NO_COLOR)'
	@echo -e '$(YELLOW)          ,W,     .Et  E#,  E###t      t#E  E#,$(NO_COLOR)'
	@echo -e '$(GREEN)         t##,    ,W#t  E#t  E#fE#f     t#E  E#t$(NO_COLOR)'
	@echo -e '$(BLUE)        L###,   j###t  E#t  E#t D#G    t#E  E#t$(NO_COLOR)'
	@echo -e '$(INDIGO)      .E#j##,  G#fE#t  E#t  E#t  f#E.  t#E  E#t$(NO_COLOR)'
	@echo -e '$(VIOLET)     ;WW; ##,:K#i E#t  E#t  E#t   t#K: t#E  E#t$(NO_COLOR)'
	@echo -e '$(RED)    j#E.  ##f#W,  E#t  E#t  E#t    ;#W,t#E  E#t$(NO_COLOR)'
	@echo -e '$(ORANGE)  .D#L    ###K:   E#t  E#t  E#t     :K#D#E  E#t$(NO_COLOR)'
	@echo -e '$(YELLOW) :K#t     ##D.    E#t  E#t  E#t      .E##E  E#t$(NO_COLOR)'
	@echo -e '$(GREEN) ...      #G      ..   E#t  ..         G#E  E#t$(NO_COLOR)'
	@echo -e '$(BLUE)          j            ,;.              fE  ,;. $(NO_COLOR)'
	@echo -e ' '

.PHONY: clean fclean re all minipd