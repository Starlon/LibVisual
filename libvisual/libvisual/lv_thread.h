#ifndef _LV_THREAD_H
#define _LV_THREAD_H

#include <libvisual/lvconfig.h>
#include <libvisual/lv_common.h>

#ifdef VISUAL_HAVE_THREADS
#ifdef VISUAL_THREAD_MODEL_POSIX
#include <pthread.h>
#else /* !VISUAL_THREAD_MODEL_POSIX */

#endif
#endif /* VISUAL_HAVE_THREADS */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _VisThread VisThread;
typedef struct _VisMutex VisMutex;

/**
 * The function defination for a function that forms the base of a new VisThread when
 * visual_thread_create is used.
 *
 * @see visual_thread_create
 *
 * @arg data Pointer that can contains the private data from the visual_thread_create function.
 *
 * @return Pointer to the data when a thread is joined.
 */
typedef void *(*VisThreadFunc)(void *data);

/**
 * The VisThread data structure and the VisThread subsystem is a wrapper system for native
 * threading implementations.
 */
struct _VisThread {
#ifdef VISUAL_HAVE_THREADS
#ifdef VISUAL_THREAD_MODEL_POSIX
	pthread_t thread;		/**< Private used for the pthread implementation. */
#else /* !VISUAL_THREAD_MODEL_POSIX */

#endif
#endif /* VISUAL_HAVE_THREADS */
};

/**
 * The VisMutex data structure and the VisMutex subsystem is a wrapper system for native
 * thread locking implementations.
 */
struct _VisMutex {
#ifdef VISUAL_HAVE_THREADS
#ifdef VISUAL_THREAD_MODEL_POSIX
	pthread_mutex_t mutex;		/**< Private used for the pthreads implementation. */
#else /* !VISUAL_THREAD_MODEL_POSIX */

#endif
#endif /* VISUAL_HAVE_THREADS */
};

int visual_thread_is_supported (void);
VisThread *visual_thread_create (VisThreadFunc func, void *data, int joinable);
int visual_thread_free (VisThread *thread);
void *visual_thread_join (VisThread *thread);
void visual_thread_exit (void *retval);
void visual_thread_yield (void);
VisThread *visual_thread_self (void);

VisMutex *visual_mutex_new (void);
int visual_mutex_lock (VisMutex *mutex);
int visual_mutex_trylock (VisMutex *mutex);
int visual_mutex_unlock (VisMutex *mutex);
int visual_mutex_free (VisMutex *mutex);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LV_THREAD_H */