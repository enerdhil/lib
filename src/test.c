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
# include <stdlib.h>
# include <sys/types.h>
# include <fcntl.h>
# include <unistd.h>

/* Real functions */
static void    *(*real_malloc)(size_t) = &malloc;
static ssize_t (*real_write)(int, const void *, size_t) = &write;
static ssize_t (*real_read)(int, void *, size_t) = &read;
static int     (*real_close)(int) = &close;

# include <fail_test.h>

static int      g_malloc_fail = -1;
static int      g_write_fail = -1;
static int      g_read_fail = -1;
static int      g_close_fail = -1;

void    *fl_malloc(size_t alloc) {
    if (g_malloc_fail == -1)
        return real_malloc(alloc);
    if (g_malloc_fail == 0) {
        g_malloc_fail = -1;
        return NULL;
    }
    g_malloc_fail--;
    return real_malloc(alloc);
}

ssize_t fl_write(int fd, const void *ptr, size_t len) {
    if (g_write_fail == -1)
        return real_write(fd, ptr, len);
    if (g_write_fail == 0) {
        g_write_fail = -1;
        return -1;
    }
    g_write_fail--;
    return real_write(fd, ptr, len);
}

ssize_t fl_read(int fd, void *ptr, size_t len) {
    if (g_read_fail == -1)
        return real_read(fd, ptr, len);
    if (g_read_fail == 0) {
        g_read_fail = -1;
        return -1;
    }
    g_read_fail--;
    return real_read(fd, ptr, len);
}

int fl_close(int fd) {
    if (g_close_fail == -1)
        return real_close(fd);
    if (g_close_fail == 0) {
        g_close_fail = -1;
        return -1;
    }
    g_close_fail--;
    return real_close(fd);
}

void set_malloc_fail(int val) {
    if (g_malloc_fail == -1)
        g_malloc_fail = val;
}

void set_write_fail(int val) {
    if (g_write_fail == -1)
        g_write_fail = val;
}

void set_read_fail(int val) {
    if (g_read_fail == -1)
        g_read_fail = val;
}

void set_close_fail(int val) {
    if (g_close_fail == -1)
        g_close_fail = val;
}

#endif /* COMPILE_WITH_TEST */
