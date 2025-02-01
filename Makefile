NAME = mini

SRC = mainmini.c safefunct.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g $(INCLUDES)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lreadline

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LDFLAGS)
	echo "${BOLD}Creating  -> ${RED}$(NAME)${NO_COLOR}"
	${MAKE} minipd

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

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
