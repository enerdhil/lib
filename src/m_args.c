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

u32_t read_opt(const int ac, char **av, const mopts_t *opts, mlist_t **args) {
    u32_t       ret = 0, i = 1, it, k;
    u8_t        n_dash;

    if (ac == 0 || av == NULL || opts == NULL || args == NULL)
        return ret;

    for (/* Using u32_t i */ u32_t j = 0; i < (u32_t)ac; i++)
    {

        if (av[i] == NULL || strlen(av[i]) == 0)
            continue ;

        n_dash = 0;
        for (j = 0; av[i][j] != '\0' && av[i][j] == '-'; j++)
            n_dash++;

        /* Single letter option */
        if (n_dash == 1)
        {

            /* If single dash alone, its a parameter */
            if (strlen(av[i]) < 2)
            {
                list_add(*args, av[i], strlen(av[i]) + 1);
                continue ;
            }

            /* Builtins options */
            if (av[i][1] == LIB_OPT_TOKEN_HELP)
                opt_help(opts, 0);
            else if (av[i][1] == LIB_OPT_TOKEN_VERSION)
                p_version(0);

            /* Search the option in the opts array */
            for (u32_t z = 1; av[i][z] != '\0'; z++)
            {
                for (it = 0; !IS_EOL(opts[it]) && opts[it].opt != av[i][z]; it++)
                    ;

                /* Can't find the option */
                if (IS_EOL(opts[it]))
                {
                    m_error("Unknow option -%s\n", &(av[i][z]));
                    opt_help(opts, 1);
                }
                else
                {
                    if (opts[it].take_arg)
                    {
                        if (i + 1 < (u32_t)ac)
                        {
                            if (!opts[it].callback(av[++i]))
                            {
                                m_error("Wrong value \"%s\" for argument -%c\n",
                                    av[i], opts[it].opt);
                                usage(opts);
                                exit(1);
                            }
                            ret++;
                            break ;
                        }
                        else
                        {
                            m_error("Option -%c must take an argument\n",
                                    opts[it].opt);
                            opt_help(opts, 1);
                        }
                    }
                    else
                    {
                        opts[it].callback(NULL);
                        ret++;
                    }
                }
            }
        /* Word option */
        }
        else if (n_dash == 2)
        {
            bool    got_arg = false;

            /* If double dash alone, skip it and then stop reading options */
            if (strlen(av[i]) < 3)
            {
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

            if (av[i][k] != '\0')
            {
                got_arg = true;
                k -= 2;
            }
            else
            {
                k = strlen(av[i]) - 2;
            }

            /* Search the option in the opts array */
            for (it = 0; !IS_EOL(opts[it]) &&
                    (strncmp((opts[it].s_opt != NULL ? opts[it].s_opt : ""), &(av[i][2]), k) != 0); it++)
                ;

            /* Can't find the option */
            if (IS_EOL(opts[it]))
            {
                m_error("Unknown option %s\n", av[i]);
                opt_help(opts, 1);
            }
            else
            {
                if (opts[it].take_arg && !got_arg)
                {
                    m_error("Option %s must take an argument", opts[it].s_opt);
                    opt_help(opts, 1);
                }
                char    *arg = NULL;

                if (got_arg)
                    arg = &(av[i][k + 3]);
                if (!opts[it].callback(arg))
                {
                    m_error("Wrong value \"%s\" for argument --%s\n",
                        arg != NULL ? arg : "", opts[it].s_opt);
                    usage(opts);
                    exit(1);
                }
                ret++;
            }
            /* Not beginning with a dash */
        }
        else
        {
            /* Stop reading options */
            list_add(*args, av[i], strlen(av[i]) + 1);
        }
    }
    /* If reading of flags is stopped by '--' get the rest of the parameters */
    for ( /* Using u32_t i */ ; i < (u32_t)ac; i++)
    {
        if (av[i] && strlen(av[i]))
            list_add(*args, av[i], strlen(av[i]) + 1);
    }
    return ret;
}

void opt_help(const mopts_t *opts, u8_t ret) {
    size_t        pad = 0;

    usage(opts);
    m_info("Help:\n");
    for (u32_t i = 0; opts[i].opt != 0; i++)
    {
        if (opts[i].s_opt != NULL && (strlen(opts[i].s_opt) + 2) > pad)
            pad = strlen(opts[i].s_opt) + 6;
    }
    for (u32_t i = 0; opts[i].opt != 0; i++)
    {
        if (opts[i].s_opt != NULL)
            m_info("\t-%c, --%s%*s%s\n", opts[i].opt, opts[i].s_opt, pad - (strlen(opts[i].s_opt)), "", opts[i].desc);
        else
            m_info("\t-%c%*s%s\n", opts[i].opt, pad + 4, "", opts[i].desc);
    }
    m_info("\t-%c, --%s%*s%s\n", LIB_OPT_TOKEN_HELP, LIB_OPT_STRING_HELP,
        pad - (strlen(LIB_OPT_STRING_HELP)), "", "Show this help");
    m_info("\t-%c, --%s%*s%s\n", LIB_OPT_TOKEN_VERSION, LIB_OPT_STRING_VERSION,
        pad - (strlen(LIB_OPT_STRING_VERSION)), "", "Print the current version");
    exit(ret);
}

void p_version(u8_t ret) {
    m_info("Program: %s\n", get_program_name());
    m_info("Version: %s\n", get_version());
    m_info("%s\n", get_maintainer());
    exit(ret);
}

void usage(const mopts_t *opts) {
    u8_t        i;

    m_info("Usage: %s [-%c%c", get_program_name(), LIB_OPT_TOKEN_HELP,
        LIB_OPT_TOKEN_VERSION);
    fflush(stdout);
    for (i = 0; !IS_EOL(opts[i]); i++)
    {
        if (opts[i].take_arg == false && opts[i].opt != '\0')
        {
            write(1, &opts[i].opt, 1);
        }
    }
    fprintf(stdout, "] ");

    for (i = 0; !IS_EOL(opts[i]); i++)
    {
        if (opts[i].take_arg == true && opts[i].usage != NULL)
        {
            if (opts[i].opt != '\0')
                fprintf(stdout, "[-%c %s] ", opts[i].opt, opts[i].usage);
            else if (opts[i].s_opt)
                fprintf(stdout, "[--%s=%s] ", opts[i].s_opt, opts[i].usage);
        }
    }

    for (i = 0; !IS_EOL(opts[i]); i++)
    {
        if (opts[i].take_arg == false && opts[i].opt == '\0')
        {
            fprintf(stdout, "[--%s] ", opts[i].s_opt);
        }
    }
    fprintf(stdout, "\n");
}
