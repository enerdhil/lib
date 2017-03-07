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
#define LIB_OPT_TOKEN_HELP 'h'
#define LIB_OPT_STRING_HELP "help"
#define LIB_OPT_TOKEN_VERSION 'V'
#define LIB_OPT_STRING_VERSION "version"

u32_t		read_opt(const int ac, char **av, const margs_t *args) {
    u32_t		ret = 0, it, k;
    u8_t		n_dash;

    if (ac == 0 || av == NULL || args == NULL)
        return ret;

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

            /* Builtins options */
            if (av[i][1] == LIB_OPT_TOKEN_HELP)
                opt_help(args, 0);
            else if (av[i][1] == LIB_OPT_TOKEN_VERSION)
                p_version(0);

            /* Search the option in the args array */
            for (u32_t z = 1; av[i][z] != '\0'; z++) {
                for (it = 0; !IS_EOL(args[it]) && args[it].opt != av[i][z]; it++)
                    ;

                /* Can't find the option */
                if (IS_EOL(args[it])) {
                    m_error("Unknow option -%s\n", &(av[i][z]));
                    opt_help(args, 1);
                } else {
                    if (args[it].take_arg) {
                        if (i + 1 < (u32_t)ac) {
                            args[it].callback(av[++i]);
                            ret++;
                            break ;
                        } else {
                            m_error("Option -%c must take an argument\n",
                                    args[it].opt);
                            opt_help(args, 1);
                        }
                    } else {
                        args[it].callback(NULL);
                        ret++;
                    }
                }
            }

            /* Word option */
        } else if (n_dash == 2) {
            bool	got_arg = false;

            if (strlen(av[i]) < 3) {
                m_error("Double dash without option. Ignoring...\n");
                continue ;
            }

            /* Builtins options */
            if (strcmp(&(av[i][2]), LIB_OPT_STRING_HELP) == 0)
                opt_help(args, 0);
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

            /* Search the option in the args array */
            for (it = 0; !IS_EOL(args[it]) &&
                    (strncmp(args[it].s_opt, &(av[i][2]), k) != 0); it++)
                ;

            /* Can't find the option */
            if (IS_EOL(args[it])) {
                m_error("Unknown option %s\n", av[i]);
                opt_help(args, 1);
            } else {
                if (args[it].take_arg && !got_arg) {
                    m_error("Option %s must take an argument", args[it].s_opt);
                    opt_help(args, 1);
                }
                if (got_arg)
                    args[it].callback(&(av[i][k + 3]));
                else
                    args[it].callback(NULL);
                ret++;
            }
        }
    }
    return ret;
}

void		opt_help(const margs_t *args, u8_t ret) {
    m_info("Help:\n");
    for (u32_t i = 0; args[i].opt != 0; i++) {
        m_info("\t-%c | --%s : %s\n", args[i].opt, args[i].s_opt, args[i].desc);
    }
    write(1, "\n", 1);
    m_info("If an argument requires a value, you can set it two ways:\n");
    m_info("\t-o value\n");
    m_info("\t--option=value\n");
    exit(ret);
}

void		p_version(u8_t ret) {
    m_info("Program: %s\n", get_program_name());
    m_info("Version: %s\n", get_version());
    m_info("%s\n", get_maintainer());
    exit(ret);
}
