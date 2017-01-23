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

typedef struct		s_test {
	char				*(*fn_test)(void);
	char				*group;
	char				*name;
}					mtest_t;

typedef struct		s_test_results {
	char				*group_name;
	u32_t				success;
	u32_t				failed;
	u32_t				total;
}					mtest_results_t;


/* Defines */
# define TEST_SUCCESS		0x0
# define TITLE_LEN			80

# define TEST(name) char		*test_##name(void)
# define reg_test(group, name) register_test(group, &test_##name, #name);
# define TEST_ASSERT(condition, error_name) {\
	if (!condition) {\
		char *ret = NULL; \
		asprintf(&ret, "\t%s: Test: '%s', File %s:%d", error_name, #condition, __FILE__, __LINE__);\
		return ret;\
	}\
}

/* Functions */
void			title(char *s);
void			register_test(char *group, char *(*fn_test)(void), char *name);
mtest_results_t	test_group(char *group);
u32_t			test_all(void);
void			test_free(void);
int				single_test_free(void *ptr);

#endif /* M_TEST_H */
