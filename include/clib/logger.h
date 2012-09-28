/*
 * logger.h
 *
 *  Created on: Sep 27, 2012
 *      Author: hujin
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>

#define LOGGER_MAX_HANDLERS		10


#define	LOGGER_EMERG 	LOG_EMERG		/* system is unusable */
#define	LOGGER_ALERT 	LOG_ALERT		/* action must be taken immediately */
#define	LOGGER_CRIT  	LOG_CRIT		/* critical conditions */
#define	LOGGER_ERR   	LOG_ERR			/* error conditions */
#define	LOGGER_WARNING 	LOG_WARNING		/* warning conditions */
#define	LOGGER_NOTICE 	LOG_NOTICE		/* normal but significant condition */
#define	LOGGER_INFO 	LOG_INFO		/* informational */
#define	LOGGER_DEBUG 	LOG_DEBUG		/* debug-level messages */

static char * logger_names[] = {
		"emerg",
		"alert",
		"crit",
		"error",
		"warn",
		"notice",
		"info",
		"debug",
		NULL
};

typedef struct _logger logger_t;

/* logger back-end interfaces */
typedef void (*logger_init_func)(logger_t * logger);
typedef void (*logger_write_func)(logger_t * logger, int level, const char * format, va_list va);
typedef void (*logger_shutdown_func)(logger_t * logger);

/* data structures */
typedef struct _logger_handlers{
	logger_init_func init;
	logger_write_func write;
	logger_shutdown_func shutdown;
}logger_handlers_t;

struct _logger{
	logger_handlers_t * handlers[LOGGER_MAX_HANDLERS];
	uint nhandlers;
	char ident[16];
};

/* logger interfaces */
logger_t * _logger_init(logger_t * logger, const char * ident, ...);
void logger_write(logger_t * logger, int level, const char * format, ...);
void logger_shutdown(logger_t * logger);
#define logger_init(logger, ident, ...)      _logger_init(logger, ident, ##__VA_ARGS__, NULL)

#define logger_warn(logger, format, ...)     logger_write(logger, LOGGER_WARNING, format, ##__VA_ARGS__)
#define logger_info(logger, format, ...)     logger_write(logger, LOGGER_INFO, format, ##__VA_ARGS__)
#define logger_debug(logger, format, ...)    logger_write(logger, LOGGER_DEBUG, format, ##__VA_ARGS__)
#define logger_error(logger, format, ...)    logger_write(logger, LOGGER_ERR, format, ##__VA_ARGS__)
#define logger_notice(logger, format, ...)   logger_write(logger, LOGGER_NOTICE, format, ##__VA_ARGS__)
#define logger_alert(logger, format, ...)    logger_write(logger, LOGGER_ALERT, format, ##__VA_ARGS__)
#define logger_crit(logger, format, ...)     logger_write(logger, LOGGER_CRIT, format, ##__VA_ARGS__)
#define logger_emerg(logger, format, ...)    logger_write(logger, LOGGER_EMERG, format, ##__VA_ARGS__)


/* syslog handlers */
void logger_syslog_init(logger_t * logger);
void logger_syslog_write(logger_t * logger, int level, const char * format, va_list va);
void logger_syslog_shutdown(logger_t * logger);

/* stderr handlers */
void logger_stderr_init(logger_t * logger);
void logger_stderr_write(logger_t * logger, int level, const char * format, va_list va);
void logger_stderr_shutdown(logger_t * logger);


static logger_handlers_t logger_syslog_handlers = {
	.init = logger_syslog_init,
	.write = logger_syslog_write,
	.shutdown = logger_syslog_shutdown
};

static logger_handlers_t logger_stderr_handlers = {
	.init = logger_stderr_init,
	.write = logger_stderr_write,
	.shutdown = logger_stderr_shutdown
};

#endif /* LOGGER_H_ */
