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
mlist_t		*list_add_member(mlist_t *list, void *member, u32_t size) {
	mlist_t		*n_member, *tmp;

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
	if (list == NULL) {
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

/*!
 * \brief Get list last member
 * \param list Head of the list
 * \return The last member of the list
 * \note If the list head is NULL, NULL is returned
 */
mlist_t		*list_get_last(mlist_t *list) {
	mlist_t		*tmp;

	if (list == NULL)
		return NULL;
	for (tmp = list; tmp->next; tmp = tmp->next)
		;
	return tmp;
}

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
mlist_t		*list_insert_after(mlist_t *org, mlist_t *ptr, void *member, u32_t size) {
	mlist_t		*n_member, *tmp, *tmp2;

	/* Allocate the new member */
	n_member = malloc(sizeof(mlist_t));
	n_member->member = malloc(size);
	assert(n_member && n_member->member);

	/* Copy the new member */
	memcpy(n_member->member, member, size);
	n_member->size = size;
	n_member->next = n_member->prev = NULL;

	/* If the list head is NULL, we return the new member */
	if (org == NULL) {
		n_member->head = n_member;
		return n_member;
	}

	/* Search for the given ptr */
	for (tmp = org; tmp->next && tmp != ptr; tmp = tmp->next)
		;

	/* If the ptr is not in the list, add the new member at the end of head */
	if (tmp->next == NULL) {
		tmp->next = n_member;
		n_member->prev = tmp;
	} else {
		tmp2 = tmp->next;
		tmp->next = n_member;
		n_member->prev = tmp;
		n_member->next = tmp2;
		tmp2->prev = n_member;
	}
	n_member->head = org;
	return org;
}

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
mlist_t		*list_insert_before(mlist_t *org, mlist_t *ptr, void *member, u32_t size) {
	mlist_t		*n_member, *tmp, *tmp2;

	/* Allocate the new member */
	n_member = malloc(sizeof(mlist_t));
	n_member->member = malloc(size);
	assert(n_member && n_member->member);

	/* Copy the new member */
	memcpy(n_member->member, member, size);
	n_member->size = size;
	n_member->next = n_member->prev = NULL;

	/* If the list is NULL, we return the new member */
	if (org == NULL) {
		n_member->head = n_member;
		return n_member;
	}

	/* Search for the given ptr */
	for (tmp = org; tmp->next != NULL && tmp != ptr; tmp = tmp->next)
		;

	/* If the ptr is not in the list, add the new member at the end of head */
	if (tmp->next == NULL) {
		tmp->next = n_member;
		n_member->prev = tmp;

	/* If the given ptr is the head, replace the head by the new member */
	} else if (ptr == org) {
		n_member->next = org;
		n_member->prev = NULL;
		org->prev = n_member;
		org = n_member;
	} else {
		tmp2 = tmp->prev;
		n_member->next = tmp;
		tmp->prev = n_member;
		tmp2->next = n_member;
	}
	n_member->head = org;
	return org;
}

/*!
 * \brief Return the size of a list
 * \param List head
 * \note If the list head is NULL, this function will return 0
 */
u32_t		list_size(mlist_t *list) {
	u32_t		i;
	mlist_t		*tmp;

	if (list == NULL)
		return 0;

	for (tmp = list, i = 0; tmp != NULL; tmp = tmp->next, i++)
		;
	return i;
}

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
mlist_t		*list_free(mlist_t *list, int (*free_fn)(void *member)) {
	mlist_t		*tmp;

	for (tmp = list; tmp != NULL; tmp = tmp->next) {
		if (free_fn(tmp->member) != 0) {
			free(tmp->member);
			free(tmp);
		} else {
			return tmp;
		}
	}
	return NULL;
}
