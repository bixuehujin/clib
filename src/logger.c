/*
 * logger.c
 *
 *  Created on: Sep 27, 2012
 *      Author: hujin
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#include <syslog.h>
#include "clib/sstring.h"
#include "clib/logger.h"


struct _logger{
	logger_handlers_t handlers[LOGGER_MAX_HANDLERS];
	uint nhandlers;
	char ident[16];
};


#define call_handler(hander_name, ...) 											\
	do{																			\
		int i = 0;																\
		for(i = 0; i < logger->nhandlers; i ++) { 								\
			if(logger->handlers[i].hander_name) {								\
				logger->handlers[i].hander_name(__VA_ARGS__);					\
			}																	\
		}																		\
	}while(0)



/**
 * initialize a logger object with n handers.
 */
logger_t * logger_init(logger_t * logger,const char * ident ,uint nhandlers, ...) {
	assert(logger != NULL);
	logger->nhandlers = nhandlers;
	strncpy(logger->ident, ident, sizeof(logger->ident) - 1);
	logger->ident[15] = '\0';

	va_list va;
	logger_handlers_t handlers;
	int i;

	va_start(va, nhandlers);
	for(i = 0; i < nhandlers; i ++) {
		handlers = va_arg(va, logger_handlers_t);
		logger->handlers[i] = handlers;
	}
	va_end(va);

	call_handler(init, logger);

	return logger;
}


/**
 * write a logger line to registered back-ends.
 */
void logger_write(logger_t * logger,int level ,const char * format, ...) {
	va_list va;
	va_start(va, format);
	call_handler(write, logger, level, format, va);
	va_end(va);
}


void logger_shutdown(logger_t * logger) {
	assert(logger != NULL);
	call_handler(shutdown, logger);
}


void logger_syslog_init(logger_t * logger) {
	openlog(logger->ident, LOG_CONS, LOG_USER);
}


void logger_syslog_write(logger_t * logger, int level, const char * format, va_list va) {
	sstring_t ss = sstring_for_init;
	sstring_vappend(&ss, format, va);
	syslog(level, "<%s> %s", logger_names[level], ss.ptr);
	sstring_destroy(&ss);
}


void logger_syslog_shutdown(logger_t * logger) {
	closelog();
}


void logger_stderr_init(logger_t * logger) {

}


void logger_stderr_write(logger_t * logger, int level, const char * format, va_list va) {
	time_t t = time(NULL);
	char * str = ctime(&t);
	str[strlen(str) - 1] = '\0';
	sstring_t ss = sstring_for_init;
	sstring_vappend(&ss, format, va);
	fprintf(stderr, "%s %s: <%s> %s\n", str, logger->ident, logger_names[level], ss.ptr);
	sstring_destroy(&ss);
}


void logger_stderr_shutdown(logger_t * logger) {

}
