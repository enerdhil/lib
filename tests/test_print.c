#include "test.h"

TEST(print_info) {
    int			st, fd[2];
    pid_t		pid;

    pipe(fd);
    if ((pid = fork()) == 0) {
        DUP_ALL_OUTPUTS(fd);
        m_info("Test");
        exit(0);
    } else {
        WAIT_AND_CLOSE(pid, st, fd);
    }
    return TEST_SUCCESS;
}

TEST(print_warning) {
    int			st, fd[2];
    pid_t		pid;

    pipe(fd);
    if ((pid = fork()) == 0) {
        DUP_ALL_OUTPUTS(fd);
        m_warning("Test");
        exit(0);
    } else {
        WAIT_AND_CLOSE(pid, st, fd);
    }
    return TEST_SUCCESS;
}

TEST(print_error) {
    int			st, fd[2];
    pid_t		pid;

    pipe(fd);
    if ((pid = fork()) == 0) {
        DUP_ALL_OUTPUTS(fd);
        m_error("Test");
        exit(0);
    } else {
        WAIT_AND_CLOSE(pid, st, fd);
    }
    return TEST_SUCCESS;
}

TEST(print_panic) {
    int			st, fd[2];
    pid_t		pid;

    pipe(fd);
    if ((pid = fork()) == 0) {
        DUP_ALL_OUTPUTS(fd);
        m_panic("Test");
        exit(0);
    } else {
        WAIT_AND_CLOSE(pid, st, fd);
    }
    return TEST_SUCCESS;
}

TEST(print_log_err) {
    TEST_ASSERT(m_log("Balec/20") == false, "Return is wrong");
    return TEST_SUCCESS;
}

#define TEST_LOG_FN     "/tmp/lib_test.log"

TEST(print_log_init) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_NONE) == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_1) {
    TEST_ASSERT(m_log("Test %d", 123) == true, "Return is wrong");
    int         fd = open(TEST_LOG_FN, O_RDONLY);
    char        buf[8];

    TEST_ASSERT(fd != 0, "Can't open the log file");
    TEST_ASSERT(read(fd, buf, 8) == 8, "Read return is wrong");
    close(fd);
    TEST_ASSERT(memcmp(buf, "Test 123", 8) == 0, "Content of the file is wrong");
    return TEST_SUCCESS;
}

TEST(print_clean_log_1) {
    set_close_fail(0);
    TEST_ASSERT(m_clean_log() == false, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_clean_log_2) {
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_clean_log_3) {
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_init_2) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_FORCE | M_LOG_TRUNC) == true, "Return is wrong");
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_init_err) {
    TEST_ASSERT(m_init_log("/nonsense/path", M_LOG_NONE) == false, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_minfo) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_FORCE | M_LOG_TRUNC) == true, "Return is wrong");
    m_info("Test 123");

    int         fd = open(TEST_LOG_FN, O_RDONLY);
    char        buf[14];

    TEST_ASSERT(fd != 0, "Can't open the log file");
    TEST_ASSERT(read(fd, buf, 14) == 14, "Read return is wrong");
    close(fd);
    TEST_ASSERT(memcmp(buf, "[INFO] Test 123", 14) == 0, "Content of the file is wrong");
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_mwarning) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_FORCE | M_LOG_TRUNC) == true, "Return is wrong");
    m_warning("Test 123");

    int         fd = open(TEST_LOG_FN, O_RDONLY);
    char        buf[14];

    TEST_ASSERT(fd != 0, "Can't open the log file");
    TEST_ASSERT(read(fd, buf, 14) == 14, "Read return is wrong");
    TEST_ASSERT(memcmp(buf, "[WARN] Test 123", 14) == 0, "Content of the file is wrong");
    close(fd);
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_merror) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_FORCE | M_LOG_TRUNC) == true, "Return is wrong");
    m_error("Test 123");

    int         fd = open(TEST_LOG_FN, O_RDONLY);
    char        buf[15];

    TEST_ASSERT(fd != 0, "Can't open the log file");
    TEST_ASSERT(read(fd, buf, 15) == 15, "Read return is wrong");
    TEST_ASSERT(memcmp(buf, "[ERROR] Test 123", 15) == 0, "Content of the file is wrong");
    close(fd);
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}


TEST(print_log_mpanic) {
    TEST_ASSERT(m_init_log(TEST_LOG_FN, M_LOG_FORCE | M_LOG_TRUNC) == true, "Return is wrong");

    int			st, p_fd[2];
    pid_t		pid;

    pipe(p_fd);
    if ((pid = fork()) == 0) {
        DUP_ALL_OUTPUTS(p_fd);
        m_panic("Test 123");
    } else {
        WAIT_AND_CLOSE(pid, st, p_fd);
    }

    int         fd = open(TEST_LOG_FN, O_RDONLY);
    char        buf[15];

    TEST_ASSERT(fd != 0, "Can't open the log file");
    TEST_ASSERT(read(fd, buf, 15) == 15, "Read return is wrong");
    close(fd);
    TEST_ASSERT(memcmp(buf, "[PANIC] Test 123", 15) == 0, "Content of the file is wrong");
    TEST_ASSERT(m_clean_log() == true, "Return is wrong");
    return TEST_SUCCESS;
}

TEST(print_log_cleanup) {
	unlink(TEST_LOG_FN);
	return TEST_SUCCESS;
}

void	register_print_tests(void) {
    reg_test("mprint", print_info);
    reg_test("mprint", print_warning);
    reg_test("mprint", print_error);
    reg_test("mprint", print_panic);
    reg_test("mprint", print_log_err);
    reg_test("mprint", print_log_init);
    reg_test("mprint", print_log_1);
    reg_test("mprint", print_clean_log_1);
    reg_test("mprint", print_clean_log_2);
    reg_test("mprint", print_clean_log_3);
    reg_test("mprint", print_log_init_2);
    reg_test("mprint", print_log_init_err);
    reg_test("mprint", print_log_minfo);
    reg_test("mprint", print_log_mwarning);
    reg_test("mprint", print_log_merror);
    reg_test("mprint", print_log_mpanic);
    reg_test("mprint", print_log_cleanup);
}
