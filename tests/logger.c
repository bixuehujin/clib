/*
 * logger.c
 *
 *  Created on: Sep 28, 2012
 *      Author: hujin
 */

#include "clib/logger.h"

int main(int argc, char **argv) {

	logger_init("test-logger", &logger_syslog_handlers, &logger_stderr_handlers);

	logger_warn("a warn log message");
	logger_info("a info log message");
	logger_error("a error log message");
	logger_notice("a notice log message");

	logger_shutdown();
	return 0;
}



