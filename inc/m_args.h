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

#ifndef M_ARGS_H
# define M_ARGS_H

# include <stdbool.h>
# include <m_types.h>
# include <m_print.h>
# include <m_infos.h>

typedef struct		s_args {
	/**
	 * Single letter option
	 * Example: -f, -s
	 */
	char	opt;

	/**
	 * Full string option
	 * Example: --force, --skip
	 */
	char	*s_opt;

	/**
	 * Description of the option.
	 * Used for the help
	 */
	char	*desc;

	/**
	 * Boolean that describe if the option must take an argument
	 */
	bool		take_arg;

	/**
	 * Callback of the option
	 */
	void	(*callback)(const char *);
}					margs_t;

#define ARGS_EOL {0, NULL, NULL, false, NULL}
#define IS_EOL(lst) (lst.opt == 0 && lst.s_opt == NULL && lst.desc == NULL && \
						lst.take_arg == false && lst.callback == NULL)


u32_t			read_opt(const int ac, char **av, const margs_t *args);
void			opt_help(const margs_t *args, u8_t ret);
void			p_version(u8_t ret);

#endif /* M_ARGS_H */
