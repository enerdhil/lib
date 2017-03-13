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
#include <m_list.h>
#define LIB_OPT_TOKEN_HELP 'h'
#define LIB_OPT_STRING_HELP "help"
#define LIB_OPT_TOKEN_VERSION 'V'
#define LIB_OPT_STRING_VERSION "version"

/*!
 * \brief Read the options given by the program
 * \param ac Number of argument in av
 * \param av Array of string, containing the arguments
 * \param args Array of margs_t, containing the preset options. Must end with
 * an empty structure.
 * \param params Adress of linked list, that will be filled with av with
 * flags removed
 * \return Number of options read
 *
 * The read_opt function reads a given list of arguments, and parse the options
 * in it. The options are read from the args array.
 * If an option is not known, the function calls the help and quit.
 * If the option -h | --help is passed, the function call the help and quit.
 * If the option -V | --version is passed, the function display the version
 * and quit.
 *
 * \note Only the arguments beginning with - are parsed.
 */
u32_t		read_opt(const int ac, char **av, const mopts_t *opts, \
						mlist_t **args) {
	u32_t		ret = 0, i = 1, it, k;
	u8_t		n_dash;

	if (ac == 0 || av == NULL || opts == NULL || args == NULL)
		return ret;

	for (/* Using u32_t i */ u32_t j = 0; i < (u32_t)ac; i++) {

		if (av[i] == NULL || strlen(av[i]) == 0)
			continue ;

		n_dash = 0;
		for (j = 0; av[i][j] != '\0' && av[i][j] == '-'; j++)
			n_dash++;

		/* Single letter option */
		if (n_dash == 1) {

			/* If single dash alone, its a parameter */
			if (strlen(av[i]) < 2) {
			 	/* And we stop reading options. */
				break ;
			}

			/* Builtins options */
			if (av[i][1] == LIB_OPT_TOKEN_HELP)
				opt_help(opts, 0);
			else if (av[i][1] == LIB_OPT_TOKEN_VERSION)
				p_version(0);

			/* Search the option in the opts array */
			for (u32_t z = 1; av[i][z] != '\0'; z++) {
				for (it = 0; !IS_EOL(opts[it]) && opts[it].opt != av[i][z]; it++)
					;

				/* Can't find the option */
				if (IS_EOL(opts[it])) {
					m_error("Unknow option -%s\n", &(av[i][z]));
					opt_help(opts, 1);
				} else {
					if (opts[it].take_arg) {
						if (i + 1 < (u32_t)ac) {
							opts[it].callback(av[++i]);
							ret++;
							break ;
						} else {
							m_error("Option -%c must take an argument\n",
								opts[it].opt);
							opt_help(opts, 1);
						}
					} else {
						opts[it].callback(NULL);
						ret++;
					}
				}
			}

		/* Word option */
		} else if (n_dash == 2) {
			bool	got_arg = false;

			/* If double dash alone, skip it and then stop reading options */
			if (strlen(av[i]) < 3) {
				i++;
				break ;
			}

			/* Builtins options */
			if (strcmp(&(av[i][2]), LIB_OPT_STRING_HELP) == 0)
				opt_help(opts, 0);
			else if (strcmp(&(av[i][2]), LIB_OPT_STRING_VERSION) == 0)
				p_version(0);

			/* Look for an argument */
			for (k = 2; av[i][k] != '\0' && av[i][k] != '='; k++)
				;

			if (av[i][k] != '\0') {
				got_arg = true;
				k -= 2;
			} else {
				k = strlen(av[i]) - 2;
			}

			/* Search the option in the opts array */
			for (it = 0; !IS_EOL(opts[it]) &&
						(strncmp(opts[it].s_opt, &(av[i][2]), k) != 0); it++)
				;

			/* Can't find the option */
			if (IS_EOL(opts[it])) {
				m_error("Unknown option %s\n", av[i]);
				opt_help(opts, 1);
			} else {
				if (opts[it].take_arg && !got_arg) {
					m_error("Option %s must take an argument", opts[it].s_opt);
					opt_help(opts, 1);
				}
				if (got_arg)
					opts[it].callback(&(av[i][k + 3]));
				else
					opts[it].callback(NULL);
				ret++;
			}
		/* Not beginning with a dash */
		} else {
			/* Stop reading options */
			list_add(*args, av[i], strlen(av[i]) + 1);
		}
	}
	/* If reading of flags is stopped by '-' or '--'
	 * get the rest of the parameters */
	for ( /* Using u32_t i */ ; i < (u32_t)ac; i++) {
		if (av[i] && strlen(av[i]))
			list_add(*args, av[i], strlen(av[i]) + 1);
	}
	return ret;
}

/*!
 * \brief Print helps with a list of argument, and exit
 * \param opts List of arguments to print
 * \param ret Return code of the exit
 */
void		opt_help(const mopts_t *opts, u8_t ret) {
	m_info("Help:\n");
	for (u32_t i = 0; opts[i].opt != 0; i++) {
		m_info("\t-%c | --%s : %s\n", opts[i].opt, opts[i].s_opt, opts[i].desc);
	}
	write(1, "\n", 1);
	m_info("If an argument requires a value, you can set it two ways:\n");
	m_info("\t-o value\n");
	m_info("\t--option=value\n");
	exit(ret);
}

/*!
 * \brief Print the program name, the version and the maintainer, then exit
 * \param ret Return code of the exit
 */
void		p_version(u8_t ret) {
	m_info("Program: %s\n", get_program_name());
	m_info("Version: %s\n", get_version());
	m_info("%s\n", get_maintainer());
	exit(ret);
}
