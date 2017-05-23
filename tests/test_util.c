#include "test.h"

typedef struct file_test_s {
    const char  *path;
    mode_t      mode;
    bool        is_dir;
} file_test_t;

#define MAIN_PATH "test_dir/"

static const file_test_t files_to_create[] = {
    {
        .path = MAIN_PATH,
        .mode =  S_IRWXU,
        .is_dir = true
    },
    {
        .path = MAIN_PATH "file_1",
        .mode = 0000
    },
    {
        .path = MAIN_PATH "file_2",
        .mode = 0777
    }
};

TEST(init_dir) {
    /* List */
    for (size_t i = 0; i < sizeof(files_to_create) / sizeof(files_to_create[0]); i++)
    {
        if (files_to_create[i].is_dir)
        {
            mkdir(files_to_create[i].path, files_to_create[i].mode);
        }
        else
        {
            int fd = open(files_to_create[i].path, O_CREAT, files_to_create[i].mode);
            close(fd);
        }
    }

    return TEST_SUCCESS;
}

TEST(recursive_delete) {
    TEST_ASSERT(recursive_delete("test_dir_2") == false, "Error did not raise");
    TEST_ASSERT(recursive_delete(MAIN_PATH) == true, "Error did not raise");
    return TEST_SUCCESS;
}

TEST(file_exist) {
    TEST_ASSERT(file_exist("Makefile") == true, "Error on the return");
    TEST_ASSERT(file_exist("Makefile_totally_not_here") == false, "Error on the return");
    return TEST_SUCCESS;
}

MPX_STATIC char **str_list_to_array(mlist_t *list);
TEST(str_list_to_array) {
    mlist_t     *head = NULL;
    char        **tmp = NULL;
    static char  *strings[] = {
        "Test",
        "1",
        "2"
    };

    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
        list_add(head, strings[i], sizeof(strings[i]));

    set_malloc_fail(0);
    TEST_ASSERT(str_list_to_array(head) == NULL, "Malloc failed did not return an error");
    set_strdup_fail(1);
    TEST_ASSERT(str_list_to_array(head) == NULL, "Failed copy did not return an error");
    tmp = str_list_to_array(head);
    TEST_ASSERT(tmp != NULL, "An error happened");

    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
    {
        TEST_ASSERT_FMT(strcmp(strings[i], tmp[i]) == 0, "Strings are different (%s/%s)", strings[i], tmp[i]);
        free(tmp[i]);
    }
    free(tmp);
    list_free(head, NULL);
    return TEST_SUCCESS;
}

TEST(exec_line) {
    set_execute_flags(COMP_FLAGS_NONE);

    TEST_ASSERT(exec_line(NULL) == 1, "Wrong return on NULL parameter");
    set_strdup_fail(0);
    TEST_ASSERT(exec_line("test") == 1, "Wrong return on failed copy");
    TEST_ASSERT(exec_line("totally_wrong_binary") != 0, "Wrong execvp return");
    TEST_ASSERT(exec_line("ls /") == 0, "Wrong execvp return");
    return TEST_SUCCESS;
}

TEST(exec_list) {
    mlist_t *head = NULL;
    static char *strings[] = {
        "ls",
        "-la"
    };

    for (size_t i = 0; i < sizeof(strings) / sizeof(strings[0]); i++)
        list_add(head, strings[i], sizeof(strings[i]));

    set_malloc_fail(0);
    TEST_ASSERT(exec_list(head) == 1, "Malloc failed did not return an error");
    set_fork_fail(0);
    TEST_ASSERT(exec_list(head) == 1, "Failed fork did not return an error");
    TEST_ASSERT(exec_list(head) == 0, "An error happened");
    list_free(head, NULL);
    return TEST_SUCCESS;
}

void register_util_tests(void) {
    reg_test("util", init_dir);
    reg_test("util", recursive_delete);
    reg_test("util", file_exist);
    reg_test("util", str_list_to_array);
    reg_test("util", exec_line);
    reg_test("util", exec_list);
}
