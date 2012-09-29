/*
 * mainloop.c
 *
 *  Created on: Sep 28, 2012
 *      Author: hujin
 */



#include <stdio.h>
#include <glib.h>
#include <glib/gthread.h>

typedef struct _UdevSource {
	GSource source;

}UdevSource;

gboolean prepare_func(GSource * source, gint * timeout) {
	printf("prepare\n");

	*timeout = 1000;
	return FALSE;
}

gboolean check_func(GSource * source) {
	printf("check\n");
	return TRUE;
}

void finalize_func(GSource * source) {
	printf("finalize\n");
}

gboolean source_func(gpointer user_data) {

	return TRUE;
}

gboolean dispatch_func(GSource * source, GSourceFunc callback, gpointer user_data) {
	printf("dispatch:callback %p\n", callback);
	GMainLoop * loop = user_data;
	static int i = 2;

	if(i-- == 1) {
		g_main_loop_quit(loop);
	}
	return TRUE;
}

int main(int argc, char **argv) {
	GMainLoop * loop = g_main_loop_new(NULL, 0);
	GMainContext * context = g_main_loop_get_context(loop);

	GSourceFuncs sources_funcs = {
		.prepare = prepare_func,
		.check = check_func,
		.dispatch = dispatch_func,
		.finalize = finalize_func
	};
	//sources_funcs.

	GPollFD fd = {0, 0, 0};
	fd.events |= G_IO_IN;

	GSource * source = g_source_new(&sources_funcs, sizeof(GSource));
	//g_source_add_poll(source, &fd);
	g_source_set_callback(source, source_func, loop, NULL);
	g_source_attach(source, context);
	g_source_unref(source);
	g_main_loop_run(loop);
	//g_main_context_unref(context);
	g_main_loop_unref(loop);
	return 0;
}

