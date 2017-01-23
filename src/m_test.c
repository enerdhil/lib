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

#include <m_test.h>

static mlist_t		*tests = NULL;

/*!
 * \brief Print a title
 * \param s Title name
 */
void	title(char *s) {
	u8_t			len = TITLE_LEN;
	int				i;

	len -= strlen(s);
	for (i = 0; i < len / 2; i++, write(1, "=", 1))
		;
	write(1, " ", 1);
	write(1, s, strlen(s));
	write(1, " ", 1);
	for (; i < len; i++, write(1, "=", 1));
	write(1, "\n", 1);
}

/*!
 * \brief Register a test
 * \param group Group name
 * \param fn_test Test function
 * \param name Test name
 *
 * \note Do not call this function directly, use test_reg macro instead.
 */
void		register_test(char *group, char *(*fn_test)(void), char *name) {
	mtest_t		*ptr;

	/* Allocate test */
	ptr = malloc(sizeof(mtest_t));
	assert(ptr);
	ptr->group = malloc(strlen(group) + 1);
	ptr->name = malloc(strlen(name) + 1);
	assert(ptr->group && ptr->name);

	/* Copy name and group */
	memcpy(ptr->group, group, strlen(group));
	memcpy(ptr->name, name, strlen(name));
	ptr->name[strlen(name)] = 0;
	ptr->group[strlen(group)] = 0;
	assert(ptr->group && ptr->name);
	ptr->fn_test = fn_test;

	/* Add to the list of tests */
	list_add(tests, ptr, sizeof(mtest_t));
	free(ptr);
}

/*!
 * \brief Test an entire group of tests
 * \param group Group name
 */
mtest_results_t	test_group(char *group) {
	mlist_t			*tmp;
	mtest_t			*ptr;
	mtest_results_t	res;
	u32_t			tab;
	char			*s_tmp;

	res.total = res.success = res.failed = 0;
	title(group);

	/* Iterate over each test */
	list_for_each(tests, tmp, ptr) {
		if (strcmp(ptr->group, group) == 0) {
			res.total++;
			m_info("Testing %s ...", ptr->name);

			for (tab = strlen(ptr->name); tab < TITLE_LEN - 18;
				tab++, printf(" "))
				;

			if ((s_tmp = ptr->fn_test()) != TEST_SUCCESS) {
				printf("[ \033[1;31mKO\033[0m ]\n");
				m_warning("\033[0;37m%s\033[0m\n", s_tmp);
				free(s_tmp);
				res.failed++;
			} else {
				printf("[ \033[1;32mOK\033[0m ]\n");
				res.success++;
			}
		}
	}

	if (res.total == 0) {
		m_warning("Could not find any registered tests in %s group.\n", group);
	} else {
		m_info("Group Results: %d%% (%d/%d)\n",
			(res.success * 100) / res.total, res.success, res.total);
	}
	return res;
}

/*!
 * \brief Test all registered tests
 * \return Numbers of tests failed
 */
u32_t		test_all(void) {
	mlist_t			*tmp, *groups = NULL, *tests_results = NULL;
	mtest_t			*ptr;
	mtest_results_t	res, *ptr2;
	u32_t			total = 0, success = 0, failed = 0;

	list_for_each(tests, tmp, ptr) {
		if (list_get(groups, ptr->group, strlen(ptr->group)) == NULL) {
			res = test_group(ptr->group);

			res.group_name = malloc(sizeof(char) * strlen(ptr->group) + 1);
			strcpy(res.group_name, ptr->group);
			total += res.total;
			success += res.success;
			failed += res.failed;
			list_add(groups, ptr->group, strlen(ptr->group));
			list_add(tests_results, &res, sizeof(res));
		}
	}

	title("Results");
	if (total == 0)
		m_warning("No tests registered, skipping.\n");
	else {
		list_for_each(tests_results, tmp, ptr2) {
			m_info("%s:\t\t%d/%d %d%%\n", ptr2->group_name, ptr2->success,
				ptr2->total, ptr2->success * 100 / ptr2->total);
		}
		m_info("Total:\t\t%d%% (%d/%d)\n", (success * 100) / total, success, total);
	}
	list_free(groups, NULL);
	list_free(tests_results, &single_result_free);

	return failed;
}

/*!
 * \brief Free all the test
 */
void	test_free(void) {
	list_free(tests, &single_test_free);
	tests = NULL;
}

/*!
 * \brief Free a single test
 * \note Used in test_free, as a list_free callback
 */
int		single_test_free(void *ptr) {
	mtest_t		*tmp = ptr;

	if (ptr) {
		free(tmp->group);
		free(tmp->name);
	}
	return 1;
}

/*!
 * \brief Free a mtest_results_t
 * \note Used in test_all, as a list_free callback
 */
int		single_result_free(void *ptr) {
	mtest_results_t		*tmp = ptr;
	if (ptr) {
		free(tmp->group_name);
	}
	return 1;
}
