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

#ifndef M_PRINT_H
# define M_PRINT_H

# define _XOPEN_SOURCE 700
# if defined(__APPLE__)
#  define _DARWIN_C_SOURCE
# endif

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <m_types.h>
# include <morphux.h>

# define M_LOG_NONE (1 << 0)
# define M_LOG_FORCE (1 << 2)
# define M_LOG_TRUNC (1 << 3)

/*!
 * \brief Print a string in an error fomat, then call exit with 1
 * \note Support printf format
 */
void m_panic(const char *str, ...);

/*!
 * \brief Print a string with an error format
 * \note Support printf format
 */
void m_error(const char *str, ...);

/*!
 * \brief Print a string with a warning format
 * \note Support printf format
 */
void m_warning(const char *str, ...);

/*!
 * \brief Print a string with an information format
 * \note Support printf format
 */
void m_info(const char *str, ...);

/*!
 * \brief Write a string in an already opened log file
 * \return true on success, false on failure
 * \note Support printf format
 */
bool m_log(const char *str, ...);

/*!
 * \brief Initialize the logging for the program
 * \param[in] str File path
 * \param[in] flags Flag (see defines M_LOG_* in m_print.h)
 *
 * Open a file with O_APPEND flag, and keep it open.
 * If the M_LOG_FORCE flag is true, all the calls to m_{panic,error,warning,info}
 * will be written in the log file.
 */
bool m_init_log(const char *str, u8_t flags);

/*!
 * \brief Close a log file descriptor and reset the flags
 */
bool m_clean_log(void);

#endif /* M_PRINT_H */
