NAME =		test
CC =		gcc
CFLAGS =	-Wall -Wextra -Wno-unused-result -g -O3 -I../inc -DDEBUG #-finstrument-functions -export-dynamic
LDFLAGS = -lmorphux -ldl
SRCS =		$(wildcard *.c)
OBJS =		$(SRCS:%.c=%.o)

OSTYPE =	$(shell uname)
ifeq ($(OSTYPE), Linux)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -g -O0 -I../inc -coverage -lgcov"
else ifeq ($(OSTYPE), Darwin)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -g -O0 -I../inc -coverage"
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L../ $(LDFLAGS)

check: $(NAME)
	./$(NAME)

valgrind: $(NAME)
	valgrind --leak-check=full --error-exitcode=1 ./$(NAME)

coverage:
	$(MAKE) fclean all CFLAGS=$(COVFLAGS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f *.gcov
	rm -f *.gcno
	rm -f *.gcda

re: fclean all
