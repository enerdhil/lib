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
	fflush(stdout);
	margs_t		opt[] = {
		{'z', "--zoiberg", "No idea.", false, NULL},
		ARGS_EOL
	};
	char		*av[] = {"./tests", "oui", "--allow"};
	pid_t		pid;
	int			st, fd[2];

	pipe(fd);

	if ((pid = fork()) == 0) {

		dup2(fd[1], 1);
		dup2(fd[1], 2);
		close(fd[1]);

		TEST_ASSERT(read_opt(sizeof(av), av, opt) == 0, "Not handling properly unknown arguments");
	} else {
		pid = waitpid(pid, &st, 0);
		close(fd[1]);
		TEST_ASSERT((WEXITSTATUS(st) == 1), "Wrong return");
	}
	return TEST_SUCCESS;
}


void		reset_args(void) {
	args.opt_q = false;
	args.opt_w = false;
	args.opt_e = false;
	args.opt_r = false;
	args.opt_t = false;
	args.opt_y = false;
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
}
