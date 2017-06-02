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
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/socket.h>
# include <fcntl.h>
# include <unistd.h>
# include <errno.h>
# include <netdb.h>
# include <arpa/inet.h>


# define MOCK_SET_IMP(fn_name, type, error, ...) \
    static type (*real_##fn_name)(__VA_ARGS__) = &(fn_name); \
    static int g_##fn_name##_fail = -1; \
    void set_##fn_name##_fail(int val) { \
        if (g_##fn_name##_fail == -1 || val == -1) \
            g_##fn_name##_fail = val; \
    } \
    type fl_##fn_name(__VA_ARGS__) { \
        if (g_##fn_name##_fail == 0) { \
            g_##fn_name##_fail = -1; \
            return error; \
        } if (g_##fn_name##_fail != -1) \
            g_##fn_name##_fail--;

# define MOCK_REAL_CALL(fn_name, ...) return real_##fn_name(__VA_ARGS__); }

/*!
 * USAGE:
 * MOCK_SET_IMP(Real_function_name, Real_function_return, Errored_return, args...);
 */

MOCK_SET_IMP(malloc, void *, NULL, size_t alloc) {
    MOCK_REAL_CALL(malloc, alloc);
}

MOCK_SET_IMP(write, ssize_t, -1, int fd, const void *ptr, size_t len) {
    MOCK_REAL_CALL(write, fd, ptr, len);
}

MOCK_SET_IMP(read, ssize_t, -1, int fd, void *ptr, size_t len) {
    MOCK_REAL_CALL(read, fd, ptr, len);
}

MOCK_SET_IMP(close, int, -1, int fd) {
    MOCK_REAL_CALL(close, fd);
}

MOCK_SET_IMP(strdup, char *, NULL, const char *str) {
    MOCK_REAL_CALL(strdup, str);
}

MOCK_SET_IMP(fstat, int, -1, int fd, struct stat *buf) {
    MOCK_REAL_CALL(fstat, fd, buf);
}

MOCK_SET_IMP(stat, int, -1, const char *fn, struct stat *buf) {
    MOCK_REAL_CALL(stat, fn, buf);
}

MOCK_SET_IMP(calloc, void *, NULL, size_t nmemb, size_t size) {
    MOCK_REAL_CALL(calloc, nmemb, size)
}

MOCK_SET_IMP(strcpy, char *, NULL, char *dst, const char *src) {
    MOCK_REAL_CALL(strcpy, dst, src);
}

MOCK_SET_IMP(strcat, char *, NULL, char *dst, const char *src) {
    MOCK_REAL_CALL(strcat, dst, src);
}

MOCK_SET_IMP(mkdir, int, -1, const char *pathname, mode_t mode) {
    MOCK_REAL_CALL(mkdir, pathname, mode);
}

MOCK_SET_IMP(fork, pid_t, -1) {
    MOCK_REAL_CALL(fork);
}

MOCK_SET_IMP(chdir, int, -1, const char *path) {
    MOCK_REAL_CALL(chdir, path);
}

MOCK_SET_IMP(getaddrinfo, int, EAI_FAIL, const char *hostname,
                const char *servname, const struct addrinfo *hints,
                    struct addrinfo **res) {
    MOCK_REAL_CALL(getaddrinfo, hostname, servname, hints, res);
}

MOCK_SET_IMP(socket, int, -1, int domain, int type, int protocol) {
    MOCK_REAL_CALL(socket, domain, type, protocol);
}

#endif /* COMPILE_WITH_TEST */
