/* When using the sdl driver and Xgl, sdl will show a nice empty window. Be sure to set the:
 * XLIB_SKIP_ARGB_VISUALS environment variable to 1
 */

#include <libvisual/libvisual.h>

#include <stdlib.h>

#include "display.h"
#include "sdldriver.h"
#include "x11driver.h"
#include "glxdriver.h"

int main (int argc, char **argv)
{
	SADisplay *display;
	VisVideo *video;

	VisInput *input;
	VisActor *actor;
	VisEventQueue *localqueue;
    VisParamContainer *params;
    VisParamEntry *param;
	VisVideoAttributeOptions *vidoptions;

	int running = TRUE;
	int fullscreen = FALSE;
	int visible = TRUE;

	int depth;

	visual_init (&argc, &argv);

    visual_log_set_verboseness(VISUAL_LOG_VERBOSENESS_HIGH);

	display = display_new (sdl_driver_new ());

	/* Libvisual stuff */
	if (argc > 1)
		actor = visual_actor_new (argv[1]);
	else
		actor = visual_actor_new ("blursk");


	if (argc > 3) {
		depth = visual_video_depth_enum_from_value (atoi (argv[3]));
	} else
		depth = visual_video_depth_get_highest (visual_actor_get_supported_depth (actor));

	vidoptions = visual_actor_get_video_attribute_options (actor);

	display_create (display, depth, vidoptions, 320, 200, TRUE);

	visual_actor_realize (actor);

	video = display_get_video (display);

        visual_actor_set_video (actor, video);
	visual_actor_video_negotiate (actor, 0, FALSE, FALSE);

	if (argc > 2)
		input = visual_input_new (argv[2]);
	else
		input = visual_input_new ("xmms2");

	visual_input_realize (input);

    params = visual_plugin_get_params(input->plugin);

    param = visual_param_container_get(params, "songinfo");

    if(param != NULL)
        visual_param_entry_set_object(param, 
            VISUAL_OBJECT(visual_actor_get_songinfo(actor)));

    //params = visual_plugin_get_params(actor->plugin);

    //string = visual_string_new_with_value("filename");
    //param = visual_param_container_get(params, string);
    //visual_param_entry_set_string(param, "/home/starlon/Projects/libvisual-svn/trunk/libvisual-avs/testpresets/ring10.avs");

	localqueue = visual_event_queue_new ();


	while (running) {
		VisEventQueue *pluginqueue;
		VisEvent *ev;

		/* Handle all events */
		display_drain_events (display, localqueue);

		pluginqueue = visual_plugin_get_eventqueue (visual_actor_get_plugin (actor));
		while (visual_event_queue_poll_by_reference (localqueue, &ev)) {

			if (ev->type != VISUAL_EVENT_RESIZE)
				visual_event_queue_add (pluginqueue, ev);

			switch (ev->type) {
                case VISUAL_EVENT_PARAM:
                    break;
				case VISUAL_EVENT_RESIZE:
					video = display_get_video (display);
					visual_actor_set_video (actor, video);

					visual_actor_video_negotiate (actor, depth, FALSE, FALSE);
					break;

				case VISUAL_EVENT_MOUSEMOTION:
					break;

				case VISUAL_EVENT_MOUSEBUTTONDOWN:

					break;

				case VISUAL_EVENT_MOUSEBUTTONUP:
					break;

				case VISUAL_EVENT_KEYDOWN:
					switch (ev->event.keyboard.keysym.sym) {
						case VKEY_ESCAPE:
							running = FALSE;
							break;

						case VKEY_TAB:
							fullscreen = !fullscreen;

							display_set_fullscreen (display, fullscreen, TRUE);

							/* Resync video */
							video = display_get_video (display);
							visual_actor_set_video (actor, video);

							visual_actor_video_negotiate (actor, depth, FALSE, FALSE);

							break;

						default:
							printf ("key: %c\n", ev->event.keyboard.keysym.sym);
							break;
					}

					break;

				case VISUAL_EVENT_KEYUP:
					break;

				case VISUAL_EVENT_QUIT:
					running = FALSE;
					break;

				case VISUAL_EVENT_VISIBILITY:
					visible = ev->event.visibility.is_visible;
					break;

				default:
					break;
			}
		}

		if (visible == FALSE) {
			visual_input_run (input);

			visual_time_usleep (10000);

			continue;
		}

		/* Do a run cycle */
		visual_input_run (input);

		display_lock (display);
		visual_actor_run (actor, input->audio);
		display_unlock (display);

		display_update_all (display);

		display_fps_limit (display, 30);
	}

	/* Termination procedure */
	display_set_fullscreen (display, FALSE, TRUE);
	display_close (display);

	visual_quit ();

	printf ("Total frames: %d, average fps: %f\n", display_fps_total (display), display_fps_average (display));

	return 0;
}
