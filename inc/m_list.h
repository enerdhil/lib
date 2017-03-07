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

#ifndef M_LIST_H
# define M_LIST_H

# include <m_types.h>
# include <stdlib.h> /* malloc */
# include <assert.h> /* assert */
# include <string.h> /* mem{cpy,move,cmp} */
# include <morphux.h>

/* Linked list */
typedef struct list_s {
    void                *member;    /*!< Actual member */
    u32_t               size;       /*!< Size of the member */
    struct list_s       *next;      /*!< Next in list */
    struct list_s       *prev;      /*!< Prev in list */
    struct list_s       *head;      /*!< Head of the list */
} mlist_t;


/* Defines */
# define list_add(ptr, member, sizeZ) ptr = list_add_member(ptr, member, sizeZ);

# define list_for_each(org_list, temp, p_tr)\
            for (temp = org_list, p_tr = temp->member;\
            temp && (p_tr = temp->member); temp = temp->next)

# define list_for_each_rev(org_list, temp, p_tr)\
            for (temp = list_get_last(org_list), p_tr = temp->member;\
            temp != temp->head && (p_tr = temp->member); temp = temp->prev)

# define list_tail(org_list) list_get_last(org_list);

# define list_add_after(org_list, p_tr1, p_tr2, sizeZ) org_list = list_insert_after(org_list, p_tr1, p_tr2, sizeZ);

# define list_add_before(org_list, p_tr1, p_tr2, sizeZ) org_list = list_insert_before(org_list, p_tr1, p_tr2, sizeZ);

# define list_del(org_list, p_tr1, sizeZ, fn) org_list = list_remove(org_list, p_tr1, sizeZ, fn);


/*!
 * \brief Add a new member to a linked list
 * \param list Head of a linked list
 * \param member New member to add
 * \param size Size of the new member
 *
 * This function will add the member at the end of the list. If the list
 * pointer is null, a new list head is returned.
 * Member is copied with a memcpy, in a pre-allocated pointer of size.
 *
 * \note If member is NULL, NULL is returned
 * \note If size is equal to 0, NULL is returned
 * \note Member is _not freed_ by this function
 */
mlist_t *list_add_member(mlist_t *list, void *member, u32_t size);

/*!
 * \brief Get list last member
 * \param list Head of the list
 * \return The last member of the list
 * \note If the list head is NULL, NULL is returned
 */
mlist_t *list_get_last(mlist_t *list);

/*!
 * \brief Insert a new member after a given existing member in a list
 * \param org List head
 * \param ptr Pointer used to add member after
 * \param member New member to add
 * \param size Size of the new member
 *
 * This function will try to add a new member after a given ptr.
 * If list head is NULL, a new list is returned
 * If the given pointer is not in the list, the new member is added at the end
 */
mlist_t *list_insert_after(mlist_t *org, mlist_t *ptr, void *member, u32_t size);

/*!
 * \brief Insert a new member before a given existing member in a list
 * \param org List head
 * \param ptr Pointer used to add member after
 * \param member New member to add
 * \param size Size of the new member
 *
 * This function will try to add a new member before a given ptr
 * If list head is NULL, a new list is returned
 * If the given ptr is the current head, the head is updated with the new
 * member.
 * If the given ptr is not in the list, the member is added at the end
 */
mlist_t *list_insert_before(mlist_t *org, mlist_t *ptr, void *member, u32_t size);

/*!
 * \brief Return the size of a list
 * \param list List head
 * \note If the list head is NULL, this function will return 0
 */
u32_t list_size(mlist_t *list);

/*!
 * \brief Free a list
 * \param list List head
 * \param free_fn Function pointer to free the member
 *
 * This function will try to free a linked list.
 * free_fn function pointer is used to free the members.
 * This function must return something besides 0 in order to this function to
 * carry on.
 * If the free_fn function return 0, this function will stop, and return the 
 * current not-freed pointer.
 *
 * \return If the list has been entirely freed, this function will return NULL
 */
mlist_t *list_free(mlist_t *list, int (*free_fn)(void *member));

/*!
 * \brief Search a member in a list
 * \param list List head
 * \param member Member to search
 * \param size Size of the member
 *
 * Search member in list.
 * If member is found, return a pointer to it
 * If not, NULL is returned
 */
void *list_get(mlist_t *list, void *member, size_t size);

/*!
 * \brief Remove a member from the list
 * \param list List head
 * \param member Member to remove
 * \param size Size of the member (Used for memcmp)
 * \param free_fn Function use to free the member
 *
 * Remove a member in a list.
 */
mlist_t *list_remove(mlist_t *list, void *member, size_t size,
        int (*free_fn)(void *member));

#endif
