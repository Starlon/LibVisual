#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include <signal.h>

#include "lv_common.h"
#include "lv_log.h"


static VisLogVerboseness verboseness = VISUAL_LOG_VERBOSENESS_MEDIUM;

static void default_info_handler (const char *msg, const char *funcname, void *privdata);
static void default_warning_handler (const char *msg, const char *funcname, void *privdata);
static void default_critical_handler (const char *msg, const char *funcname, void *privdata);
static void default_error_handler (const char *msg, const char *funcname, void *privdata);

static struct _message_handlers {
	visual_log_message_handler_func_t info_handler;
	visual_log_message_handler_func_t warning_handler;
	visual_log_message_handler_func_t critical_handler;
	visual_log_message_handler_func_t error_handler;
} message_handlers;


/**
 * @defgroup VisLog VisLog
 * @{
 */

/**
 * Set the library verbosity level.
 */
void visual_log_set_verboseness (VisLogVerboseness v)
{
	verboseness = v;
}

/**
 * Get the current library verbosity level.
 */
VisLogVerboseness visual_log_get_verboseness ()
{
	return verboseness;
}

/**
 * Set the function that handles info messages.
 */
void visual_log_set_info_handler (visual_log_message_handler_func_t handler)
{
	visual_log_return_if_fail (handler != NULL);

	message_handlers.info_handler = handler;
}

/**
 * Set the function that handles warning messages.
 */
void visual_log_set_warning_handler (visual_log_message_handler_func_t handler)
{
	visual_log_return_if_fail (handler != NULL);

	message_handlers.warning_handler = handler;
}

/**
 * Set the function that handles critical messages.
 */
void visual_log_set_critical_handler (visual_log_message_handler_func_t handler)
{
	visual_log_return_if_fail (handler != NULL);

	message_handlers.critical_handler = handler;
}

/**
 * Set the function that handles error messages. After handling the message with
 * this function, libvisual will abort the program. This behavior cannot be
 * changed.
 */
void visual_log_set_error_handler (visual_log_message_handler_func_t handler)
{
	visual_log_return_if_fail (handler != NULL);

	message_handlers.error_handler = handler;
}

/**
 * Set the function that handles all the messages.
 */
void visual_log_set_all_messages_handler (visual_log_message_handler_func_t handler)
{
	visual_log_return_if_fail (handler != NULL);

	message_handlers.info_handler = handler;
	message_handlers.warning_handler = handler;
	message_handlers.critical_handler = handler;
	message_handlers.error_handler = handler;
}
	
/**
 * Private library call used by the visual_log define to display debug,
 * warning and error messages.
 *
 * @see visual_log
 * 
 * @param severity Severity of the log message.
 * @param file Char pointer to a string that contains the source filename.
 * @param line Line number for which the log message is.
 * @param funcname Function name in which the log message is called.
 * @param fmt Format string to display the log message.
 *
 * @return 0 on succes -1 on error.
 */
void _lv_log (VisLogSeverity severity, const char *file,
			int line, const char *funcname, const char *fmt, ...)
{
	char str[1024];
	va_list va;
	
	assert (fmt != NULL);

	va_start (va, fmt);
	vsnprintf (str, 1023, fmt, va);
	va_end (va);

	switch (severity) {
		case VISUAL_LOG_DEBUG:
			if (verboseness == VISUAL_LOG_VERBOSENESS_HIGH)
				fprintf (stderr, "libvisual DEBUG: %s: %s() [(%s,%d)]: %s\n",
						__lv_progname, funcname, file, line, str);
		
			break;
		case VISUAL_LOG_INFO:
			if (!message_handlers.info_handler)
				visual_log_set_info_handler (default_info_handler);

			if (verboseness >= VISUAL_LOG_VERBOSENESS_MEDIUM)
				message_handlers.info_handler (str, funcname, NULL);

			break;
		case VISUAL_LOG_WARNING:
			if (!message_handlers.warning_handler)
				visual_log_set_warning_handler (default_warning_handler);

			if (verboseness >= VISUAL_LOG_VERBOSENESS_MEDIUM)
				message_handlers.warning_handler (str, funcname, NULL);
			
			break;

		case VISUAL_LOG_CRITICAL:
			if (!message_handlers.critical_handler)
				visual_log_set_critical_handler (default_critical_handler);

			if (verboseness >= VISUAL_LOG_VERBOSENESS_LOW)
				message_handlers.critical_handler (str, funcname, NULL);
		
			break;

		case VISUAL_LOG_ERROR:
			if (!message_handlers.error_handler)
				visual_log_set_error_handler (default_error_handler);

			if (verboseness >= VISUAL_LOG_VERBOSENESS_LOW)
				message_handlers.error_handler (str, funcname, NULL);
			
			raise (SIGTRAP);
			exit (1);

			break;
	}
}

/**
 * @}
 */

static void default_info_handler (const char *msg, const char *funcname, void *privdata)
{
	printf ("libvisual INFO: %s: %s\n", __lv_progname, msg);
}

static void default_warning_handler (const char *msg, const char *funcname, void *privdata)
{
	if (funcname)
		fprintf (stderr, "libvisual WARNING: %s: %s(): %s\n",
				__lv_progname, funcname, msg);
	else
		fprintf (stderr, "libvisual WARNING: %s: %s\n", __lv_progname, msg);
}

static void default_critical_handler (const char *msg, const char *funcname, void *privdata)
{
	if (funcname)
		fprintf (stderr, "libvisual CRITICAL: %s: %s(): %s\n",
				__lv_progname, funcname, msg);
	else
		fprintf (stderr, "libvisual CRITICAL: %s: %s\n", __lv_progname, msg);
}

static void default_error_handler (const char *msg, const char *funcname, void *privdata)
{
	if (funcname)
		fprintf (stderr, "libvisual ERROR: %s: %s(): %s\n",
				__lv_progname, funcname, msg);
	else
		fprintf (stderr, "libvisual ERROR: %s: %s\n", __lv_progname, msg);
}
