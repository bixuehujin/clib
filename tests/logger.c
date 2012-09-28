/*
 * logger.c
 *
 *  Created on: Sep 28, 2012
 *      Author: hujin
 */

#include "clib/logger.h"

int main(int argc, char **argv) {

	logger_t logger;
	logger_init(&logger, "test-logger", &logger_syslog_handlers, &logger_stderr_handlers);

	logger_warn(&logger, "a warn log message");
	logger_info(&logger, "a info log message");
	logger_error(&logger, "a error log message");
	logger_notice(&logger, "a notice log message");

	logger_shutdown(&logger);
	return 0;
}



