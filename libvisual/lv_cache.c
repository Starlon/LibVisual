/* Libvisual - The audio visualisation framework.
 * 
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id: lv_cache.c,v 1.12 2006-09-20 19:26:07 synap Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lv_common.h"
#include "lv_cache.h"

static int cache_dtor (VisObject *object);
static int cache_remove_list_entry (VisCache *cache, VisListEntry **le);

static inline void handle_request_reset (VisCache *cache, VisListEntry *le);

static int cache_dtor (VisObject *object)
{
	VisCache *cache = VISUAL_CACHE (object);
	VisListEntry *le = NULL;

	/* Destroy all entries in cache first */
	while (visual_list_next (cache->list, &le) != NULL)
		cache_remove_list_entry (cache, &le);

	/* Destroy the rest */
	if (cache->list != NULL)
		visual_object_unref (VISUAL_OBJECT (cache->list));

	if (cache->index != NULL)
		visual_object_unref (VISUAL_OBJECT (cache->index));

	cache->list = NULL;
	cache->index = NULL;

	return TRUE;
}

static int cache_remove_list_entry (VisCache *cache, VisListEntry **le)
{
	VisCacheEntry *centry;

	centry = (*le)->data;

	if (centry->key != NULL)
		visual_hashmap_remove_string (cache->index, centry->key, FALSE);

	if (cache->destroyer != NULL)
		cache->destroyer (centry->data);

	visual_list_destroy (cache->list, le);

	return VISUAL_OK;
}


static inline void handle_request_reset (VisCache *cache, VisListEntry *le)
{
	VisCacheEntry *centry;

	if (cache->reqreset == FALSE)
		return;

	centry = le->data;
	visual_timer_start (&centry->timer);

	/* Unchain from the list */
	visual_list_unchain (cache->list, le);

	/* Rechain at the head */
	visual_list_chain_at_begin (cache->list, le);
}

/**
 * @defgroup VisCache VisCache
 * @{
 */

/**
 * Creates a new VisCache.
 *
 * @param destroyer The collection destroyer that is used to destroy the individual members.
 * @param size The number of elements allowed in the cache.
 * @param maxage The maximum age of cache elemnts. A copy is made, not a reference.
 * @param reqreset Reset position and age of an element that is requested.
 *
 * @return A newly allocated VisCache.
 */
VisCache *visual_cache_new (VisCollectionDestroyerFunc destroyer, int size, VisTime *maxage, int reqreset)
{
	VisCache *cache;

	cache = visual_mem_new0 (VisCache, 1);

	visual_cache_init (cache, destroyer, size, maxage, reqreset);

	/* Do the VisObject initialization */
	visual_object_set_allocated (VISUAL_OBJECT (cache), TRUE);
	visual_object_ref (VISUAL_OBJECT (cache));

	return cache;
}

/**
 * Initializes a VisCache structure.
 *
 * @param destroyer The collection destroyer that is used to destroy the individual members.
 * @param size The number of elements allowed in the cache.
 * @param maxage The maximum age of cache elemnts. A copy is made, not a reference.
 * @param reqreset Reset position and age of an element that is requested.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL on failure.
 */
int visual_cache_init (VisCache *cache, VisCollectionDestroyerFunc destroyer, int size, VisTime *maxage, int reqreset)
{
	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);

	/* Do the VisObject initialization */
	visual_object_clear (VISUAL_OBJECT (cache));
	visual_object_set_dtor (VISUAL_OBJECT (cache), cache_dtor);
	visual_object_set_allocated (VISUAL_OBJECT (cache), FALSE);

	/* Set the VisCache data */
	visual_cache_set_limits (cache, size, maxage);
	cache->destroyer = destroyer;
	cache->reqreset = reqreset;

	cache->list = visual_list_new (NULL);

	cache->index = visual_hashmap_new (NULL); /* FIXME create in set_limits, rehash if not NULL */
	visual_hashmap_set_table_size (cache->index, size); /* <- also */

	return VISUAL_OK;
}

/**
 * Clears the complete cache of cache elements.
 *
 * @param cache Pointer to the VisCache that is to be cleared.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL on failure.
 */
int visual_cache_clear (VisCache *cache)
{
	VisListEntry *le = NULL;

	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);

	/* Destroy all entries in cache first */
	while (visual_list_next (cache->list, &le) != NULL)
		cache_remove_list_entry (cache, &le);

	if (cache->index != NULL)
		visual_object_unref (VISUAL_OBJECT (cache->index));

	cache->index = visual_hashmap_new (NULL);
	visual_hashmap_set_table_size (cache->index, cache->size);

	return VISUAL_OK;
}

/**
 * Flushes all outdated cache entries, this only works when a max age has been set while creating the cache.
 *
 * @param cache Pointer to the VisCache that is cleared of outdated entries.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL on failure.
 */
int visual_cache_flush_outdated (VisCache *cache)
{
	VisCacheEntry *centry;
	VisListEntry *le;

	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);

	if (cache->withmaxage == TRUE) {
		le = cache->list->tail;

		if (le == NULL)
			return VISUAL_OK;

		centry = le->data;

		while (visual_timer_elapsed (&centry->timer, &cache->maxage)) {
			cache_remove_list_entry (cache, &le);

			le = cache->list->tail;

			if (le == NULL)
				return VISUAL_OK;

			centry = le->data;
		}
	}

	return VISUAL_OK;
}

/**
 * Puts a new entry in the cache.
 *
 * @param cache Pointer to the VisCache in which an entry is added.
 * @param key The key that links to this entry.
 * @param data The cached data.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL or -VISUAL_ERROR_NULL on failure.
 */
int visual_cache_put (VisCache *cache, char *key, void *data)
{
	VisCacheEntry *centry;
	VisListEntry *le;

	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);
	visual_log_return_val_if_fail (key != NULL, -VISUAL_ERROR_NULL);
	visual_log_return_val_if_fail (data != NULL, -VISUAL_ERROR_NULL);

	/* Don't try to put in a 0 sized cache */
	if (cache->size < 1)
		return VISUAL_OK;

	/* Remove items that are no longer wished in the cache */
	while (visual_collection_size (VISUAL_COLLECTION (cache->list)) > cache->size - 1) {
		le = cache->list->tail;

		if (le == NULL)
			return VISUAL_OK;

		cache_remove_list_entry (cache, &le);
	}

	/* Remove items that are out dated */
	visual_cache_flush_outdated (cache);

	/* Add to cache */
	le = visual_hashmap_get_string (cache->index, key);

	if (le != NULL) {
		centry = le->data;

		centry->data = data;

		handle_request_reset (cache, le);

	} else {
		centry = visual_mem_new0 (VisCacheEntry, 1);

		visual_timer_init (&centry->timer);
		visual_timer_start (&centry->timer);

		centry->key = strdup (key);
		centry->data = data;

		visual_list_add (cache->list, centry);

		le = cache->list->tail;

		visual_hashmap_put_string (cache->index, key, le);
	}

	return VISUAL_OK;
}

/**
 * Removes an entry from the cache.
 *
 * @param cache Pointer to the VisCache from which an entry is removed.
 * @param key The key that links to this entry.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL or -VISUAL_ERROR_NULL on failure.
 */
int visual_cache_remove (VisCache *cache, char *key)
{
	VisListEntry *le;

	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);
	visual_log_return_val_if_fail (key != NULL, -VISUAL_ERROR_NULL);

	le = visual_hashmap_get_string (cache->index, key);

	if (le != NULL)
		cache_remove_list_entry (cache, &le);

	return VISUAL_OK;
}

/**
 * Retrieves an entry from the cache.
 *
 * @param cache Pointer to the VisCache from which an entry is requested.
 * @param key The key that links to this entry.
 *
 * @return A pointer to the cache entry that links to the key or NULL on failure.
 */
void *visual_cache_get (VisCache *cache, char *key)
{
	VisCacheEntry *centry;
	VisListEntry *le;

	visual_log_return_val_if_fail (cache != NULL, NULL);
	visual_log_return_val_if_fail (key != NULL, NULL);

	le = visual_hashmap_get_string (cache->index, key);

	if (le == NULL)
		return NULL;

	centry = le->data;

	handle_request_reset (cache, le);

	return centry->data;
}

/**
 * Gives the number of entries currently in the cache.
 *
 * @param cache Pointer to the VisCache of which the number of elements that
 *	are currently in the cache is requested.
 *
 * @return The number of entries in the cache or -VISUAL_ERROR_CACHE_NULL on failure.
 */
int visual_cache_get_size (VisCache *cache)
{
	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);

	return visual_collection_size (VISUAL_COLLECTION (cache->list));
}

/**
 * Sets the boundaries in which the cache is operating. This can be bound to the maximum cache size
 * and also the maximum age of elements within the cache.
 *
 * @param cache Pointer to the VisCache on which boundaries are being set.
 * @param size The maximum size of the cache in N elements.
 * @param maxage The maximum age of cache elements within the cache. NULL is a valid argument.
 *
 * @return VISUAL_OK on succes, -VISUAL_ERROR_CACHE_NULL on failure.
 */
int visual_cache_set_limits (VisCache *cache, int size, VisTime *maxage)
{
	visual_log_return_val_if_fail (cache != NULL, -VISUAL_ERROR_CACHE_NULL);

	/* FIXME limit size change, rehash the index */

	cache->size = size;

	if (maxage != NULL) {
		cache->withmaxage = TRUE;
		visual_time_copy (&cache->maxage, maxage);
	} else {
		cache->withmaxage = FALSE;
	}

	return VISUAL_OK;
}

/**
 * Gives the complete list of VisCacheEntry elements in the cache. Never add new
 * entries to this list by hand as it won't update the indeces. The list obtained using
 * this function is absolutely read only.
 *
 * @param cache Pointer to the VisCache of which the complete list of VisCacheEntry elements is requested.
 *
 * @return The VisList containing the VisCacheEntry elements or NULL on failure.
 */
VisList *visual_cache_get_list (VisCache *cache)
{
	visual_log_return_val_if_fail (cache != NULL, NULL);

	return cache->list;
}

/**
 * @}
 */

