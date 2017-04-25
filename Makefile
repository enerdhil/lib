################################### LICENSE ####################################
#                            Copyright 2016 Morphux                            #
#                                                                              #
#        Licensed under the Apache License, Version 2.0 (the "License");       #
#        you may not use this file except in compliance with the License.      #
#                  You may obtain a copy of the License at                     #
#                                                                              #
#                 http://www.apache.org/licenses/LICENSE-2.0                   #
#                                                                              #
#      Unless required by applicable law or agreed to in writing, software     #
#       distributed under the License is distributed on an "AS IS" BASIS,      #
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  #
#        See the License for the specific language governing permissions and   #
#                       limitations under the License.                         #
################################################################################

# Names and binaries
NAME =		libmorphux.a
CC =		gcc
LIB =		ar

# Flags
IFLAGS =	-I inc/
SFLAGS =	-Wall -Wextra -Werror -Wno-unused-result
OFLAGS =	-std=gnu99 -g -O3
LFLAGS =	-cq

CFLAGS =	$(IFLAGS) $(SFLAGS) $(OFLAGS)

# Sources
SRCS =		$(wildcard src/*.c)
OBJS =		$(SRCS:%.c=%.o)

# Coverage flags
OSTYPE =	$(shell uname)
ifeq ($(OSTYPE), Linux)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -I inc/ -std=gnu99 -g -O0 -coverage -lgcov -DCOMPILE_WITH_TEST"
else ifeq ($(OSTYPE), Darwin)
COVFLAGS =	"-Wall -Wextra -Wno-unused-result -I inc/ -std=gnu99 -g -O0 -coverage -DCOMPILE_WITH_TEST"
endif

all: $(NAME)

%.o: %.c
	@echo "CC\t\t$@"
	@$(CC) $(CFLAGS) -c -o $@ $^

$(NAME): $(OBJS)
	@$(LIB) $(LFLAGS) $(NAME) $(OBJS)
	@echo "CCLD\t\t$(NAME)"

check: all
	$(MAKE) fclean all CFLAGS="$(CFLAGS) -Wno-error -DCOMPILE_WITH_TEST -DDEBUG -DDEBUG_FULL -ldl"
	make -C tests re check

doc:
	doxygen docs/doxyfile

coverage:
	$(MAKE) fclean all CFLAGS=$(COVFLAGS)
	make -C tests coverage check
	gcov -o src/ $(SRCS)

clean:
	rm -f $(OBJS)
	rm -f *.gcov
	rm -f src/*.gcno
	rm -f src/*.gcda

fclean: clean
	rm -f $(NAME)

test:
	$(MAKE) fclean all CFLAGS="$(CFLAGS) -DCOMPILE_WITH_TEST -Wno-error -DDEBUG_FULL"

re: fclean all

.PHONY: fclean clean all
