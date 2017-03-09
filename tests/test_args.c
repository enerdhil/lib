#include "test.h"

static struct margs_tests	args;

TEST(args_NULL) {
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

TEST(args_empty_1) {
	mopts_t	t;
	char	*tab[] = {"", "", ""};
	mlist_t	*lst = NULL;;

	TEST_ASSERT(read_opt(3, tab, &t, &lst) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_empty_2) {
	mopts_t	t;
	char	*tab[] = {NULL, NULL, NULL};
	mlist_t	*lst = NULL;;

	TEST_ASSERT(read_opt(3, tab, &t, &lst) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_unhandled_1) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./tests", "oui", "--allow"};
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

TEST(args_unhandled_2) {
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

TEST(args_unhandled_3) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-", "<-", "Single dash"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling alone '-' in arguments");
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	list_free(lst, NULL);
	return TEST_SUCCESS;
}

TEST(args_unhandled_4) {
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

TEST(args_unhandled_5) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling double dash without option");
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_unhandled_6) {
	mopts_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-"};
	int			st, fd[2];
	pid_t		pid;
	mlist_t		*lst = NULL;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 0,
					"Not handling single dash without option");
		exit(1);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_help_1) {
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

TEST(args_help_2) {
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

TEST(args_version_1) {
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

TEST(args_version_2) {
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

TEST(args_base_1) {
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

TEST(args_base_2) {
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

TEST(args_base_3) {
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

TEST(args_base_4) {
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

TEST(args_base_5) {
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

TEST(args_base_6) {
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

TEST(args_base_7) {
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

TEST(args_missing_value_1) {
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

TEST(args_missing_value_2) {
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

TEST(args_value_1) {
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

TEST(args_value_2) {
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

TEST(args_value_3) {
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

TEST(args_value_4) {
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

TEST(args_word_only_1) {
	mopts_t		opt[] = {
		{0, "qwerty", "qwerty", false, &callback_q},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--qwerty"};
	mlist_t		*lst = NULL;

	reset_args();
	TEST_ASSERT(read_opt(sizeof(av) / sizeof(av[0]), av, opt, &lst) == 1,
				"Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == false, "Wrong argument");
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
	reg_test("m_args", args_NULL);
	reg_test("m_args", args_empty_1);
	reg_test("m_args", args_empty_2);
	reg_test("m_args", args_unhandled_1);
	reg_test("m_args", args_unhandled_2);
	reg_test("m_args", args_unhandled_3);
	reg_test("m_args", args_unhandled_4);
	reg_test("m_args", args_unhandled_5);
	reg_test("m_args", args_unhandled_6);
	reg_test("m_args", args_help_1);
	reg_test("m_args", args_help_2);
	reg_test("m_args", args_version_1);
	reg_test("m_args", args_version_2);
	reg_test("m_args", args_base_1);
	reg_test("m_args", args_base_2);
	reg_test("m_args", args_base_3);
	reg_test("m_args", args_base_4);
	reg_test("m_args", args_base_5);
	reg_test("m_args", args_base_6);
	reg_test("m_args", args_base_7);
	reg_test("m_args", args_missing_value_1);
	reg_test("m_args", args_missing_value_2);
	reg_test("m_args", args_value_1);
	reg_test("m_args", args_value_2);
	reg_test("m_args", args_value_3);
	reg_test("m_args", args_value_4);
	reg_test("m_args", args_word_only_1);
}
