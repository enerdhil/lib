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

# include <stdarg.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <string.h>
# include <unistd.h>
# include <stdbool.h>
# include <m_types.h>

# define M_LOG_NONE (1 << 0)
# define M_LOG_FORCE (1 << 2)
# define M_LOG_TRUNC (1 << 3)

void		m_panic(const char *str, ...);
void		m_error(const char *str, ...);
void		m_warning(const char *str, ...);
void		m_info(const char *str, ...);
bool        m_log(const char *str, ...);
bool        m_init_log(const char *str, u8_t flags);
bool        m_clean_log(void);

#endif /* M_PRINT_H */
