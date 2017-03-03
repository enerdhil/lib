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

#include <m_print.h>

static int      g_log_fd = 0;
static u8_t     g_log_flags = M_LOG_NONE;

/*!
 * \brief Initialize the logging for the program
 * \param file File path
 * \param force Flag (see defines M_LOG_* in m_print.h)
 *
 * Open a file with O_APPEN flag, and keep it open.
 * If the M_LOG_FORCE flag is true, all the calls to m_{panic,error,warning,info}
 * will be written in the log file.
 */
bool        m_init_log(const char *file, u8_t flags) {
    u32_t    open_flags = O_CREAT | O_WRONLY | O_ASYNC;
    int     fd;

    if (flags & M_LOG_TRUNC) {
        open_flags |= O_TRUNC;
    } else {
        open_flags |= O_APPEND;
    }
    fd = open(file, open_flags, 0662);
    if (fd != -1) {
        if (flags & M_LOG_FORCE)
            g_log_flags |= M_LOG_FORCE;
        g_log_fd = fd;
        return true;
    }
    return false;
}

/*!
 * \brief Close a log file descriptor and reset the flags
 */
bool        m_clean_log(void) {
    if (g_log_fd != 0) {
        if (close(g_log_fd) != 0)
            return false;
        g_log_fd = 0;
        g_log_flags = 0;
    }
    return true;
}

static bool m_log_v(const char *str, va_list ap) {
    if (g_log_fd == 0)
        return false;
    vdprintf(g_log_fd, str, ap);
    return true;
}

/*!
 * \brief Print a string in an error fomat, then call exit with 1
 * \note Support printf format
 */
void		m_panic(const char *str, ...) {
    va_list		ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE) {
        m_log_v(str, ap);
    } else {
        write(2, "\033[0;31m> \033[0m", 13);
        vfprintf(stderr, str, ap);
        if (str[strlen(str) - 1] != '\n')
            fprintf(stderr, "\n");
    }
    va_end(ap);
    exit(1);
}

/*!
 * \brief Print a string with an error format
 * \note Support printf format
 */
void		m_error(const char *str, ...) {
    va_list		ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE) {
        m_log_v(str, ap);
    } else {
        write(2, "\033[0;31m> \033[0m", 13);
        vfprintf(stderr, str, ap);
        if (str[strlen(str) - 1] != '\n')
            fprintf(stderr, "\n");
    }
    va_end(ap);
}

/*!
 * \brief Print a string with a warning format
 * \note Support printf format
 */
void		m_warning(const char *str, ...) {
    va_list		ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE) {
        m_log_v(str, ap);
    } else {
        write(2, "\033[0;31m> \033[0m", 13);
        vfprintf(stderr, str, ap);
    }
    va_end(ap);
}

/*!
 * \brief Print a string with an information format
 * \note Support printf format
 */
void		m_info(const char *str, ...) {
    va_list		ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE) {
        m_log_v(str, ap);
    } else {
        write(1, "\033[0;34m> \033[0m", 13);
        vprintf(str, ap);
    }
    va_end(ap);
}

/*!
 * \brief Write a string in an already opened log file
 * \return true on success, false on failure
 */
bool        m_log(const char *str, ...) {
    va_list     ap;
    bool        ret;

    va_start(ap, str);
    ret = m_log_v(str, ap);
    va_end(ap);
    return ret;
}
