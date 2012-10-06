/*
 * mainloop.c
 *
 *  Created on: Sep 28, 2012
 *      Author: hujin
 */



#include <stdio.h>
#include <glib.h>
#include <glib/gthread.h>
#include <libudev.h>

typedef struct _UdevSource {
	GSource source;
	struct udev_monitor * monitor;
	struct udev * udev;
	int fd;
}UdevSource;


gboolean prepare_func(GSource * source, gint * timeout) {
	UdevSource * udev_source = (UdevSource *)source;
	printf("udev fd: %d\n", udev_source->fd);
	//*timeout = 1000;
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


static void print_attrs(struct udev_device * device) {
	struct udev_list_entry * list_entry, * curr;
	char * name, * value;
	list_entry = udev_device_get_sysattr_list_entry(device);
	printf("SubSystem: %s\n", udev_device_get_subsystem(device));
	printf("Action: %s\n", udev_device_get_action(device));
	printf("SysAttr:\n");
	udev_list_entry_foreach(curr, list_entry) {
		name = udev_list_entry_get_name(curr);
		value = udev_device_get_sysattr_value(device, name);
		printf("    %s: %s\n", name, value);
	}
	printf("Properties:\n");
	list_entry = udev_device_get_properties_list_entry(device);
	udev_list_entry_foreach(curr, list_entry) {
		name = udev_list_entry_get_name(curr);
		value = udev_device_get_property_value(device, name);
		printf("    %s: %s\n", name, value);
	}
	printf("\n\n");
}


gboolean dispatch_func(GSource * source, GSourceFunc callback, gpointer user_data) {
	printf("dispatch:callback %p\n", callback);
	GMainLoop * loop = user_data;
	UdevSource * udev_source = (UdevSource *)source;
	struct udev_device * device;
	device = udev_monitor_receive_device(udev_source->monitor);
	if(device) {
		print_attrs(device);
		udev_device_unref(device);
	}
	//g_main_loop_quit(loop);
	return TRUE;
}

GSourceFuncs sources_funcs = {
	.prepare = prepare_func,
	.check = check_func,
	.dispatch = dispatch_func,
	.finalize = finalize_func
};

UdevSource * udev_source_new() {
	UdevSource * ret = (UdevSource *) g_source_new(&sources_funcs, sizeof(UdevSource));
	ret->udev = udev_new();
	ret->monitor = udev_monitor_new_from_netlink(ret->udev, "udev");
	udev_monitor_filter_add_match_subsystem_devtype(ret->monitor, "backlight", NULL);
	udev_monitor_filter_add_match_subsystem_devtype(ret->monitor, "power_supply", NULL);
	udev_monitor_enable_receiving(ret->monitor);
	ret->fd = udev_monitor_get_fd(ret->monitor);
	return ret;
}


int main(int argc, char **argv) {
	GMainLoop * loop = g_main_loop_new(NULL, 0);
	GMainContext * context = g_main_loop_get_context(loop);


	//sources_funcs.
	UdevSource * source = udev_source_new();
	GPollFD fd = {0, 0, 0};
	fd.fd = source->fd;
	fd.events |= G_IO_IN;
	g_source_add_poll(source, &fd);

	g_source_set_callback(source, source_func, loop, NULL);
	g_source_attach(source, context);
	g_source_unref(source);
	g_main_loop_run(loop);
	//g_main_context_unref(context);
	g_main_loop_unref(loop);
	return 0;
}

