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

#ifndef M_INFOS_H
# define M_INFOS_H

# include <stdio.h>
# include <string.h>
# include <morphux.h>

# define INFOS_G_LEN_MAX 150

/*!
 * \brief Set the program name to a string
 * \param[in] str String to set to
 */
void set_program_name(const char *str);

/*!
 * \brief Set the version name to a string
 * \param[in] str String to set to
 */
void set_version(const char *str);

/*!
 * \brief Set the maintainer name to a string
 * \param[in] str String to set to
 */
void set_maintainer(const char *str);

/*!
 * \brief Get the program name
 * \note If the program is not set, return NULL
 */
const char *get_program_name(void);

/*!
 * Get the version
 * \note If the version is not set, return NULL
 */
const char *get_version(void);

/*!
 * Get the maintainer
 * \note If the maintainer is not set, return NULL
 */
const char *get_maintainer(void);

#endif /* M_INFOS_H */
