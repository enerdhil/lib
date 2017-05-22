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

#include <m_util.h>
#include <fts.h>

bool recursive_delete(const char *dir) {
    FTS         *ftsp = NULL;
    FTSENT      *curr = NULL;
    const char  *files[] = { (char *)dir, NULL };

    ftsp = fts_open((char * const *)files, FTS_NOCHDIR | FTS_PHYSICAL | FTS_XDEV, NULL);
    assert(ftsp != NULL);

    while ((curr = fts_read(ftsp))) {
        switch (curr->fts_info) {
#ifndef COMPILE_WITH_TEST
            case FTS_NS:
            /* FALLTROUGH */
            case FTS_DNR:
            /* FALLTROUGH */
            case FTS_ERR:
                goto error;

            case FTS_DC:
            /* FALLTROUGH */
            case FTS_DOT:
            /* FALLTROUGH */
            case FTS_NSOK:
                break ;
#endif /* COMPILE_WITH_TEST */

            case FTS_DP:
            /* FALLTROUGH */
            case FTS_F:
            /* FALLTROUGH */
            case FTS_SL:
            /* FALLTROUGH */
            case FTS_SLNONE:
            /* FALLTROUGH */
            case FTS_DEFAULT:
                if (remove(curr->fts_accpath) < 0)
                    goto error;
                break;
        }
    }

    fts_close(ftsp);
    return true;

error:
    fts_close(ftsp);
    return false;
}

MPX_STATIC char **str_list_to_array(mlist_t *list) {
    char    **arr = NULL, *ptr = NULL;
    mlist_t *tmp;
    size_t  i = 0;

    arr = malloc(sizeof(char *) * (list_size(list) + 1));
    if (arr == NULL)
        return NULL;

    list_for_each(list, tmp, ptr) {
        arr[i] = strdup(ptr);
        arr[i + 1] = NULL;
        if (arr[i] == NULL)
            goto end;
        i++;
    }

    return arr;

end:
    for (i = 0; arr[i] != NULL; i++)
        free(arr[i]);
    free(arr);
    return NULL;
}

int exec_line(const char *str) {
    mlist_t     *list = NULL;
    char        *cmd;
    size_t      j, i, len;
    int         ret;

    if (str == NULL)
        return 0;

    cmd = strdup(str);
    if (cmd == NULL)
        return 0;

    len = strlen(cmd);
    for (i = 0, j = 0; i < len; i++)
    {
        if (cmd[i] == ' ')
        {
            while (i < len && cmd[i] == ' ')
                i++;

            i--;
            cmd[i] = '\0';
            list_add(list, cmd + j, i - j + 1);
            cmd[i] = ' ';
            j = i + 1;
        }
    }
    list_add(list, cmd + j, i - j + 1);

    ret = exec_list(list);
    list_free(list, NULL);
    free(cmd);
    return ret;
}

int exec_list(mlist_t *list) {
    char    **tab = NULL;
    int     pid, status = 1;

    tab = str_list_to_array(list);

    if (tab == NULL)
        goto end;

    if ((pid = fork()) == -1)
        goto end;

    if (pid == 0)
    {
        execvp(tab[0], tab);
        exit(1);
    }
    else
    {
        waitpid(pid, &status, 0);
    }

end:
    for (size_t i = 0; tab[i]; i++)
        free(tab[i]);
    free(tab);
    return status;
}
