#ifndef _LV_EVENT_H
#define _LV_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * Number of events allowed in the queue
 */
#define VISUAL_EVENT_MAXEVENTS	256

#include <libvisual/lv_songinfo.h>
#include <libvisual/lv_video.h>
#include <libvisual/lv_list.h>
#include <libvisual/lv_keysym.h>

/**
 * Used to define what kind of event is emitted by the VisEvent system.
 */
typedef enum {
	VISUAL_EVENT_NOEVENT,		/**< No event. */
	VISUAL_EVENT_KEYDOWN,		/**< Keyboard key pressed event. */
	VISUAL_EVENT_KEYUP,		/**< Keyboard key released event. */
	VISUAL_EVENT_MOUSEMOTION,	/**< Mouse movement event. */
	VISUAL_EVENT_MOUSEBUTTONDOWN,	/**< Mouse button pressed event. */
	VISUAL_EVENT_MOUSEBUTTONUP,	/**< Mouse button released event. */
	VISUAL_EVENT_NEWSONG,		/**< Song change event. */
	VISUAL_EVENT_RESIZE		/**< Window dimension change event. */
} VisEventType;

/**
 * Used to define key states within the VisEvent system.
 */
typedef enum {
	VISUAL_KEY_DOWN,		/**< Key is pressed. */
	VISUAL_KEY_UP			/**< Key is released. */
} VisKeyState;

/**
 * Used to define mouse button states within the VisEvent system.
 */
typedef enum {
	VISUAL_MOUSE_DOWN,		/**< Mouse button is pressed. */
	VISUAL_MOUSE_UP			/**< Mouse button is released. */
} VisMouseState;

typedef struct _VisEventKeyboard VisEventKeyboard;
typedef struct _VisEventMouseMotion VisEventMouseMotion;
typedef struct _VisEventMouseButton VisEventMouseButton;
typedef struct _VisEventResize VisEventResize;
typedef struct _VisEventNewSong VisEventNewSong;
typedef struct _VisEvent VisEvent;
typedef struct _VisEventQueue VisEventQueue;

/**
 * Keyboard event data structure.
 *
 * Contains information about keyboard events.
 *
 * @see visual_event_queue_add_keyboard
 */
struct _VisEventKeyboard {
	VisEventType	type;		/**< Event type of the event being emitted */
	VisKeySym	keysym;		/**< A keysym entry that contains which key had an
					  * event and information about modifier keys. */
};

/**
 * Mouse movement event data structure.
 *
 * Contains information about mouse movement events.
 *
 * @see visual_event_queue_add_mousemotion
 */
struct _VisEventMouseMotion {
	VisEventType	type;		/**< Event type of the event being emitted. */
	VisMouseState	state;		/**< Mouse button state. */
	int		x;		/**< The absolute mouse X value, where 0 is left of screen. */
	int		y;		/**< The absolute mouse Y value. where 0 is top of screen. */
	int		xrel;		/**< Relative X movement to the previous location. */
	int		yrel;		/**< Relative Y movement to the previous location. */
};

/**
 * Mouse button event data structure.
 *
 * Contains information about mouse button events.
 *
 * @see visual_event_queue_add_mousebutton
 */
struct _VisEventMouseButton {
	VisEventType	type;		/**< Event type of the event being emitted. */
	VisMouseState	state;		/**< Mouse button state. */
	int		button;		/**< The button the state relates to. */
	int		x;		/**< The absolute mouse X value. */
	int		y;		/**< The absolute mouse Y value. */
};

/**
 * Dimension change event data structure.
 *
 * Contains information about screen dimension and depth changes.
 *
 * @see visual_event_queue_add_resize
 */
struct _VisEventResize {
	VisEventType	 type;		/**< Event type of the event being emitted. */
	VisVideo	*video;		/**< Pointer to the VisVideo structure containing all the screen information. */
	int		 width;		/**< Width of the surface. */
	int		 height;	/**< Height of the surface. */
};

/**
 * Song change event data structure.
 *
 * Contains information about song changes.
 *
 * @see visual_event_queue_add_newsong
 */
struct _VisEventNewSong {
	VisEventType	 type;		/**< Event type of the event being emitted. */
	VisSongInfo	*songinfo;	/**< Pointer to the VisSongInfo structure containing all the information about
					 * the new song. */
};

/**
 * The main event data structure.
 * 
 * All events are encapsulated using the VisEvent structure.
 *
 * @see visual_event_new
 */
struct _VisEvent {
	VisEventType		type;		/**< Event type of the event being emitted. */
	VisEventKeyboard	keyboard;	/**< Keyboard event. */
	VisEventMouseMotion	mousemotion;	/**< Mouse movement event. */
	VisEventMouseButton	mousebutton;	/**< Mouse button event. */
	VisEventResize		resize;		/**< Dimension change event. */
	VisEventNewSong		newsong;	/**< Song change event. */
};

/**
 * The event queue data structure.
 *
 * Used to manage events queues and also provides quick access to
 * high piority data from events.
 *
 * @see visual_event_queue_new
 */
struct _VisEventQueue {
	VisList		 events;	/**< List of VisEvents in the queue. */
	VisEvent 	 lastresize;	/**< Last resize event to provide quick access
					  * to this high piority event. */
	int		 resizenew;	/**< Flag that is set when there is a new resize event. */
	int		 eventcount;	/**< Contains the number of events in queue. */

	int		 mousex;	/**< Current absolute mouse X value. */
	int		 mousey;	/**< Current absolute mouse Y value. */
	VisMouseState	 mousestate;	/**< Current mouse button state. */
};

VisEvent *visual_event_new ();
int visual_event_free (VisEvent *event);
VisEventQueue *visual_event_queue_new ();
int visual_event_queue_free (VisEventQueue *eventqueue);
int visual_event_queue_poll (VisEventQueue *eventqueue, VisEvent *event);
int visual_event_queue_add (VisEventQueue *eventqueue, VisEvent *event);
int visual_event_queue_add_keyboard (VisEventQueue *eventqueue, VisKey keysym, int keymod, VisKeyState state);
int visual_event_queue_add_mousemotion (VisEventQueue *eventqueue, int x, int y);
int visual_event_queue_add_mousebutton (VisEventQueue *eventqueue, int button, VisMouseState state);
int visual_event_queue_add_resize (VisEventQueue *eventqueue, VisVideo *video, int width, int height);
int visual_event_queue_add_newsong (VisEventQueue *eventqueue, VisSongInfo *songinfo);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LV_EVENT_H */
