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

# define INFOS_G_LEN_MAX 150

void	set_program_name(const char *str);
void	set_version(const char *str);
void	set_maintainer(const char *str);
const char	*get_program_name(void);
const char	*get_version(void);
const char	*get_maintainer(void);

#endif /* M_INFOS_H */
