NAME =		test
CC =		gcc
CFLAGS =	-Wall -Wextra -g -O3 -I../inc -lmorphux
SRCS =		$(wildcard *.c)
OBJS =		$(SRCS:%.c=%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L../ -lmorphux

check: $(NAME)
	./$(NAME)

valgrind: $(NAME)
	valgrind --leak-check=full --error-exitcode=1 ./$(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
