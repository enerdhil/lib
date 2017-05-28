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

TEST(test_fail_calloc) {

    char    *ret = calloc(1, 10);
    TEST_ASSERT(ret != NULL, "calloc should have succeed (Or we're very unlucky)");
    free(ret);

    set_calloc_fail(1);
    ret = calloc(1, 10);
    TEST_ASSERT(ret != NULL, "calloc should have succeed (Or we're very unlucky)");
    free(ret);
    ret = calloc(1, 10);
    TEST_ASSERT(ret == NULL, "calloc should have failed")
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

TEST(test_fail_strdup) {
    char *ptr;
    set_strdup_fail(1);

    ptr = strdup("Test");
    TEST_ASSERT(ptr != NULL, "Strdup failed (Or we're very unlucky)");
    free(ptr);
    ptr = strdup("Test");
    TEST_ASSERT(ptr == NULL, "Function should have not succeed");
    ptr = strdup("Test");
    TEST_ASSERT(ptr != NULL, "Strdup failed (Or we're very unlucky)");
    free(ptr);
    return TEST_SUCCESS;
}

TEST(test_fail_fstat) {
    int     fd = open("./Makefile", O_RDONLY);
    struct stat     buf;
    int     ret;

    set_fstat_fail(1);
    ret = fstat(fd, &buf);
    TEST_ASSERT(ret != -1, "fstat failed");
    ret = fstat(fd, &buf);
    TEST_ASSERT(ret == -1, "fstat succeed");
    ret = fstat(fd, &buf);
    TEST_ASSERT(ret != -1, "fstat failed");
    close(fd);
    return TEST_SUCCESS;
}

TEST(test_fail_stat) {
    struct stat     buf;
    int             ret;

    set_stat_fail(1);
    ret = stat("Makefile", &buf);
    TEST_ASSERT(ret != -1, "stat failed");
    ret = stat("Makefile", &buf);
    TEST_ASSERT(ret == -1, "stat succeed");
    ret = stat("Makefile", &buf);
    TEST_ASSERT(ret != -1, "stat failed");

    return TEST_SUCCESS;
}


TEST(test_fail_strcpy) {
    const char  *src = "Hello !";
    char        dst[sizeof(src)];

    TEST_ASSERT((strcpy(dst, src) != NULL), "Should have succeed");
    set_strcpy_fail(1);
    TEST_ASSERT((strcpy(dst, src) != NULL), "Should have succeed");
    TEST_ASSERT((strcpy(dst, src) == NULL), "Should have failed");

    return TEST_SUCCESS;
}

TEST(test_fail_strcat) {
    const char  *src = "Hello !";
    char        dst[sizeof(src) + 5];

    strcpy(dst, "Test");
    TEST_ASSERT((strcat(dst, src) != NULL), "Should have succeed");
    set_strcat_fail(1);
    strcpy(dst, "Test");
    TEST_ASSERT((strcat(dst, src) != NULL), "Should have succeed");
    strcpy(dst, "Test");
    TEST_ASSERT((strcat(dst, src) == NULL), "Should have failed");

    return TEST_SUCCESS;
}

TEST(test_fail_mkdir) {
    const char *dir = "/tmp/test_fail_mkdir";

    TEST_ASSERT(mkdir(dir, S_IRWXU) != -1, "Should have succeed")
    rmdir(dir);

    set_mkdir_fail(1);
    TEST_ASSERT(mkdir(dir, S_IRWXU) != -1, "Should have succeed")
    rmdir(dir);

    TEST_ASSERT(mkdir(dir, S_IRWXU) == -1, "Should have failed")
    return TEST_SUCCESS;
}

TEST(test_fail_fork) {
    pid_t       ret;
    int         status;

    ret = fork();
    if (ret == 0)
    {
        _exit(0);
    }
    else if (ret == -1)
    {
        TEST_ASSERT(ret != -1, "Should have succeed");
    }
    else
    {
        waitpid(ret, &status, 0);
    }

    set_fork_fail(1);
    ret = fork();
    if (ret == 0)
    {
        _exit(0);
    }
    else if (ret == -1)
    {
        TEST_ASSERT(ret != -1, "Should have succeed");
    }
    else
    {
        waitpid(ret, &status, 0);
    }

    ret = fork();
    TEST_ASSERT(ret == -1, "Should have failed");
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
	reg_test("fake_functions", test_fail_calloc);
	reg_test("fake_functions", test_fail_write);
	reg_test("fake_functions", test_fail_read);
	reg_test("fake_functions", test_fail_close);
	reg_test("fake_functions", test_fail_strdup);
	reg_test("fake_functions", test_fail_fstat);
	reg_test("fake_functions", test_fail_stat);
	reg_test("fake_functions", test_fail_strcpy);
	reg_test("fake_functions", test_fail_strcat);
	reg_test("fake_functions", test_fail_cleanup);
	reg_test("fake_functions", test_fail_mkdir);
        reg_test("fake_functions", test_fail_fork);
}
