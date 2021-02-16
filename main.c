#include <time.h>
#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

struct _resize_widgets {
	GtkWidget *image;
	GdkPixbuf *pixbuf;
};

typedef struct _resize_widgets ResizeWidgets;

gboolean resize_image(GtkWidget *widget, GdkRectangle *allocation, gpointer user_data)
{
	GdkPixbuf *pxbscaled;
	GtkWidget *image = (GtkWidget *) ((ResizeWidgets*) user_data)->image; 
	GdkPixbuf *pixbuf = (GdkPixbuf *) ((ResizeWidgets*) user_data)->pixbuf; 
	pxbscaled = gdk_pixbuf_scale_simple(pixbuf, allocation->width, allocation->height, GDK_INTERP_BILINEAR);
	gtk_image_set_from_pixbuf(GTK_IMAGE(image), pxbscaled);
	g_object_unref (pxbscaled);
	return FALSE;
}

int main(int argc, char **argv)
{
	GtkWidget *window = NULL;
	GtkWidget *image = NULL;
	GtkWidget *container = NULL;
	GdkPixbuf *pixbuf = NULL;
	GdkColor color;
	ResizeWidgets *widgets;

	color.red = 0x0000;
	color.green = 0x0000;
	color.blue = 0x0000;

	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &color);
	container = gtk_layout_new(NULL, NULL);
	image = gtk_image_new();

	pixbuf = gdk_pixbuf_new_from_file(argv[1], NULL);
	if (pixbuf == NULL)
	{
		printf("\x1b[38;2;255;255;255m[\x1b[38;2;255;0;0mERROR\x1b[38;2;255;255;255m] INVALID FILE\n");
		exit(1);
	}

	widgets = g_new0(ResizeWidgets, 1);
	widgets->image = image;
	widgets->pixbuf = pixbuf;

	gtk_container_add(GTK_CONTAINER(window), container);
	gtk_layout_put(GTK_LAYOUT(container), image, 0, 0);

	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(container, "size-allocate", G_CALLBACK(resize_image), widgets);

	gtk_widget_show_all(GTK_WIDGET(window));

	gtk_widget_set_size_request(GTK_WIDGET(window), 100, 100);

	gtk_main();

	g_object_unref(pixbuf);
	g_free(widgets);

	return 0;
}
