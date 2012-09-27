/*
 * logger.h
 *
 *  Created on: Sep 27, 2012
 *      Author: hujin
 */

#ifndef LOGGER_H_
#define LOGGER_H_

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

typedef struct _logger_handlers{
	logger_init_func init;
	logger_write_func write;
	logger_shutdown_func shutdown;
}logger_handlers_t;

logger_t * logger_init(logger_t * logger, const char * ident, uint nhandlers, ...);
void logger_write(logger_t * logger, int level, const char * format, ...);
void logger_shutdown(logger_t * logger);

/* syslog handlers */
void logger_syslog_init(logger_t * logger);
void logger_syslog_write(logger_t * logger, int level, const char * format, va_list va);
void logger_syslog_shutdown(logger_t * logger);

/* stderr handlers */
void logger_stderr_init(logger_t * logger);
void logger_stderr_write(logger_t * logger, int level, const char * format, va_list va);
void logger_stderr_shutdown(logger_t * logger);

#endif /* LOGGER_H_ */
