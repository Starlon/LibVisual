#ifndef _LV_LOG_H
#define _LV_LOG_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* This is read-only */
extern char *__lv_progname;

/**
 * Used to determine the severity of the log message using the visual_log
 * define.
 *
 * @see visual_log
 */
typedef enum {
	VISUAL_LOG_DEBUG,	/**< Debug message, to use for debug messages. */
	VISUAL_LOG_INFO,	/**< Informative message, can be used for general info. */
	VISUAL_LOG_WARNING,	/**< Warning message, use to warn the user. */
	VISUAL_LOG_CRITICAL,	/**< Critical message, when a critical situation happens.
				 * Like a NULL pointer passed to a method. */
	VISUAL_LOG_ERROR	/**< Error message, use to notify the user of fatals. 
				 * After message showing, the program is aborted. */
} VisLogSeverity;

/**
 * Used for log messages, this is brought under a define so
 * that the __FILE__ and __LINE__ macros (and probably __PRETTY_FUNC__) work,
 * and thus provide better information.
 *
 * @see VisLogSeverity
 *
 * @param severity Determines the severity of the message using VisLogSeverity.
 * @param format The format string of the log message.
 */
#ifdef __GNUC__

#ifdef LV_HAVE_ISO_VARARGS
#define visual_log(severity,...)		\
		_lv_log (severity,		\
			__FILE__,		\
			__LINE__,		\
			__PRETTY_FUNCTION__,	\
			__VA_ARGS__)
#elif defined(LV_HAVE_GNUC_VARARGS)
#define visual_log(severity,format...)		\
		_lv_log (severity,		\
			__FILE__,		\
			__LINE__,		\
			__PRETTY_FUNCTION__,	\
			format)
#else
static void visual_log (VisLogSeverity severity, const char *fmt, ...)
{
	char str[1024];
	va_list va;
	char sever_msg[10];
	
	assert (fmt != NULL);

	va_start (va, fmt);
	vsnprintf (str, 1023, fmt, va);
	va_end (va);

	switch (severity) {
		case VISUAL_LOG_DEBUG:
			strncpy (sever_msg, "DEBUG", 9);
			break;
		case VISUAL_LOG_INFO:
			strncpy (sever_msg, "INFO", 9);
			break;
		case VISUAL_LOG_WARNING:
			strncpy (sever_msg, "WARNING", 9);
			break;
		case VISUAL_LOG_CRITICAL:
			strncpy (sever_msg, "CRITICAL", 9);
			break;
		case VISUAL_LOG_ERROR:
			strncpy (sever_msg, "ERROR", 9);
			break;
		default:
			assert (0);
	}
	/*
	 * Sorry, we doesn't have (file,line) information
	 */
	if (severity == VISUAL_LOG_INFO) {
		printf ("libvisual %s: %s: %s\n",
				sever_msg, __lv_progname, str);
	} else {
		fprintf (stderr, "libvisual %s: %s: %s\n",
				sever_msg, __lv_progname, str);
	}

	if (severity == VISUAL_LOG_ERROR)
		exit(1);
}
#endif /* !(ISO_VARARGS || GNUC_VARARGS) */

#endif /* __GNUC__ */


#ifndef __GNUC__

#ifdef LV_HAVE_ISO_VARARGS
#define visual_log(severity,...)		\
		_lv_log (severity,		\
			__FILE__,		\
			__LINE__,		\
			(NULL),			\
			__VA_ARGS__)
#else
static void visual_log (VisLogSeverity severity, const char *fmt, ...)
{
	char str[1024];
	va_list va;
	char sever_msg[10];
	
	assert (fmt != NULL);

	va_start (va, fmt);
	vsnprintf (str, 1023, fmt, va);
	va_end (va);

	switch (severity) {
		case VISUAL_LOG_DEBUG:
			strncpy (sever_msg, "DEBUG", 9);
			break;
		case VISUAL_LOG_INFO:
			strncpy (sever_msg, "INFO", 9);
			break;
		case VISUAL_LOG_WARNING:
			strncpy (sever_msg, "WARNING", 9);
			break;
		case VISUAL_LOG_CRITICAL:
			strncpy (sever_msg, "CRITICAL", 9);
			break;
		case VISUAL_LOG_ERROR:
			strncpy (sever_msg, "ERROR", 9);
			break;
		default:
			assert (0);
	}
	/*
	 * Sorry, we don't have (file,line) information
	 */
	if (severity == VISUAL_LOG_INFO) {
		printf ("libvisual %s: %s: %s\n",
				sever_msg, __lv_progname, str);
	} else {
		fprintf (stderr, "libvisual %s: %s: %s\n",
				sever_msg, __lv_progname, str);
	}

	if (severity == VISUAL_LOG_ERROR)
		exit(1);
}
#endif /* ISO_VARARGS */

#endif /* !__GNUC__ */

/**
 * Return if @a expr is FALSE, showing a critical message with
 * useful information.
 */
#define visual_log_return_if_fail(expr)				\
	if (expr) { } else					\
	{							\
	visual_log (VISUAL_LOG_CRITICAL,			\
		 "assertion `%s' failed",			\
		#expr);						\
	return;							\
	}

/**
 * Return if @a val if @a expr is FALSE, showing a critical message
 * with useful information.
 */
#define visual_log_return_val_if_fail(expr, val)		\
	if (expr) { } else					\
	{							\
	visual_log (VISUAL_LOG_CRITICAL,			\
		 "assertion `%s' failed",			\
		#expr);						\
	return (val);						\
	}

#if defined __GNUC__
void _lv_log (VisLogSeverity severity, const char *file,
		int line, const char *funcname, const char *fmt, ...)
			__attribute__ ((__format__ (__printf__, 5, 6)));
#else
void _lv_log (VisLogSeverity severity, const char *file,
		int line, const char *funcname, const char *fmt, ...);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _LV_LOG_H */