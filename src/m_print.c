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

bool m_init_log(const char *file, u8_t flags) {
    u32_t    open_flags = O_CREAT | O_WRONLY | O_ASYNC;
    int      fd;

    if (flags & M_LOG_TRUNC)
        open_flags |= O_TRUNC;
    else
        open_flags |= O_APPEND;

    fd = open(file, open_flags, 0662);
    if (fd != -1)
    {
        if (flags & M_LOG_FORCE)
            g_log_flags |= M_LOG_FORCE;
        g_log_fd = fd;
        return true;
    }
    return false;
}


bool m_clean_log(void) {
    if (g_log_fd != 0)
    {
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


void m_panic(const char *str, ...) {
    va_list     ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE)
    {
        m_log(M_LOG_PANIC_BEG);
        m_log_v(str, ap);
    }
    else
    {
        write(2, M_PANIC_BEG, sizeof(M_PANIC_BEG));
        vfprintf(stderr, str, ap);
        if (str[strlen(str) - 1] != '\n')
            fprintf(stderr, "\n");
    }
    va_end(ap);
    exit(1);
}


void m_error(const char *str, ...) {
    va_list     ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE)
    {
        m_log(M_LOG_ERROR_BEG);
        m_log_v(str, ap);
    }
    else
    {
        write(2, M_ERROR_BEG, sizeof(M_ERROR_BEG));
        vfprintf(stderr, str, ap);
        if (str[strlen(str) - 1] != '\n')
            fprintf(stderr, "\n");
    }
    va_end(ap);
}


void m_warning(const char *str, ...) {
    va_list     ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE)
    {
        m_log(M_LOG_WARN_BEG);
        m_log_v(str, ap);
    }
    else
    {
        write(2, M_WARN_BEG, sizeof(M_WARN_BEG));
        vfprintf(stderr, str, ap);
    }
    va_end(ap);
}

void m_info(const char *str, ...) {
    va_list     ap;

    va_start(ap, str);
    if (g_log_flags & M_LOG_FORCE)
    {
        m_log(M_LOG_INFO_BEG);
        m_log_v(str, ap);
    }
    else
    {
        write(1, M_INFO_BEG, sizeof(M_INFO_BEG));
        vprintf(str, ap);
        fflush(stdout);
    }
    va_end(ap);
}

void m_debug(const char *str, ...) {
#ifdef DEBUG
    va_list     ap;

    va_start(ap, str);
    vprintf(str, ap);
    fflush(stdout);
    va_end(ap);
#else
    (void)str;
    return ;
#endif

}

bool m_log(const char *str, ...) {
    va_list     ap;
    bool        ret;

    va_start(ap, str);
    ret = m_log_v(str, ap);
    va_end(ap);
    return ret;
}

#ifdef DEBUG_FULL
# define __USE_GNU
# include <dlfcn.h>
# include <time.h>

void __cyg_profile_func_enter(void *fn, void *call) __attribute__((no_instrument_function));
void __cyg_profile_func_exit(void *fn, void *call) __attribute__((no_instrument_function));

void __cyg_profile_func_enter(void *fn, void *call) {
    Dl_info     info;

    (void)call;
    if (dladdr(fn, &info) != 0)
    {
        printf("[%lu]: %s:%s %p\n", time(NULL), info.dli_fname,
            info.dli_sname == NULL ? "???" : info.dli_sname, fn);
        return ;
    }
}

void __cyg_profile_func_exit(void *fn, void *call) {
    (void)fn;
    (void)call;
    return ;
}

#endif
