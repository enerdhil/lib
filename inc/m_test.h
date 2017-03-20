/*********************************** LICENSE **********************************\
*                            Copyright 2017 Morphux                            *
*                                                                              *
*        Licensed under the Apache License, Version 2.0 (the "License");       *
*        you may not use this file except in compliance with the License.      *
*                  You may obtain a copy of the License at                     *
*                                                                              *
*                 http://www.apache.org/licenses/LICENSE-2.0                   *
*                                                                              *
*      Unless required by applicable law or agreed to in writing, software     *
*       distributed under the License is distributed on an "AS IS" BASIS,      *
*    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*        See the License for the specific language governing permissions and   *
*                       limitations under the License.                         *
\******************************************************************************/

#ifndef M_TEST_H
# define M_TEST_H

# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <m_list.h>
# include <m_print.h>

typedef struct test_s {
    char    *(*fn_test)(void);
    char    *group;
    char    *name;
} mtest_t;

typedef struct test_results_s {
    char    *group_name;
    u32_t   success;
    u32_t   failed;
    u32_t   total;
} mtest_results_t;


/* Defines */
# define TEST_SUCCESS       0x0
# define LINE_SIZE          80
# define TITLE_LEN          LINE_SIZE

# define TEST(name) char *test_##name(void)
# define reg_test(group, name) register_test(group, &test_##name, #name);
# define TEST_ASSERT(condition, error_name) {\
    if (!(condition)) {\
        char *__ret = NULL; \
        asprintf(&__ret, "\t%s: Test: '%s', File %s:%d", error_name, #condition, __FILE__, __LINE__);\
        return __ret;\
    }\
}
# define TEST_ASSERT_FMT(condition, error_name, ...) {\
    if (!(condition)) {\
        char *__ret = NULL, *tmp = NULL; \
        asprintf(&tmp, error_name, __VA_ARGS__); \
        asprintf(&__ret, "\t%s: Test: '%s', File %s:%d", tmp, #condition, __FILE__, __LINE__);\
        free(tmp); \
        return __ret;\
    }\
}



/*!
 * \brief Print a title
 * \param s Title name
 */
void title(char *s);

/*!
 * \brief Register a test
 * \param group Group name
 * \param fn_test Test function
 * \param name Test name
 *
 * \note Do not call this function directly, use test_reg macro instead.
 */
void register_test(char *group, char *(*fn_test)(void), char *name);

/*!
 * \brief Test an entire group of tests
 * \param group Group name
 */
mtest_results_t test_group(char *group);

/*!
 * \brief Test all registered tests
 * \return Numbers of tests failed
 */
u32_t test_all(void);

/*!
 * \brief Free all the test
 */
void test_free(void);

/*!
 * \brief Free a single test
 * \note Used in test_free, as a list_free callback
 */
int single_test_free(void *ptr);

/*!
 * \brief Free a mtest_results_t
 * \note Used in test_all, as a list_free callback
 */
int single_result_free(void *ptr);

void print_result(const char *title, u32_t success, u32_t failed);

#endif /* M_TEST_H */
