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

void register_util_tests(void) {
    reg_test("util", init_dir);
    reg_test("util", recursive_delete);
}
