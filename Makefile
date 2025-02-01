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

# Main target
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)Creating  -> $(RED)$(NAME)$(NO_COLOR)"
	@$(MAKE) minipd

# Object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

all: $(NAME)

# ASCII art
minipd:
	@echo '                       t    EW:        ,ft  t'
	@echo '           ..       :  Ej   E##;       t#E  Ej'
	@echo '          ,W,     .Et  E#,  E###t      t#E  E#,'
	@echo '         t##,    ,W#t  E#t  E#fE#f     t#E  E#t'
	@echo '        L###,   j###t  E#t  E#t D#G    t#E  E#t'
	@echo '      .E#j##,  G#fE#t  E#t  E#t  f#E.  t#E  E#t'
	@echo '     ;WW; ##,:K#i E#t  E#t  E#t   t#K: t#E  E#t'
	@echo '    j#E.  ##f#W,  E#t  E#t  E#t    ;#W,t#E  E#t'
	@echo '  .D#L    ###K:   E#t  E#t  E#t     :K#D#E  E#t'
	@echo ' :K#t     ##D.    E#t  E#t  E#t      .E##E  E#t'
	@echo ' ...      #G      ..   E#t  ..         G#E  E#t'
	@echo '          j            ,;.              fE  ,;.'

.PHONY: clean fclean re all minipd