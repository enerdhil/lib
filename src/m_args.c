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

#include <m_args.h>

/*!
 * \brief Read the options given by the program
 * \param ac Number of argument in av
 * \param av Array of string, containing the arguments
 * \param args Array of margs_t, containing the preset options. Must end with
 * an empty structure.
 * \return Number of options read
 *
 * The read_opt function reads a given list of arguments, and parse the options
 * in it. The options are read from the args array.
 * If an option is not known, the function calls the help and quit.
 * If the option is -h | --help is passed, the function call the help and quit.
 *
 * \note Only the arguments beginning with - are parsed.
 */
u32_t		read_opt(const int ac, char **av, const margs_t *args) {
	u32_t		ret = 0, it, k;
	u8_t		n_dash;

	for (u32_t i = 1, j = 0; i < (u32_t)ac; i++) {

		if (av[i] == NULL || strlen(av[i]) == 0)
			continue ;

		n_dash = 0;
		for (j = 0; av[i][j] != '\0' && av[i][j] == '-'; j++)
			n_dash++;

		/* Argument have more than two '-' */
		if (n_dash > 2) {
			m_error("Malformed option: %s\n", av[i]);
			continue ;
		}

		/* Single letter option */
		if (n_dash == 1) {

			if (strlen(av[i]) < 2) {
				m_error("Dash without option. Ignoring...\n");
				continue ;
			}

			/* Help option */
			if (av[i][1] == 'h')
				opt_help(args);

			/* Search the option in the args array */
			for (it = 0; args[it].opt != 0 && args[it].opt != av[i][1]; it++)
				;

			/* Can't find the option */
			if (args[it].opt == 0) {
				m_error("Unknow option -%s\n", &(av[i][1]));
				opt_help(args);
			} else {
				if (args[it].take_arg) {
					if (i + 1 < (u32_t)ac)
						args[it].callback(av[++i]);
					else
						m_panic("Option -%c must take an argument\n",
							args[it].opt);
				} else {
					args[it].callback(NULL);
				}
			}

		/* Word option */
		} else if (n_dash == 2) {
			bool	got_arg = false;

			if (strlen(av[i]) < 3) {
				m_error("Double dash without option. Ignoring...\n");
				continue ;
			}

			if (strcmp(&(av[i][2]), "help") == 0)
				opt_help(args);

			/* Look for an argument */
			for (k = 2; av[i][k] != '\0' && av[i][k] != '='; k++)
				;

			if (av[i][k] != '\0') {
				got_arg = true;
				k -= 2;
			} else {
				k = strlen(av[i]) - 2;
			}

			/* Search the option in the args array */
			for (it = 0; args[it].opt != 0 && (args[it].s_opt != NULL &&
						strncmp(args[it].s_opt, &(av[i][2]), k) != 0); it++)
				;

			/* Can't find the option */
			if (args[it].opt == 0) {
				m_error("Unknown option %s\n", av[i]);
				opt_help(args);
			} else {
				if (args[it].take_arg && !got_arg)
					m_panic("Option %s must take an argument", args[it].s_opt);
				if (got_arg)
					args[it].callback(&(av[i][k + 3]));
				else
					args[it].callback(NULL);
			}
		}
	}
	return ret;
}

/*!
 * \brief Print helps with a list of argument
 * \param args List of arguments to print
 */
static void		opt_help(const margs_t *args) {
	m_info("Help:\n");
	for (u32_t i = 0; args[i].opt != 0; i++) {
		m_info("\t-%c | --%s : %s\n", args[i].opt, args[i].s_opt, args[i].desc);
	}
	_exit(1);
}
