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

#ifdef COMPILE_WITH_TEST
# ifndef M_FAIL_TEST_H
#  define M_FAIL_TEST_H

# undef strdup

# define malloc(x) fl_malloc(x)
# define write(fd, ptr, len) fl_write(fd, ptr, len)
# define read(fd, ptr, len) fl_read(fd, ptr, len)
# define close(fd) fl_close(fd)
# define strdup(str) fl_strdup(str)
# define fstat(fd, buf) fl_fstat(fd, buf)
# define calloc(memb, size) fl_calloc(memb, size)

void    *fl_malloc(size_t alloc);
ssize_t fl_write(int fd, const void *ptr, size_t len);
ssize_t fl_read(int fd, void *ptr, size_t len);
int     fl_close(int fd);
char    *fl_strdup(const char *str);
int     fl_fstat(int fd, struct stat *buf);
void    *fl_calloc(size_t nmemb, size_t size);

void    set_malloc_fail(int val);
void    set_write_fail(int val);
void    set_read_fail(int val);
void    set_close_fail(int val);
void    set_strdup_fail(int val);
void    set_fstat_fail(int val);
void    set_calloc_fail(int val);


# endif /* M_FAIL_TEST_H */
#endif /* COMPILE_WITH_TEST */
