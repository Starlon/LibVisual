/*	List implementation from RCL.
 *	
 *	Copyright (C) 2002,2003,2004
 *				Dennis Smit <synap@nerds-incorporated.org>,
 *			  	Sepp Wijnands <mrrazz@nerds-incorporated.org>,
 *			   	Tom Wimmenhove <nohup@nerds-incorporated.org>
 *
 *	$Id: lv_list.c,v 1.12 2004-09-28 15:14:00 synap Exp $
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with this program; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <lvconfig.h>
#include "lv_list.h"
#include "lv_log.h"
#include "lv_mem.h"

/**
 * @defgroup VisList VisList
 * @{
 */

/**
 * Creates a new VisList structure.
 * The VisList system is a double linked list implementation.
 *
 * @return A newly allocated VisList.
 */
VisList *visual_list_new ()
{
	VisList *list;

	list = visual_mem_new0 (VisList, 1);

	return list;
}

/**
 * Frees the VisList. This frees the VisList data structure.
 *
 * @param list Pointer to a VisList that needs to be freed.
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_free (VisList *list)
{
	visual_log_return_val_if_fail (list != NULL, -1);

	visual_mem_free (list);

	list = NULL;

	return 0;
}

/**
 * Destroys the entries that are in a list, but not the list itself. If the destroyer
 * argument isn't NULL this function is called with a pointer to the data element of
 * a list entry. You can use 'free' as an argument here is it's sufficient as a
 * destroyer function, or if you don't want to free the data entries at all pass
 * NULL.
 *
 * @param list Pointer to a VisList of which the elements need to be destroyed.
 * @param destroyer Pointer to a destroyer function that is used to destroy the data
 * 	in the elements
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_destroy_elements (VisList *list, visual_list_destroy_func_t destroyer)
{
	VisListEntry *le = NULL;
	void *elem;

	visual_log_return_val_if_fail (list != NULL, -1);
		
	/* Walk through the given list, possibly calling the destroyer for it */
	if (destroyer == NULL) {
		while ((elem = visual_list_next (list, &le)) != NULL)
			visual_list_delete (list, &le);
	} else {
		while ((elem = visual_list_next (list, &le)) != NULL) {
			destroyer (elem);
			visual_list_delete (list, &le);
		}
	}

	return 0;
}

/**
 * Destroys a list and it's entries. This destroys a list and when the destroyer
 * argument isn't NULL this function is called with a pointer to the data element
 * of a list entry. You can use 'free' as an argument here when this is sufficient
 * as a destroyer function, or if you don't want to free the data entries at all pass
 * NULL.
 *
 * @param list Pointer to a VisList that needs to be destroyed.
 * @param destroyer Pointer to a destroyer function that is used to destroy the data
 * 	in the elements
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_destroy (VisList *list, visual_list_destroy_func_t destroyer)
{
	visual_log_return_val_if_fail (list != NULL, -1);
	
	visual_list_destroy_elements (list, destroyer);	
	
	visual_list_free (list);

	return 0;
}

/**
 * Go to the next entry in the list and return it's data element.
 * This function will load the next entry in le and return a pointer
 * to the data element.
 *
 * @see visual_list_prev
 * 
 * @param list Pointer to the VisList we're traversing.
 * @param le Pointer to a VisListEntry to store the next entry within
 * 	and also to use as a reference to determine at which entry we're
 * 	currently. To begin traversing do: VisListEntry *le = NULL and pass
 * 	it as &le in the argument.
 *
 * @return The data element of the next entry, or NULL.
 */
void *visual_list_next (VisList *list, VisListEntry **le)
{
	visual_log_return_val_if_fail (list != NULL, NULL);
	visual_log_return_val_if_fail (le != NULL, NULL);

	if (*le == NULL)
		*le = list->head;
	else
		*le = (*le)->next;

	if (*le != NULL)
		return (*le)->data;

	return NULL;
}

/**
 * Go to the previous entry in the list and return it's data element.
 * This function will load the previous entry in le and return a pointer
 * to the data element.
 *
 * @see visual_list_next
 * 
 * @param list Pointer to the VisList we're traversing.
 * @param le Pointer to a VisListEntry to store the previous entry within
 * 	and also to use as a reference to determine at which entry we're
 * 	currently. To begin traversing at the end of the list do:
 * 	VisList *le = NULL and pass it as &le in the argument.
 *
 * @return The data element of the previous entry, or NULL.
 */
void *visual_list_prev (VisList *list, VisListEntry **le)
{
	visual_log_return_val_if_fail (list != NULL, NULL);
	visual_log_return_val_if_fail (le != NULL, NULL);

	if (!*le)
		*le = list->tail;
	else
		*le = (*le)->prev;

	if (*le)
		return (*le)->data;

	return NULL;
}

/**
 * Get an data entry by index. This will give the pointer to an data
 * element based on the index in the list.
 *
 * @param list Pointer to the VisList of which we want an element.
 * @param index Index to determine which entry we want. The index starts at
 * 	1.
 *
 * @return The data element of the requested entry, or NULL.
 */
void *visual_list_get (VisList *list, int index)
{
	VisListEntry *le = NULL;
	void *data = NULL;
	int i, lc;

	visual_log_return_val_if_fail (list != NULL, NULL);
	visual_log_return_val_if_fail (index >= 0, NULL);

	lc = visual_list_count (list);

	if (lc - 1 < index)
		return NULL;
	
	for (i = 0; i <= index; i++) {
		data = visual_list_next (list, &le);
		
		if (data == NULL)
			return NULL;
	}

	return data;
}

/**
 * Adds an entry at the beginning of the list.
 *
 * @param list Pointer to the VisList to which an entry needs to be added
 * 	at it's head.
 * @param data A pointer to the data that needs to be added to the list.
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_add_at_begin (VisList *list, void *data)
{
	VisListEntry *current, *next;

	visual_log_return_val_if_fail (list != NULL, -1);

	/* Allocate memory for new list entry */
	current = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	current->data = data;

	if (list->head == NULL) {
		list->head = current;
		list->tail = current;
	} else {
		next = list->head;

		current->next = next;
		list->head = current;
	}

	/* Done */
	list->count++;

	return 0;
}

/**
 * Adds an entry at the end of the list.
 *
 * @param list Pointer to the VisList to which an entry needs to be added
 * 	at it's tail.
 * @param data A pointer to the data that needs to be added to the list.
 *
 * @return 0 on succes -1 on error.
 */	
int visual_list_add (VisList *list, void *data)
{
	VisListEntry *current, *prev;
	
	visual_log_return_val_if_fail (list != NULL, -1);

	current = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	current->data = data;

	/* Add list entry to list */
	/* Is this the first entry for this list ? */
	if (list->head == NULL) {
		list->head = current;
		list->tail = current;
	} else {
		/* Nope, add to tail of this list */
		prev = list->tail;

		/* Exchange pointers */
		prev->next = current;
		current->prev = prev;
		
		/* Point tail to new entry */
		list->tail = current;
	}

	/* Done */
	list->count++;

	return 0;
}

/**
 * Insert an entry in the middle of a list. By adding it
 * after the le entry.
 *
 * @param list Pointer to the VisList in which an entry needs to be inserted.
 * @param le Pointer to a VisListEntry after which the entry needs to be inserted.
 * @param data Pointer to the data the new entry represents.
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_insert (VisList *list, VisListEntry **le, void *data)
{
	VisListEntry *prev, *next, *current;
	
	visual_log_return_val_if_fail (list != NULL, -1);
	visual_log_return_val_if_fail (le != NULL, -1);
	visual_log_return_val_if_fail (data != NULL, -1);
	
	current = visual_mem_new0 (VisListEntry, 1);

	/* Assign data element */
	current->data = data;

	/* Add entry to list */
	if (list->head == NULL && *le == NULL) {
		/* First entry */
		list->head = current;
		list->tail = current;
	} else if (*le == NULL) {
		/* Insert entry at first position */
		next = list->head;
		/* Exchange pointers */
		current->next = next;
		next->prev = current;
		/* Point head to current pointer */
		list->head = current;
	} else {
		/* Insert entry at *le's position */
		prev = *le;
		next = prev->next;
		
		current->prev = prev;
		current->next = next;

		prev->next = current;
		if (next != NULL)
			next->prev = current;
		else
			list->tail = current;
	}

	/* Hop to new entry */
	*le = current;
	
	/* Done */
	list->count++;
	return 0;
}

/**
 * Removes an entry from the list.
 *
 * @param list A pointer to the VisList in which an entry needs to be deleted.
 * @param le A pointer to the entry that needs to be deleted.
 *
 * @return 0 on succes -1 on error.
 */
int visual_list_delete (VisList *list, VisListEntry **le)
{
	VisListEntry *prev, *current, *next;
	
	visual_log_return_val_if_fail (list != NULL, -1);
	visual_log_return_val_if_fail (le != NULL, -1);
	
	prev = current = next = NULL;

	/* Valid list entry ? */
	if (*le == NULL) {
		visual_log (VISUAL_LOG_CRITICAL, "There is no list entry to delete");
		return -1; /* Nope */
	}

	/* Point new to le's previous entry */
	current = *le;
	prev = current->prev;
	next = current->next;

	/* Does it have a previous entry ? */
	if (prev != NULL) 
		prev->next = next;
	else
		list->head = next;
	
	if (next != NULL) /* It does have a next entry ? */
		next->prev = prev;
	else
		list->tail = prev;

	/* Point current entry to previous one */
	*le = prev;

	/* Free 'old' pointer */
	list->count--;
	visual_mem_free (current);

	return 0;
}

/**
 * Counts the number of entries within the list.
 *
 * @param list A pointer to the list from which an entry count is needed.
 * 
 * @return The number of elements or -1 on error.
 */
int visual_list_count (VisList *list)
{
	VisListEntry *le = NULL;
	int count = 0;
	
	visual_log_return_val_if_fail (list != NULL, -1);
	
	/* Walk through list */
	while (visual_list_next (list, &le) != NULL) 
		count++;

	list->count = count;

	return count;
}

/**
 * @}
 */
