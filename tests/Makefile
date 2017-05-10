NAME =		test
CC =		gcc
CFLAGS =	-Wall -Wextra -Wno-unused-result -g -O3 -I../inc -DDEBUG -std=gnu99#-finstrument-functions -export-dynamic
LDFLAGS = -lmorphux -ldl
SRCS =		$(wildcard *.c)
OBJS =		$(SRCS:%.c=%.o)

OSTYPE =	$(shell uname)
ifeq ($(OSTYPE), Linux)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -g -O0 -I../inc -coverage -lgcov -std=gnu99"
else ifeq ($(OSTYPE), Darwin)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -g -O0 -I../inc -coverage -std=gnu99"
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -L../ $(LDFLAGS)

check: $(NAME)
	mkdir -p test_dir_2 && touch test_dir_2/file && chmod a-w test_dir_2
	./$(NAME)
	chmod +w test_dir_2 && rm -rf test_dir_2

valgrind: $(NAME)
	mkdir test_dir_2 && touch test_dir_2/file && chmod a-w test_dir_2
	valgrind --leak-check=full --error-exitcode=1 ./$(NAME)
	chmod +w test_dir_2 && rm -rf test_dir_2

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
