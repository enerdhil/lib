#include "test.h"

static struct margs_tests	args;

/* Testing all possibilities with a NULL args somewhere */
TEST(opts_NULL) {
	char	*tab[] = {"test", "test2"};
	mopts_t	t;
	mlist_t	*lst = NULL;;

	/* 0 0 0 0 */
	TEST_ASSERT(read_opt(0, NULL, NULL, NULL) == 0,
				"Not handling null arguments");
	/* 0 0 0 1 */
	TEST_ASSERT(read_opt(0, NULL, NULL, &lst) == 0,
				"Not handling null arguments");
	/* 0 0 1 0 */
	TEST_ASSERT(read_opt(0, NULL, &t, NULL) == 0,
				"Not handling null arguments");
	/* 0 0 1 1 */
	TEST_ASSERT(read_opt(0, NULL, &t, &lst) == 0,
				"Not handling null arguments");
	/* 0 1 0 0 */
	TEST_ASSERT(read_opt(0, tab, NULL, NULL) == 0,
				"Not handling null arguments");
	/* 0 1 0 1 */
	TEST_ASSERT(read_opt(0, tab, NULL, &lst) == 0,
				"Not handling null arguments");
	/* 0 1 1 0 */
	TEST_ASSERT(read_opt(0, tab, &t, NULL) == 0,
				"Not handling null arguments");
	/* 0 1 1 1 */
	TEST_ASSERT(read_opt(0, tab, &t, &lst) == 0,
				"Not handling null arguments");
	/* 1 0 0 0 */
	TEST_ASSERT(read_opt(10, NULL, NULL, NULL) == 0,
				"Not handling null arguments");
	/* 1 0 0 1 */
	TEST_ASSERT(read_opt(10, NULL, NULL, &lst) == 0,
				"Not handling null arguments");
	/* 1 0 1 0 */
	TEST_ASSERT(read_opt(10, NULL, &t, NULL) == 0,
				"Not handling null arguments");
	/* 1 0 1 1 */
	TEST_ASSERT(read_opt(10, NULL, &t, &lst) == 0,
				"Not handling null arguments");
	/* 1 1 0 0 */
	TEST_ASSERT(read_opt(10, tab, NULL, NULL) == 0,
				"Not handling null arguments");
	/* 1 1 0 1 */
	TEST_ASSERT(read_opt(10, tab, NULL, &lst) == 0,
				"Not handling null arguments");
	/* 1 1 1 0 */
	TEST_ASSERT(read_opt(10, tab, &t, NULL) == 0,
				"Not handling null arguments");

	return TEST_SUCCESS;
}

/* Testing with av being an array full of empty strings */
TEST(opts_empty_1) {
	mopts_t	t;
	char	*tab[] = {"", "", ""};
	mlist_t	*lst = NULL;;

	TEST_ASSERT(read_opt(3, tab, &t, &lst) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

/* Testing with av being an array full of NULLs */
TEST(opts_empty_2) {
	mopts_t	t;
	char	*tab[] = {NULL, NULL, NULL};
	mlist_t	*lst = NULL;;

	TEST_ASSERT(read_opt(3, tab, &t, &lst) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

/* Testing with unknown single dash option */
TEST(opts_unhandled_1) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-q"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling unknown option");
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing with unknown double dash option */
TEST(opts_unhandled_2) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./tests", "--oui"};
	pid_t		pid;
	int			st, fd[2];
	mlist_t		*lst = NULL;;

	pipe(fd);
	fflush(stdout);
	if ((pid = fork()) == 0) {

		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt, &lst) == 0, 
					"Not handling properly unknown arguments");
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing with triple dash */
TEST(opts_unhandled_3) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "---wrong-option"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling triple '-' arguments");
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing a call to -h builtin option */
TEST(opts_help_1) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-h"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling -h arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing a call to --help builtin option */
TEST(opts_help_2) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--help"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling --help arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing that --help properly exits instantly */
TEST(opts_help_3) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--help", "--zoiberg"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling --help arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing a call to -V builtin option */
TEST(opts_version_1) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-V"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling --version arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing a call to --version builtin option */
TEST(opts_version_2) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--version"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling -v arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing that --version properly exit instantly */
TEST(opts_version_3) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--version", "--zoiberg"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling -v arguments");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing reading of 1 option without parameters */
TEST(opts_base_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-q"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w != true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading of 2 packed options without parameters */
TEST(opts_base_2) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-qw"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading of 2 separated options without parameters */
TEST(opts_base_3) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-q", "-w"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading of 1 long and 1 short options without parameters */
TEST(opts_base_4) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "--qwerty", "-w"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading of 2 long options without parameters */
TEST(opts_base_5) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "--qwerty", "--wertyu"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading 6 packed options that makes the name of a long option */
TEST(opts_base_6) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-qwerty"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 6),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e == true, "Argument not read");
	TEST_ASSERT(args.opt_r == true, "Argument not read");
	TEST_ASSERT(args.opt_t == true, "Argument not read");
	TEST_ASSERT(args.opt_y == true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing reading of 6 long options without parameters */
TEST(opts_base_7) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test",
						"--qwerty",
						"--wertyu",
						"--ertyui",
						"--rtyuio",
						"--tyuiop",
						"--yuiop["};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 6),
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e == true, "Argument not read");
	TEST_ASSERT(args.opt_r == true, "Argument not read");
	TEST_ASSERT(args.opt_t == true, "Argument not read");
	TEST_ASSERT(args.opt_y == true, "Argument not read");
	return TEST_SUCCESS;
}

/* Testing missing argument in short options-argument pair */
TEST(opts_missing_value_1) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "-q"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	reset_args();
	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling missing argument");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing missing argument in long options-argument pair */
TEST(opts_missing_value_2) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "--qwerty"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	reset_args();
	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling missing argument");
		exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

/* Testing good reading of short options-argument pair */
TEST(opts_value_1) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "-q", "toto"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_q, "toto") == 0, "Value not read");
	return TEST_SUCCESS;
}

/* Testing good reading of long options-argument pair */
TEST(opts_value_2) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "--qwerty=toto"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_q, "toto") == 0, "Value not read");
	return TEST_SUCCESS;
}

/* Mixing both tests above */
TEST(opts_value_3) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "--qwerty=toto", "-w", "tata"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_q, "toto") == 0, "Value not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_w, "tata") == 0, "Value not read");
	return TEST_SUCCESS;
}

/* Testing good reading of multiple long options-argument pair */
TEST(opts_value_4) {
	mopts_t		opt[] = OPT_DEF(true);
	char		*av[] = {"./test", "--qwerty=toto", "--wertyu=tata"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 2,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_q, "toto") == 0, "Value not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(strcmp(args.str_w, "tata") == 0, "Value not read");
	return TEST_SUCCESS;
}

/* Testing existence of long-only options */
TEST(opts_long_only_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "--qwerty"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == false, "Wrong argument");
	return TEST_SUCCESS;
}

/* Testing existence of short-only options */
TEST(opts_short_only_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-q"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == false, "Wrong argument");
	return TEST_SUCCESS;
}

/* Testing reading of a params alone */
TEST(params_reading_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "hey"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
				"Wrong return");
	TEST_ASSERT(args.opt_q == false, "Wrong argument");
	TEST_ASSERT(lst, "List not created");
	TEST_ASSERT(lst->size == strlen(av[1]) + 1, "Parameter not read");
	TEST_ASSERT(!strcmp(lst->member, "hey"), "Parameter not read");
	return TEST_SUCCESS;
}

/* Testing reading of a params then a short option */
TEST(params_reading_2) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "hey", "-q"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Didn't parsed option");
	TEST_ASSERT(lst, "List not created");
	TEST_ASSERT(!strcmp(lst->member, "hey"), "Parameter not read");
	TEST_ASSERT(lst->size == strlen(av[1]) + 1, "Wrong size allocated");
	TEST_ASSERT(!lst->next, "Unwanted node created");
	return TEST_SUCCESS;
}

/* Testing reading of a params then a long option */
TEST(params_reading_3) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "hey", "--qwerty"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Didn't parsed option");
	TEST_ASSERT(lst, "List not created");
	TEST_ASSERT(!strcmp(lst->member, "hey"), "Parameter not read");
	TEST_ASSERT(lst->size == strlen(av[1]) + 1, "Wrong size allocated");
	TEST_ASSERT(!lst->next, "Unwanted node created");
	return TEST_SUCCESS;
}

/* Testing with empty param */
TEST(empty_param_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", ""};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
				"Wrong return");
	TEST_ASSERT(args.opt_q == false, "Wrong argument");
	TEST_ASSERT(!lst, "List created with empty param");
	return TEST_SUCCESS;
}

/* Testing with null param */
TEST(empty_param_2) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", NULL};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
				"Wrong return");
	TEST_ASSERT(args.opt_q == false, "Wrong argument");
	TEST_ASSERT(!lst, "List created with null param");
	return TEST_SUCCESS;
}

/* Testing with empty param followed by a param */
TEST(empty_param_3) {
	mopts_t		opt[] = {
		{'q', NULL, "qwerty", false, &callback_q},
		ARGS_EOL
	};
	char		*av[] = {"./test", "", "hey"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
				"Wrong return");
	TEST_ASSERT(args.opt_q == false, "Wrong argument");
	TEST_ASSERT(lst, "List not created");
	TEST_ASSERT(!strcmp(lst->member, "hey"), "Parameter not read");
	TEST_ASSERT(lst->size == strlen(av[2]) + 1, "Wrong size allocated");
	return TEST_SUCCESS;
}

/* Testing reading of parameters, options, and empty cases in av */
TEST(mixed_1) {
	mopts_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "hey", "--qwerty", "-q", "-w",
						"Ne02ptzero", "is", "--rtyuio", "", "-e"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Didn't parsed option");
	TEST_ASSERT(args.opt_w == true, "Didn't parsed option");
	TEST_ASSERT(args.opt_e == true, "Didn't parsed option");
	TEST_ASSERT(args.opt_r == true, "Didn't parsed option");
	TEST_ASSERT(args.opt_t == true, "Didn't parsed option");
	TEST_ASSERT(lst, "List not created");
	TEST_ASSERT(!strcmp(lst->member, "hey"), "Parameter not read");
	TEST_ASSERT(lst->size == strlen(av[1]) + 1, "Wrong size allocated");
	TEST_ASSERT(lst->next, "Node not created");
	TEST_ASSERT(!strcmp(lst->next->member, "Ne02ptzero"), "Parameter not read");
	TEST_ASSERT(lst->next->size == strlen(av[5]) + 1, "Wrong size allocated");
	TEST_ASSERT(lst->next->next, "Node not created");
	TEST_ASSERT(!strcmp(lst->next->next->member, "is"),
				"Parameter not read");
	TEST_ASSERT(lst->next->next->size == strlen(av[6]) + 1,
				"Wrong size allocated");
	TEST_ASSERT(!lst->next->next->next, "Unwanted node created");
	return TEST_SUCCESS;
}

void		callback_q(const char *s) {
	args.opt_q = true;
	if (s == NULL)
		strcpy(args.str_q, "");
	else
		strcpy(args.str_q, s);
}

void		callback_w(const char *s) {
	args.opt_w = true;
	if (s == NULL)
		strcpy(args.str_w, "");
	else
		strcpy(args.str_w, s);
}

void		callback_e(const char *s) {
	args.opt_e = true;
	if (s == NULL)
		strcpy(args.str_e, "");
	else
		strcpy(args.str_e, s);
}

void		callback_r(const char *s) {
	args.opt_r = true;
	if (s == NULL)
		strcpy(args.str_r, "");
	else
		strcpy(args.str_r, s);
}

void		callback_t(const char *s) {
	args.opt_t = true;
	if (s == NULL)
		strcpy(args.str_t, "");
	else
		strcpy(args.str_t, s);
}

void		callback_y(const char *s) {
	args.opt_y = true;
	if (s == NULL)
		strcpy(args.str_y, "");
	else
		strcpy(args.str_y, s);
}

void		reset_args(void) {
	args.opt_q = false;
	args.opt_w = false;
	args.opt_e = false;
	args.opt_r = false;
	args.opt_t = false;
	args.opt_y = false;
	strcpy(args.str_q, "");
	strcpy(args.str_w, "");
	strcpy(args.str_e, "");
	strcpy(args.str_r, "");
	strcpy(args.str_t, "");
	strcpy(args.str_y, "");
}

void		register_args_tests(void) {
/* Testing all possibilities with a NULL args somewhere */
	reg_test("m_args", opts_NULL);
/* Testing with av being an array full of empty strings */
	reg_test("m_args", opts_empty_1);
/* Testing with av being an array full of NULLs */
	reg_test("m_args", opts_empty_2);
/* Testing with unknown single dash option */
	reg_test("m_args", opts_unhandled_1);
/* Testing with unknown double dash option */
	reg_test("m_args", opts_unhandled_2);
/* Testing with triple dash */
	reg_test("m_args", opts_unhandled_3);
/* Testing a call to -h builtin option */
	reg_test("m_args", opts_help_1);
/* Testing a call to --help builtin option */
	reg_test("m_args", opts_help_2);
/* Testing that --help properly exits instantly */
	reg_test("m_args", opts_help_3);
/* Testing a call to -V builtin option */
	reg_test("m_args", opts_version_1);
/* Testing a call to --version builtin option */
	reg_test("m_args", opts_version_2);
/* Testing that --version properly exit instantly */
	reg_test("m_args", opts_version_3);
/* Testing reading of 1 option without parameters */
	reg_test("m_args", opts_base_1);
/* Testing reading of 2 packed options without parameters */
	reg_test("m_args", opts_base_2);
/* Testing reading of 2 separated options without parameters */
	reg_test("m_args", opts_base_3);
/* Testing reading of 1 long and 1 short options without parameters */
	reg_test("m_args", opts_base_4);
/* Testing reading of 2 long options without parameters */
	reg_test("m_args", opts_base_5);
/* Testing reading 6 packed options that makes the name of a long option */
	reg_test("m_args", opts_base_6);
/* Testing reading of 6 long options without parameters */
	reg_test("m_args", opts_base_7);
/* Testing missing argument in short options-argument pair */
	reg_test("m_args", opts_missing_value_1);
/* Testing missing argument in long options-argument pair */
	reg_test("m_args", opts_missing_value_2);
/* Testing good reading of short options-argument pair */
	reg_test("m_args", opts_value_1);
/* Testing good reading of long options-argument pair */
	reg_test("m_args", opts_value_2);
/* Mixing both tests above */
	reg_test("m_args", opts_value_3);
/* Testing good reading of multiple long options-argument pair */
	reg_test("m_args", opts_value_4);
/* Testing existence of long-only options */
	reg_test("m_args", opts_long_only_1);
/* Testing existence of short-only options */
	reg_test("m_args", opts_short_only_1);
/* Testing reading of a params alone */
	reg_test("m_args", params_reading_1);
/* Testing reading of a params then a short option */
	reg_test("m_args", params_reading_2);
/* Testing reading of a params then a long option */
	reg_test("m_args", params_reading_3);
/* Testing with empty param */
	reg_test("m_args", empty_param_1);
/* Testing with null param */
	reg_test("m_args", empty_param_2);
/* Testing with empty param followed by a param */
	reg_test("m_args", empty_param_3);
}
