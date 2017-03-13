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

TEST(test_fail_malloc) {
	set_malloc_fail(1);

	char	*ret = malloc(10);
	TEST_ASSERT(ret != NULL, "Malloc should have succeed (Or we're very unlucky)");
	free(ret);
	ret = malloc(10);
	TEST_ASSERT(ret == NULL, "Malloc should have failed.");
	return TEST_SUCCESS;
}

# define TMP_FD_FN		"/tmp/test_fake_functions"

TEST(test_fail_write) {
	int		fd = open(TMP_FD_FN, O_CREAT | O_WRONLY, 0655);

	set_write_fail(1);
	TEST_ASSERT(fd != -1, "Cannot open test file");
	TEST_ASSERT(write(fd, "0", 1) != -1, "Write should have succeed (Or we're very unlucky)");
	TEST_ASSERT(write(fd, "0", 1) == -1, "Write should have failed.");
	close(fd);
	return TEST_SUCCESS;
}

TEST(test_fail_read) {
	int		fd = open(TMP_FD_FN, O_CREAT | O_RDONLY, 0655);
	char	buf[10];
	set_read_fail(1);

	TEST_ASSERT(fd != -1, "Cannot open test file");
	TEST_ASSERT(read(fd, buf, 10) != -1, "Read should have succeed (Or we're very unlucky)");
	TEST_ASSERT(read(fd, buf, 10) == -1, "Read should have failed.");
	close(fd);
	return TEST_SUCCESS;
}

TEST(test_fail_close) {
	int		fd = open(TMP_FD_FN, O_CREAT | O_RDONLY, 0655);
	set_close_fail(1);

	TEST_ASSERT(fd != -1, "Cannot open test file");
	TEST_ASSERT(close(fd) != -1, "Close should have succeed (Or we're very unlucky)");
	fd = open(TMP_FD_FN, O_CREAT | O_RDONLY, 0655);
	TEST_ASSERT(fd != -1, "Cannot open test file");
	TEST_ASSERT(close(fd) == -1, "Read should have failed.");
	/* Actually close the file */
	close(fd);
	return TEST_SUCCESS;
}

TEST(test_fail_cleanup) {
	unlink(TMP_FD_FN);
	return TEST_SUCCESS;
}


void	register_tests_tests(void) {
	test_test_all_empty();
	test_test_group_unknown();
	test_test_result();
	test_test_group_fail();
	reg_test("fake_functions", test_fail_malloc);
	reg_test("fake_functions", test_fail_write);
	reg_test("fake_functions", test_fail_read);
	reg_test("fake_functions", test_fail_close);
	reg_test("fake_functions", test_fail_cleanup);
}
