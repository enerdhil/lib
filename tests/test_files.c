#include "test.h"

TEST(file_get_file_size_from_fd_1) {
    TEST_ASSERT(mpm_get_file_size_from_fd(-1) == 0, "Did not return 0");
    return TEST_SUCCESS;
}

TEST(file_get_file_size_from_fd_2) {
    int fd = open("./Makefile", O_RDONLY);

    set_fstat_fail(0);
    TEST_ASSERT(mpm_get_file_size_from_fd(fd) == 0, "Wrong return");
    set_fstat_fail(-1);
    close(fd);
    return TEST_SUCCESS;
}

TEST(file_get_file_size_from_fd_3) {
    int fd = open("./Makefile", O_RDONLY);
    TEST_ASSERT(mpm_get_file_size_from_fd(fd) != 0, "Wrong return");
    close(fd);
    return TEST_SUCCESS;
}

TEST(file_get_file_size_from_fn_1) {
    TEST_ASSERT(mpm_get_file_size_from_fn(NULL) == 0, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_get_file_size_from_fn_2) {
    TEST_ASSERT(mpm_get_file_size_from_fn("/non/sense/path") == 0, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_get_file_size_from_fn_3) {
    TEST_ASSERT(mpm_get_file_size_from_fn("./Makefile") != 0, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fd_1) {
    TEST_ASSERT(mpm_read_file_from_fd(-1) == NULL, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fd_2) {
    TEST_ASSERT(mpm_read_file_from_fd(2) == NULL, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fd_3) {
    int fd = open("./Makefile", O_RDONLY);

    set_malloc_fail(0);
    TEST_ASSERT(mpm_read_file_from_fd(fd) == NULL, "Wrong return");
    close(fd);
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fd_4) {
    int fd = open("./Makefile", O_RDONLY);

    set_read_fail(0);
    TEST_ASSERT(mpm_read_file_from_fd(fd) == NULL, "Wrong return");
    close(fd);
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fd_5) {
    int fd = open("./Makefile", O_RDONLY);
    char *ret;

    ret = mpm_read_file_from_fd(fd);
    TEST_ASSERT(ret != NULL, "Wrong return");
    close(fd);
    free(ret);
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fn_1) {
    TEST_ASSERT(mpm_read_file_from_fn(NULL) == NULL, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fn_2) {
    TEST_ASSERT(mpm_read_file_from_fn("/non/sense/path") == NULL, "Wrong return");
    return TEST_SUCCESS;
}

TEST(file_read_file_from_fn_3) {
    char *ret;

    ret = mpm_read_file_from_fn("./Makefile");
    TEST_ASSERT(ret != NULL, "Wrong return");
    free(ret);
    return TEST_SUCCESS;
}

# define _DIR_TO_CREATE "/tmp/dir_1/dir_2/dir_3/file"
TEST(recursive_file_open) {
    FILE    *fd = NULL;

    TEST_ASSERT(recursive_file_open("/somepath_i_cant_create/file") == NULL, "Error did not raise");
    set_strdup_fail(0);
    TEST_ASSERT(recursive_file_open("/somepath_i_cant_create/file") == NULL, "Error did not raise");

    fd = recursive_file_open(_DIR_TO_CREATE);
    TEST_ASSERT(fd != NULL, "Can't create the file");
    fclose(fd);
    recursive_delete("/tmp/dir_1");
    return TEST_SUCCESS;
}


void register_files_tests(void) {
    reg_test("m_file", file_get_file_size_from_fd_1);
    reg_test("m_file", file_get_file_size_from_fd_2);
    reg_test("m_file", file_get_file_size_from_fd_3);
    reg_test("m_file", file_get_file_size_from_fn_1);
    reg_test("m_file", file_get_file_size_from_fn_2);
    reg_test("m_file", file_get_file_size_from_fn_3);
    reg_test("m_file", file_read_file_from_fd_1);
    reg_test("m_file", file_read_file_from_fd_2);
    reg_test("m_file", file_read_file_from_fd_3);
    reg_test("m_file", file_read_file_from_fd_4);
    reg_test("m_file", file_read_file_from_fd_5);
    reg_test("m_file", file_read_file_from_fn_1);
    reg_test("m_file", file_read_file_from_fn_2);
    reg_test("m_file", file_read_file_from_fn_3);
    reg_test("m_file", recursive_file_open);
}
