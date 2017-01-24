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

/* Linked list */
typedef struct		s_list {
	void				*member;	/* Actual member */
	u32_t				size; 		/* Size of the member */
	struct s_list		*next;		/* Next in list */
	struct s_list		*prev;		/* Prev in list */
	struct s_list		*head;		/* Head of the list */
}					mlist_t;


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


/* Functions */
mlist_t		*list_add_member(mlist_t *list, void *member, u32_t size);
mlist_t		*list_get_last(mlist_t *list);
mlist_t		*list_insert_after(mlist_t *org, mlist_t *ptr, void *member, u32_t size);
mlist_t		*list_insert_before(mlist_t *org, mlist_t *ptr, void *member, u32_t size);
u32_t		list_size(mlist_t *list);
mlist_t		*list_free(mlist_t *list, int (*free_fn)(void *member));
void		*list_get(mlist_t *list, void *member, size_t size);
mlist_t		*list_remove(mlist_t *list, void *member, size_t size,
					int (*free_fn)(void *member));

#endif
