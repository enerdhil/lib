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

#include <m_file.h>

off_t mpm_get_file_size_from_fd(int fd)
{
    struct stat buf;
    if (fd == -1)
        return 0;

    if (fstat(fd, &buf) == -1)
        return 0;
    return buf.st_size;
}

off_t mpm_get_file_size_from_fn(const char *fn)
{
    int     fd;
    off_t   ret;

    if (fn == NULL)
        return 0;

    fd = open(fn, O_RDONLY);
    if (fd == -1)
        return 0;
    ret = mpm_get_file_size_from_fd(fd);
    close(fd);
    return ret;
}

char *mpm_read_file_from_fd(int fd)
{
    char    *ret = NULL;
    off_t   size;

    if (fd == -1)
        return NULL;

    size = mpm_get_file_size_from_fd(fd);
    if (size == 0)
        return NULL;

    ret = malloc(size);
    if (ret == NULL)
        return NULL;

    if (read(fd, ret, size) == -1)
        goto cleanup;

    return ret;

cleanup:
    free(ret);
    return NULL;
}

char *mpm_read_file_from_fn(const char *fn)
{
    int     fd;
    char    *ret = NULL;

    if (fn == NULL)
        return NULL;

    fd = open(fn, O_RDONLY);
    if (fd == -1)
        return NULL;

    ret = mpm_read_file_from_fd(fd);
    close(fd);
    return ret;
}
