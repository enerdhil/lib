#include "test.h"

static struct margs_tests	args;

TEST(args_NULL) {
	TEST_ASSERT(read_opt(0, NULL, NULL) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_NULL_1) {
	TEST_ASSERT(read_opt(10, NULL, NULL) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_NULL_2) {
	char	*tab[] = {"test", "test2"};
	TEST_ASSERT(read_opt(0, tab, NULL) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_NULL_3) {
	char	*tab[] = {"test", "test2"};
	TEST_ASSERT(read_opt(10, tab, NULL) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_NULL_4) {
	char	*tab[] = {"test", "test2"};
	margs_t	t;

	TEST_ASSERT(read_opt(0, tab, &t) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_NULL_5) {
	margs_t	t;

	TEST_ASSERT(read_opt(10, NULL, &t) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_empty_1) {
	margs_t	t;
	char	*tab[] = {"", "", ""};

	TEST_ASSERT(read_opt(3, tab, &t) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}

TEST(args_empty_2) {
	margs_t	t;
	char	*tab[] = {NULL, NULL, NULL};

	TEST_ASSERT(read_opt(3, tab, &t) == 0, "Not handling null arguments");
	return TEST_SUCCESS;
}


TEST(args_unhandled_1) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./tests", "oui", "--allow"};
	pid_t		pid;
	int			st, fd[2];

	pipe(fd);
	fflush(stdout);
	if ((pid = fork()) == 0) {

		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling properly unknown arguments");
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_unhandled_2) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "---wrong-option"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling triple '-' arguments");
		_exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_unhandled_3) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-", "<-", "Single dash"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling alone '-' in arguments");
		_exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_help_1) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-h"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling -h arguments");
		_exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_help_2) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--help"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling --help arguments");
		_exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_version_1) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "-v"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling --version arguments");
		_exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_version_2) {
	margs_t		opt[] = {
		{'z', "zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./test", "--version"};
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling -v arguments");
		_exit(5);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
		TEST_ASSERT((WEXITSTATUS(st) == 0), "Wrong return");
	}
	return TEST_SUCCESS;
}

TEST(args_base_1) {
	margs_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-q"};

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt) == 1), "Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w != true, "Argument not read");
	return TEST_SUCCESS;
}

TEST(args_base_2) {
	margs_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-qw"};

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt) == 2), "Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
	return TEST_SUCCESS;
}

TEST(args_base_3) {
	margs_t		opt[] = OPT_DEF(false);
	char		*av[] = {"./test", "-q", "-w"};

	reset_args();
	TEST_ASSERT((read_opt(sizeof(av) / sizeof(av[0]), av, opt) == 2), "Wrong return");
	TEST_ASSERT(args.opt_q == true, "Argument not read");
	TEST_ASSERT(args.opt_w == true, "Argument not read");
	TEST_ASSERT(args.opt_e != true, "Argument not read");
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
	reg_test("m_args", args_NULL_1);
	reg_test("m_args", args_NULL_2);
	reg_test("m_args", args_NULL_3);
	reg_test("m_args", args_NULL_4);
	reg_test("m_args", args_NULL_5);
	reg_test("m_args", args_empty_1);
	reg_test("m_args", args_empty_2);
	reg_test("m_args", args_unhandled_1);
	reg_test("m_args", args_unhandled_2);
	reg_test("m_args", args_unhandled_3);
	reg_test("m_args", args_help_1);
	reg_test("m_args", args_help_2);
	reg_test("m_args", args_version_1);
	reg_test("m_args", args_version_2);
	reg_test("m_args", args_base_1);
	reg_test("m_args", args_base_2);
	reg_test("m_args", args_base_3);
}
