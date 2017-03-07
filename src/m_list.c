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

#include <m_list.h>

mlist_t *list_add_member(mlist_t *list, void *member, u32_t size) {
    mlist_t     *n_member, *tmp;

    if (member == NULL || size <= 0)
        return NULL;

    /* Allocate the list container and the new member */
    n_member = malloc(sizeof(mlist_t));
    n_member->member = malloc(size);
    assert(n_member && n_member->member);

    /* Copy the new member */
    memcpy(n_member->member, member, size);
    n_member->size = size;
    n_member->next = n_member->prev = NULL;

    /* If the list head is NULL, we return the new */
    if (list == NULL)
    {
        n_member->head = n_member;
        return n_member;
    }

    /* Else, we go the end of the list */
    for (tmp = list; tmp->next != NULL; tmp = tmp->next)
        ;
    tmp->next = n_member;
    n_member->prev = tmp;
    n_member->head = list;
    return list;
}

mlist_t *list_get_last(mlist_t *list)
{
    mlist_t     *tmp;

    if (list == NULL)
        return NULL;
    for (tmp = list; tmp->next; tmp = tmp->next)
        ;
    return tmp;
}

mlist_t *list_insert_after(mlist_t *org, mlist_t *ptr, void *member, u32_t size) {
    mlist_t     *n_member, *tmp, *tmp2;

    /* Allocate the new member */
    n_member = malloc(sizeof(mlist_t));
    n_member->member = malloc(size);
    assert(n_member && n_member->member);

    /* Copy the new member */
    memcpy(n_member->member, member, size);
    n_member->size = size;
    n_member->next = n_member->prev = NULL;

    /* If the list head is NULL, we return the new member */
    if (org == NULL)
    {
        n_member->head = n_member;
        return n_member;
    }

    /* Search for the given ptr */
    for (tmp = org; tmp->next && tmp != ptr; tmp = tmp->next)
        ;

    /* If the ptr is not in the list, add the new member at the end of head */
    if (tmp->next == NULL)
    {
        tmp->next = n_member;
        n_member->prev = tmp;
    }
    else
    {
        tmp2 = tmp->next;
        tmp->next = n_member;
        n_member->prev = tmp;
        n_member->next = tmp2;
        tmp2->prev = n_member;
    }
    n_member->head = org;
    return org;
}

mlist_t *list_insert_before(mlist_t *org, mlist_t *ptr, void *member, u32_t size) {
    mlist_t     *n_member, *tmp, *tmp2;

    /* Allocate the new member */
    n_member = malloc(sizeof(mlist_t));
    n_member->member = malloc(size);
    assert(n_member && n_member->member);

    /* Copy the new member */
    memcpy(n_member->member, member, size);
    n_member->size = size;
    n_member->next = n_member->prev = NULL;

    /* If the list is NULL, we return the new member */
    if (org == NULL)
    {
        n_member->head = n_member;
        return n_member;
    }

    /* Search for the given ptr */
    for (tmp = org; tmp->next != NULL && tmp != ptr; tmp = tmp->next)
        ;

    /* If the ptr is not in the list, add the new member at the end of head */
    if (tmp->next == NULL)
    {
        tmp->next = n_member;
        n_member->prev = tmp;
    }
    /* If the given ptr is the head, replace the head by the new member */
    else if (ptr == org)
    {
        n_member->next = org;
        n_member->prev = NULL;
        org->prev = n_member;
        org = n_member;
    }
    else
    {
        tmp2 = tmp->prev;
        n_member->next = tmp;
        tmp->prev = n_member;
        tmp2->next = n_member;
    }
    n_member->head = org;
    return org;
}

u32_t list_size(mlist_t *list) {
    u32_t       i;
    mlist_t     *tmp;

    if (list == NULL)
        return 0;

    for (tmp = list, i = 0; tmp != NULL; tmp = tmp->next, i++)
        ;
    return i;
}

mlist_t *list_free(mlist_t *list, int (*free_fn)(void *member)) {
    mlist_t     *tmp = list, *tmp2;

    while (tmp != NULL)
    {
        tmp2 = tmp->next;
        if (free_fn != NULL && free_fn(tmp->member) == 0)
        {
            return tmp;
        }
        else
        {
            free(tmp->member);
            free(tmp);
        }
        tmp = tmp2;
    }
    return NULL;
}

void *list_get(mlist_t *list, void *member, size_t size) {
    mlist_t     *tmp;
    void        *ptr;

    if (list == NULL)
        return NULL;
    list_for_each(list, tmp, ptr)
    {
        if (memcmp(ptr, member, size) == 0 && (size == tmp->size))
            return ptr;
    }
    return NULL;
}

mlist_t *list_remove(mlist_t *list, void *member, size_t size,
        int (*free_fn)(void *member)) {
    mlist_t     *tmp, *tmp2;
    void        *ptr;

    if (list == NULL)
        return NULL;

    /* Search for the member */
    list_for_each(list, tmp2, ptr)
    {
        if (memcmp(member, ptr, size) == 0)
            break ;
    }

    /* We can't find the member */
    if (tmp2 == NULL)
        return list;

    if (tmp2 == list)
    {
        /* Replace the head */
        list = tmp2->next;

        /* Update the head in all other members */
        list_for_each(list, tmp, ptr)
        {
            tmp->head = list;
        }
    }
    else
    {
        tmp2->prev->next = tmp2->next;
    }

    if (free_fn != NULL)
        free_fn(tmp2->member);
    else
        free(tmp2->member);

    free(tmp2);
    return list;
}
