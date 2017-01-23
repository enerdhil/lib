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

/*!
 * \brief Print a string in an error fomat, then call _exit with 1
 * \note Support printf format
 */
void		m_panic(const char *str, ...) {
	va_list		ap;

	write(2, "\033[0;31m> \033[0m", 13);
	va_start(ap, str);
	vfprintf(stderr, str, ap);
	va_end(ap);
	if (str[strlen(str) - 1] != '\n')
		fprintf(stderr, "\n");
	_exit(1);
}

/*!
 * \brief Print a string with an error format
 * \note Support printf format
 */
void		m_error(const char *str, ...) {
	va_list		ap;

	write(2, "\033[0;31m> \033[0m", 13);
	va_start(ap, str);
	vfprintf(stderr, str, ap);
	va_end(ap);
	if (str[strlen(str) - 1] != '\n')
		fprintf(stderr, "\n");
}

/*!
 * \brief Print a string with a warning format
 * \note Support printf format
 */
void		m_warning(const char *str, ...) {
	va_list		ap;

	write(2, "\033[0;31m> \033[0m", 13);
	va_start(ap, str);
	vfprintf(stderr, str, ap);
	va_end(ap);
}

/*!
 * \brief Print a string with an information format
 * \note Support printf format
 */
void		m_info(const char *str, ...) {
	va_list		ap;

	write(1, "\033[0;34m> \033[0m", 13);
	va_start(ap, str);
	vprintf(str, ap);
	va_end(ap);
}
