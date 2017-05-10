#include "test.h"

int     main(void) {
    u32_t   ret;

    /* MUST keep test_tests called first */
    register_tests_tests();
    register_infos_tests();
    register_args_tests();
    register_list_tests();
    register_print_tests();
    register_files_tests();
    register_util_tests();

    m_info("Beginning tests...\n");
    ret = test_all();
    test_free();

    return ret;
}
