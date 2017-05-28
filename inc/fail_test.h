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

# define malloc(size)           fl_malloc(size)
# define write(fd, ptr, len)    fl_write(fd, ptr, len)
# define read(fd, ptr, len)     fl_read(fd, ptr, len)
# define close(fd)              fl_close(fd)
# define strdup(str)            fl_strdup(str)
# define fstat(fd, buf)         fl_fstat(fd, buf)
# define stat(fd, buf)          fl_stat(fd, buf)
# define calloc(memb, size)     fl_calloc(memb, size)
# define strcpy(dst, src)       fl_strcpy(dst, src)
# define strcat(dst, src)       fl_strcat(dst, src)
# define mkdir(path, mode)      fl_mkdir(path, mode)
# define fork                   fl_fork
# define chdir                  fl_chdir

# define MOCK_SET_DECL(fn_name, type, ...) \
                            void set_##fn_name##_fail(int val); \
                            type fl_##fn_name(__VA_ARGS__);

MOCK_SET_DECL(malloc, void *, size_t);
MOCK_SET_DECL(write, ssize_t, int, const void *, size_t);
MOCK_SET_DECL(read, ssize_t, int, void *, size_t);
MOCK_SET_DECL(close, int, int);
MOCK_SET_DECL(strdup, char *, const char *);
MOCK_SET_DECL(fstat, int, int, struct stat *);
MOCK_SET_DECL(stat, int, const char *, struct stat *);
MOCK_SET_DECL(calloc, void *, size_t, size_t);
MOCK_SET_DECL(strcpy, char *, char *, const char *);
MOCK_SET_DECL(strcat, char *, char *, const char *);
MOCK_SET_DECL(mkdir, int, const char *, mode_t);
MOCK_SET_DECL(fork, pid_t);
MOCK_SET_DECL(chdir, int, const char *);

# endif /* M_FAIL_TEST_H */
#endif /* COMPILE_WITH_TEST */
