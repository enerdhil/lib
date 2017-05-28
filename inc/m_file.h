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

#ifndef M_FILE_H
# define M_FILE_H

# include <sys/types.h>
# include <sys/stat.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>

/*!
 * \brief Read an entire file from an fd
 *
 * \param[in] fd File descriptor
 *
 * Read an entire from a fd, in two syscalls
 *
 * \return The content of the file, NULL on failure
 */
char *mpm_read_file_from_fd(int fd);

/*!
 * \brief Read an entire file from a filename
 *
 * \param[in] fn Filename
 *
 * Read an entire file from a fd, in 4 syscalls
 * 
 * \return The content of the file, NULL on failure
 */
char *mpm_read_file_from_fn(const char *fn);

/*!
 * \brief Get a size from an fd, in bytes
 *
 * \param[in] fd File descriptor
 *
 * \return The size of file, 0 on failure
 */
off_t mpm_get_file_size_from_fd(int fd);

/*!
 * \brief Get a size from a filename, in bytes
 *
 * \param[in] fn Filename
 *
 * \return The size of the file, 0 on failure
 */
off_t mpm_get_file_size_from_fn(const char *fn);

/*!
 * \brief Open a file, an create directory in order to access it
 *
 * \param[in] File path
 *
 * This function will create directories in order to achieve the desired path,
 * then return a FILE * on the opened file.
 * Example:
 * > recursive_file_open("/tmp/something/somedir/somefile");
 *
 * \note Due to its nature, this function is not very likely to fail
 * \return A FILE * on success, NULL on failure
 */
FILE *recursive_file_open(char *fn);

#endif /* M_FILE_H */
