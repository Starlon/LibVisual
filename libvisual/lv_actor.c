#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "lvconfig.h"
#include "lv_log.h"
#include "lv_list.h"
#include "lv_actor.h"
#include "lv_mem.h"

extern VisList *__lv_plugins_actor;

static VisActorPlugin *get_actor_plugin (VisActor *actor);

static VisActorPlugin *get_actor_plugin (VisActor *actor)
{
	VisActorPlugin *actplugin;

	visual_log_return_val_if_fail (actor != NULL, NULL);
	visual_log_return_val_if_fail (actor->plugin != NULL, NULL);

	actplugin = actor->plugin->info->plugin;

	return actplugin;
}

/**
 * @defgroup VisActor VisActor
 * @{
 */

/**
 * Gives the encapsulated VisPluginData from a VisActor.
 *
 * @param actor Pointer of a VisActor of which the VisPluginData needs to be returned.
 *
 * @return VisPluginData that is encapsulated in the VisActor, possibly NULL.
 */
VisPluginData *visual_actor_get_plugin (const VisActor *actor)
{
	return actor->plugin;
}

/**
 * Gives a list of VisActors in the current plugin registry.
 *
 * @return An VisList containing the VisActors in the plugin registry.
 */
VisList *visual_actor_get_list ()
{
	return __lv_plugins_actor;
}

/** @todo find a way to NOT load, unload the plugins */

/**
 * Gives the next actor plugin based on the name of a plugin but skips non
 * GL plugins.
 *
 * @see visual_actor_get_prev_by_name_gl
 *
 * @param name The name of the current plugin or NULL to get the first.
 *
 * @return The name of the next plugin within the list that is a GL plugin.
 */
const char *visual_actor_get_next_by_name_gl (const char *name)
{
	const char *next = name;
	VisPluginData *plugin;
	VisPluginRef *ref;
	VisActorPlugin *actplugin;
	int gl;

	do {
		next = visual_plugin_get_next_by_name (visual_actor_get_list (), next);

		if (next == NULL)
			return NULL;
		
		ref = visual_plugin_find (__lv_plugins_actor, next);
		plugin = visual_plugin_load (ref);

		actplugin = plugin->info->plugin;

		if ((actplugin->depth & VISUAL_VIDEO_DEPTH_GL) > 0)
			gl = TRUE;
		else
			gl = FALSE;
	
		visual_plugin_unload (plugin);

	} while (gl == FALSE);

	return next;
}

/**
 * Gives the previous actor plugin based on the name of a plugin but skips non
 * GL plugins.
 *
 * @see visual_actor_get_next_by_name_gl
 *
 * @param name The name of the current plugin or NULL to get the last.
 *
 * @return The name of the previous plugin within the list that is a GL plugin.
 */
const char *visual_actor_get_prev_by_name_gl (const char *name)
{
	const char *prev = name;
	VisPluginData *plugin;
	VisPluginRef *ref;
	VisActorPlugin *actplugin;
	int gl;

	do {
		prev = visual_plugin_get_prev_by_name (visual_actor_get_list (), prev);

		if (prev == NULL)
			return NULL;
		
		ref = visual_plugin_find (__lv_plugins_actor, prev);
		plugin = visual_plugin_load (ref);
		actplugin = plugin->info->plugin;

		if ((actplugin->depth & VISUAL_VIDEO_DEPTH_GL) > 0)
			gl = TRUE;
		else
			gl = FALSE;
	
		visual_plugin_unload (plugin);

	} while (gl == FALSE);

	return prev;
}

/**
 * Gives the next actor plugin based on the name of a plugin but skips
 * GL plugins.
 *
 * @see visual_actor_get_prev_by_name_nogl
 *
 * @param name The name of the current plugin or NULL to get the first.
 *
 * @return The name of the next plugin within the list that is not a GL plugin.
 */
const char *visual_actor_get_next_by_name_nogl (const char *name)
{
	const char *next = name;
	VisPluginData *plugin;
	VisPluginRef *ref;
	VisActorPlugin *actplugin;
	int gl;

	do {
		next = visual_plugin_get_next_by_name (visual_actor_get_list (), next);

		if (next == NULL)
			return NULL;
		
		ref = visual_plugin_find (__lv_plugins_actor, next);
		plugin = visual_plugin_load (ref);
		actplugin = plugin->info->plugin;

		if ((actplugin->depth & VISUAL_VIDEO_DEPTH_GL) > 0)
			gl = TRUE;
		else
			gl = FALSE;
	
		visual_plugin_unload (plugin);

	} while (gl == TRUE);

	return next;
}

/**
 * Gives the previous actor plugin based on the name of a plugin but skips
 * GL plugins.
 *
 * @see visual_actor_get_next_by_name_nogl
 *
 * @param name The name of the current plugin or NULL to get the last.
 *
 * @return The name of the previous plugin within the list that is not a GL plugin.
 */
const char *visual_actor_get_prev_by_name_nogl (const char *name)
{
	const char *prev = name;
	VisPluginData *plugin;
	VisPluginRef *ref;
	VisActorPlugin *actplugin;
	int gl;

	do {
		prev = visual_plugin_get_prev_by_name (visual_actor_get_list (), prev);

		if (prev == NULL)
			return NULL;
		
		ref = visual_plugin_find (__lv_plugins_actor, prev);
		plugin = visual_plugin_load (ref);
		actplugin = plugin->info->plugin;

		if ((actplugin->depth & VISUAL_VIDEO_DEPTH_GL) > 0)
			gl = TRUE;
		else
			gl = FALSE;
	
		visual_plugin_unload (plugin);

	} while (gl == TRUE);

	return prev;
}

/**
 * Gives the next actor plugin based on the name of a plugin.
 *
 * @see visual_actor_get_prev_by_name
 * 
 * @param name The name of the current plugin, or NULL to get the first.
 *
 * @return The name of the next plugin within the list.
 */
const char *visual_actor_get_next_by_name (const char *name)
{
	return visual_plugin_get_next_by_name (visual_actor_get_list (), name);
}

/**
 * Gives the previous actor plugin based on the name of a plugin.
 *
 * @see visual_actor_get_next_by_name
 * 
 * @param name The name of the current plugin. or NULL to get the last.
 *
 * @return The name of the previous plugin within the list.
 */
const char *visual_actor_get_prev_by_name (const char *name)
{
	return visual_plugin_get_prev_by_name (visual_actor_get_list (), name);
}

/**
 * Checks if the actor plugin is in the registry, based on it's name.
 *
 * @param name The name of the plugin that needs to be checked.
 *
 * @return TRUE if found, else FALSE.
 */
int visual_actor_valid_by_name (const char *name)
{
	if (visual_plugin_find (visual_actor_get_list (), name) == NULL)
		return FALSE;
	else
		return TRUE;
}

/**
 * Creates a new actor from name, the plugin will be loaded but won't be realized.
 *
 * @param actorname
 * 	The name of the plugin to load, or NULL to simply allocate a new
 * 	actor. 
 *
 * @return A newly allocated VisActor, optionally containing a loaded plugin. Or NULL on error.
 */
VisActor *visual_actor_new (const char *actorname)
{
	VisActor *actor;
	VisPluginRef *ref;

	if (__lv_plugins_actor == NULL && actorname != NULL) {
		visual_log (VISUAL_LOG_CRITICAL, "the plugin list is NULL");
		return NULL;
	}
	
	actor = visual_mem_new0 (VisActor, 1);

	if (actorname == NULL)
		return actor;

	ref = visual_plugin_find (__lv_plugins_actor, actorname);

	actor->plugin = visual_plugin_load (ref);

	return actor;
}

/**
 * Realize the VisActor. This also calls the plugin init function.
 *
 * @param actor Pointer to a VisActor that needs to be realized.
 *
 * @return 0 on succes -1 on error.
 */
int visual_actor_realize (VisActor *actor)
{
	visual_log_return_val_if_fail (actor != NULL, -1);
	visual_log_return_val_if_fail (actor->plugin != NULL, -1);

	visual_plugin_realize (actor->plugin);

	return 0;
}

/**
 * Destroy the VisActor. This unloads the plugin when it's loaded, and also frees the actor itself including
 * all it's members.
 *
 * @param actor Pointer to a VisActor that needs to be destroyed.
 *
 * @return 0 on succes -1 on error.
 */ 
int visual_actor_destroy (VisActor *actor)
{
	visual_log_return_val_if_fail (actor != NULL, -1);

	if (actor->plugin != NULL)
		visual_plugin_unload (actor->plugin);

	return visual_actor_free (actor);
}

/**
 * Free the VisActor. This frees the VisActor data structure, but does not destroy the plugin within. If this is desired
 * use visual_actor_destroy.
 *
 * @see visual_actor_destroy
 * 
 * @param actor Pointer to a VisActor that needs to be freed.
 *
 * @return 0 on succes -1 on error.
 */
int visual_actor_free (VisActor *actor)
{
	visual_log_return_val_if_fail (actor != NULL, -1);

	if (actor->transform != NULL)
		visual_video_free_with_buffer (actor->transform);

	if (actor->fitting != NULL)
		visual_video_free_with_buffer (actor->fitting);
		
	if (actor->songinfo != NULL)
		visual_songinfo_free (actor->songinfo);

	if (actor->songcompare != NULL)
		visual_songinfo_free (actor->songcompare);

	free (actor);
	
	return 0;
}

/**
 * Gives a pointer to the song info data within the VisActor. This song info data can be used
 * to set name, artist and even coverart which can be used by the plugins and the framework itself.
 *
 * @see VisSongInfo
 *
 * @param actor Pointer to a VisActor of which the song info is needed.
 *
 * @return Pointer to the song info structure on succes or NULL on error.
 */
VisSongInfo *visual_actor_get_songinfo (VisActor *actor)
{
	visual_log_return_val_if_fail (actor != NULL, NULL);

	if (actor->songinfo == NULL)
		actor->songinfo = visual_songinfo_new (VISUAL_SONGINFO_TYPE_NULL);
	
	return actor->songinfo;
}

/**
 * Gives a pointer to the palette within the VisActor. This can be needed to set a palette on the target
 * display when it's in index mode.
 *
 * @see VisPalette
 *
 * @param actor Pointer to a VisActor of which the palette is needed.
 *
 * @return Pointer to the palette structure on succes or NULL on error. Also it's possible that NULL
 * is returned when the plugin is running in a full color mode or openGL. The returned palette is
 * read only.
 */
VisPalette *visual_actor_get_palette (VisActor *actor)
{
	VisActorPlugin *actplugin;

	visual_log_return_val_if_fail (actor != NULL, NULL);

	actplugin = get_actor_plugin (actor);
	
	if (actplugin == NULL) {
		visual_log (VISUAL_LOG_CRITICAL,
			"The given actor does not reference any actor plugin");
		return NULL;
	}

	if (actor->transform != NULL &&
		actor->video->depth == VISUAL_VIDEO_DEPTH_8BIT) {
		
		return actor->ditherpal;

	} else {
		return actplugin->palette (visual_actor_get_plugin (actor));
	}

	return NULL;
}

/**
 * This function negotiates the VisActor with it's target video that is set by visual_actor_set_video.
 * When needed it also sets up size fitting environment and depth transformation environment.
 *
 * The function has a few extra arguments that are mainly to be used from within internal code.
 *
 * This function needs to be called everytime there is a change within either the size or depth of
 * the target video. 
 *
 * The main method of calling this function is: "visual_actor_video_negotiate (actor, 0, FALSE, FALSE)"
 * 
 * @see visual_actor_set_video
 *
 * @param actor Pointer to a VisActor that needs negotiation.
 * @param rundepth An depth in the form of the VISUAL_VIDEO_DEPTH_* style when a depth is forced.
 * 	  This could be needed when for example a plugin has both a 8 bits and a 32 bits display method
 * 	  but while the target video is in 32 bits you still want to run the plugin in 8 bits. If this
 * 	  is desired the "forced" argument also needs to be set on TRUE.
 * @param noevent When set on TRUE this does only renegotiate depth transformation environments. For example
 * 	  when the target display was running in 32 bits and switched to 8 bits while the plugin was already
 * 	  in 8 bits it doesn't need an events, which possibly reinitializes the plugin.
 * @param forced This should be set if the rundepth argument is set, so it forces the plugin in a certain
 * 	  depth.
 *
 * @return 0 on succes -1 on error. 
 */ 
int visual_actor_video_negotiate (VisActor *actor, int rundepth, int noevent, int forced)
{
	VisActorPlugin *actplugin = get_actor_plugin (actor);
	int tmpwidth, tmpheight, tmppitch;
	int depthflag;

	/*
	 * Uhau, we really check the structure sanity.
	 */
	visual_log_return_val_if_fail (actor != NULL, -1);
	visual_log_return_val_if_fail (actor->plugin != NULL, -1);
	visual_log_return_val_if_fail (actor->plugin->ref != NULL, -1);
	visual_log_return_val_if_fail (actor->video != NULL, -1);

	if (actor->transform != NULL) {
		visual_video_free_with_buffer (actor->transform);
		actor->transform = NULL;
	}
	
	if (actor->fitting != NULL) {
		visual_video_free_with_buffer (actor->fitting);
		actor->fitting = NULL;
	}

	if (actor->ditherpal != NULL) {
		visual_palette_free (actor->ditherpal);
		actor->ditherpal = NULL;
	}

	depthflag = visual_actor_get_supported_depth (actor);

	visual_log (VISUAL_LOG_INFO, "negotiating plugin %s", actor->plugin->info->name);
	/* Set up depth transformation enviroment */
	if (visual_video_depth_is_supported (depthflag, actor->video->depth) != TRUE ||
			(forced == TRUE && actor->video->depth != rundepth)) {

		/* Depth transform enviroment, it automaticly
		 * fits size because it can use the pitch from
		 * the dest video context */
		actor->transform = visual_video_new ();

		visual_log (VISUAL_LOG_INFO, "run depth %d forced %d\n", rundepth, forced);
		
		if (forced == TRUE)
			visual_video_set_depth (actor->transform, rundepth);
		else
			visual_video_set_depth (actor->transform,
					visual_video_depth_get_highest_nogl (depthflag));

		visual_log (VISUAL_LOG_INFO, "transpitch1 %d depth %d bpp %d", actor->transform->pitch, actor->transform->depth,
				actor->transform->bpp);
		/* If there is only GL (which gets returned by highest nogl if
		 * nothing else is there, stop here */
		if (actor->transform->depth == VISUAL_VIDEO_DEPTH_GL)
			return -1;

		visual_video_set_dimension (actor->transform, actor->video->width, actor->video->height);
		visual_log (VISUAL_LOG_INFO, "transpitch2 %d %d", actor->transform->width, actor->transform->pitch);
		
		actplugin->requisition (visual_actor_get_plugin (actor), &actor->transform->width, &actor->transform->height);
		visual_log (VISUAL_LOG_INFO, "transpitch3 %d", actor->transform->pitch);
		
		if (noevent == FALSE) {
			visual_event_queue_add_resize (&actor->plugin->eventqueue, actor->transform,
					actor->transform->width, actor->transform->height);
			visual_plugin_events_pump (actor->plugin);
		} else {
			/* Normally a visual_video_set_dimension get's called within the
			 * event handler, but we won't come there right now so we've
			 * got to set the pitch ourself */
			visual_video_set_dimension (actor->transform,
					actor->transform->width, actor->transform->height);
		}
		
		visual_log (VISUAL_LOG_INFO, "rundepth: %d transpitch %d\n", rundepth, actor->transform->pitch);
		visual_video_allocate_buffer (actor->transform);

		if (actor->video->depth == VISUAL_VIDEO_DEPTH_8BIT)
			actor->ditherpal = visual_palette_new (256);

	} else {
		tmpwidth = actor->video->width;
		tmpheight = actor->video->height;
		tmppitch = actor->video->pitch;

		/* Pump the resize events and handle all the pending events */
		actplugin->requisition (visual_actor_get_plugin (actor), &actor->video->width, &actor->video->height);
		
		if (noevent == FALSE) {
			visual_event_queue_add_resize (&actor->plugin->eventqueue, actor->video,
					actor->video->width, actor->video->height);
			visual_plugin_events_pump (actor->plugin);
		}

		/* Size fitting enviroment */
		if (tmpwidth != actor->video->width || tmpheight != actor->video->height) {
			actor->fitting = visual_video_new_with_buffer (actor->video->width,
					actor->video->height, actor->video->depth);

			visual_video_set_dimension (actor->video, tmpwidth, tmpheight);
		}

		/* Set the pitch seen this is the framebuffer context */
		visual_video_set_pitch (actor->video, tmppitch);
	}

	return 0;
}

/**
 * Gives the by the plugin natively supported depths
 *
 * @param actor Pointer to a VisActor of which the supported depth of it's
 * 	  encapsulated plugin is requested.
 *
 * @return an OR value of the VISUAL_VIDEO_DEPTH_* values which can be checked against using AND on succes, -1 on error
 */
int visual_actor_get_supported_depth (const VisActor *actor)
{
	VisPluginData *plugin;
	VisActorPlugin *actplugin;

	visual_log_return_val_if_fail (actor != NULL, -1);

	plugin = visual_actor_get_plugin (actor);

	if (plugin == NULL) {
		visual_log (VISUAL_LOG_CRITICAL,
			"The given actor does not reference any actor plugin");
		return -1;
	}

	actplugin = plugin->info->plugin;

	return actplugin->depth;
}

/**
 * Used to connect the target display it's VisVideo structure to the VisActor.
 *
 * Using the visual_video methods the screenbuffer, it's depth and dimension and optionally it's pitch
 * can be set so the actor plugins know about their graphical environment and have a place to draw.
 *
 * After this function it's most likely that visual_actor_video_negotiate needs to be called.
 *
 * @see visual_video_new
 * @see visual_actor_video_negotiate
 * 
 * @param actor Pointer to a VisActor to which the VisVideo needs to be set.
 * @param video Pointer to a VisVideo which contains information about the target display and the pointer
 * 	  to it's screenbuffer.
 *
 * @return 0 on succes -1 on error.
 */
int visual_actor_set_video (VisActor *actor, VisVideo *video)
{
	visual_log_return_val_if_fail (actor != NULL, -1);

	actor->video = video;

	return 0;
}

/**
 * This is called to run an VisActor. It also pump it's events when needed, checks for new song events and also does the fitting 
 * and depth transformation actions when needed.
 *
 * Every run cycle one frame is created, so this function needs to be used in the main draw loop of the application.
 *
 * @param actor Pointer to a VisActor that needs to be runned.
 * @param audio Pointer to a VisAudio that contains all the audio data.
 */
int visual_actor_run (VisActor *actor, VisAudio *audio)
{
	VisActorPlugin *actplugin;
	VisPluginData *plugin;
	VisVideo *video;
	VisVideo *transform;
	VisVideo *fitting;

	/* We don't check for video, because we don't always need a video */
	/*
	 * Really? take a look at visual_video_set_palette bellow
	 */
	visual_log_return_val_if_fail (actor != NULL, -1);
	visual_log_return_val_if_fail (actor->video != NULL, -1);
	visual_log_return_val_if_fail (audio != NULL, -1);

	actplugin = get_actor_plugin (actor);
	plugin = visual_actor_get_plugin (actor);

	if (actplugin == NULL) {
		visual_log (VISUAL_LOG_CRITICAL,
			"The given actor does not reference any actor plugin");
		return -1;
	}

	/* Songinfo handling */
	if (actor->songinfo != NULL) {
		if (actor->songcompare == NULL) {
			visual_songinfo_mark (actor->songinfo);
		
			if (plugin->info->events != NULL)
				visual_event_queue_add_newsong (&plugin->eventqueue, actor->songinfo);

			actor->songcompare = visual_songinfo_new (VISUAL_SONGINFO_TYPE_NULL);

			visual_songinfo_free_strings (actor->songcompare);
			visual_songinfo_copy (actor->songcompare, actor->songinfo);
		}

		if (visual_songinfo_compare (actor->songinfo, actor->songcompare) != 0) {
			visual_songinfo_mark (actor->songinfo);

			if (plugin->info->events != NULL)
				visual_event_queue_add_newsong (&plugin->eventqueue, actor->songinfo);

			visual_songinfo_free_strings (actor->songcompare);
			visual_songinfo_copy (actor->songcompare, actor->songinfo);
		}
	}
	
	plugin->songinfo = actor->songinfo;

	video = actor->video;
	transform = actor->transform;
	fitting = actor->fitting;

	/*
	 * This needs to happen before palette, render stuff, always, period.
	 * Also internal vars can be initialized when params have been set in init on the param
	 * events in the event loop.
	 */
	visual_plugin_events_pump (actor->plugin);
	
	visual_video_set_palette (video, visual_actor_get_palette (actor));
	
	/* Set the palette to the target video */
	video->pal = visual_actor_get_palette (actor);

	
	/* Yeah some transformation magic is going on here when needed */
	if (transform != NULL && (transform->depth != video->depth)) {
		actplugin->render (plugin, transform, audio);

		if (transform->depth == VISUAL_VIDEO_DEPTH_8BIT) {
			visual_video_set_palette (transform, visual_actor_get_palette (actor));
			visual_video_depth_transform (video, transform);
		} else {
			visual_video_set_palette (transform, actor->ditherpal);
			visual_video_depth_transform (video, transform);
		}
	} else {
		if (fitting != NULL && (fitting->width != video->width || fitting->height != video->height)) {
			actplugin->render (plugin, fitting, audio);
			visual_video_blit_overlay (video, fitting, 0, 0, FALSE);
		} else {
			actplugin->render (plugin, video, audio);
		}
	}

	return 0;
}

/**
 * @}
 */
