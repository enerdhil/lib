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
# include <stdlib.h>
# include <errno.h>
# include <m_types.h>
# include <m_print.h>
# include <m_infos.h>
# include <m_list.h>
# include <morphux.h>

typedef struct      s_opts {
    /**
     * Single letter option
     * Example: -f, -s
     */
    char    opt;

    /**
     * Full string option
     * Example: --force, --skip
     */
    char    *s_opt;

    /**
     * Description of the option.
     * Used for the help
     */
    char    *desc;

    /**
     * Boolean that describe if the option must take an argument
     */
    bool    take_arg;

    /**
     * Callback of the option
     */
    void    (*callback)(const char *);
}                   mopts_t;

#define ARGS_EOL {0, NULL, NULL, false, NULL}
#define IS_EOL(lst) (lst.opt == 0 && lst.s_opt == NULL && lst.desc == NULL && \
                        lst.take_arg == false && lst.callback == NULL)

/*!
 * \brief Read the options given by the program
 * \param[in] ac Number of argument in av
 * \param[in] av Array of string, containing the arguments
 * \param[in] args Array of margs_t, containing the preset options. Must end with
 * an empty structure.
 *
 * The read_opt function reads a given list of arguments, and parse the options
 * in it. The options are read from the args array.
 * If an option is not known, the function calls the help and quit.
 * If the option -h | --help is passed, the function call the help and quit.
 * If the option -v | --version is passed, the function call the version and quit.
 *
 * \note Only the arguments beginning with - are parsed.
 * \return Number of options read
 */
u32_t read_opt(const int ac, char **av, const margs_t *args);

/*!
 * \brief Print helps with a list of argument, and exit
 * \param[in] args List of arguments to print
 * \param[in] ret Return code of the exit
 */
void opt_help(const margs_t *args, u8_t ret);

/*!
 * \brief Print the program name, the version and the maintainer, then exit
 * \param[in] ret Return code of the exit
 */
void p_version(u8_t ret);

#endif /* M_ARGS_H */
