/* Inception BWAAAAAAH */

#include "test.h"

TEST(test_all_empty) {
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		test_free();
		test_all();
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
	}
	return TEST_SUCCESS;

}

TEST(test_group_unknown) {
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		test_group("Unknown");
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
	}
	return TEST_SUCCESS;
}

TEST(test_result) {
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		print_result("Test", 10, 0);
		print_result("Test", 9, 1);
		print_result("Test", 0, 10);
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
	}
	return TEST_SUCCESS;
}

TEST(test_false) {
	TEST_ASSERT(false, "Nope.");
	return TEST_SUCCESS;
}

TEST(test_group_fail) {
	int			st, fd[2];
	pid_t		pid;

	pipe(fd);
	if ((pid = fork()) == 0) {
		DUP_ALL_OUTPUTS(fd);
		reg_test("group_fail", test_false);
		test_all();
		test_free();
		exit(0);
	} else {
		WAIT_AND_CLOSE(pid, st, fd);
	}
	return TEST_SUCCESS;
}

void	register_tests_tests(void) {
	test_test_all_empty();
	test_test_group_unknown();
	test_test_result();
	test_test_group_fail();
}
