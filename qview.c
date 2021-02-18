#include <gtk/gtk.h>
#include <gdk/gdk.h>

static gboolean image_resize(GtkWidget* widget, GtkAllocation* allocation, gpointer data);

static GtkWidget* image;

int main(int argc, char** argv)
{
	//GtkWidget* image;
	GtkWidget* window;
	GtkWidget* viewport;
	GdkPixbuf* pixbuf = NULL;
	GdkColor background_color = {};
	const char* filename = argv[1];

	background_color.red = 0;
	background_color.green = 0;
	background_color.blue = 0;

	gtk_init(&argc, &argv);

	pixbuf = gdk_pixbuf_new_from_file(filename, 0);
	if (pixbuf == NULL)
	{
		printf("ERROR: INVALID FILE TYPE/NAME\n");
		return 1;
	}

	image = gtk_image_new_from_pixbuf(gdk_pixbuf_copy(pixbuf ));

	viewport = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_min_content_width(GTK_SCROLLED_WINDOW(viewport), 1);
	gtk_scrolled_window_set_min_content_height(GTK_SCROLLED_WINDOW(viewport), 1);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_modify_bg(window, GTK_STATE_NORMAL, &background_color);
	gtk_window_set_title(GTK_WINDOW(window), "qview");

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(viewport, "size-allocate", G_CALLBACK(image_resize), pixbuf);

	gtk_container_add(GTK_CONTAINER(viewport), image);
	gtk_container_add(GTK_CONTAINER(window), viewport);

	gtk_window_set_default_size(GTK_WINDOW(window),
					gdk_pixbuf_get_width(pixbuf),
					gdk_pixbuf_get_height(pixbuf));

	gtk_widget_show_all(window);
	gtk_main();

	printf("\n");

	return 0;
}

static gboolean image_resize(GtkWidget* widget, GtkAllocation* allocation, gpointer data)
{
	GdkPixbuf* pixbuf = data;
	GdkPixbuf* image_pixbuf;

	image_pixbuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
	if (image_pixbuf == NULL)
		return TRUE;

	if (allocation->width != gdk_pixbuf_get_width(image_pixbuf) ||
		allocation->height != gdk_pixbuf_get_height(image_pixbuf))
	{

		gtk_image_set_from_pixbuf(
			GTK_IMAGE(image),
			gdk_pixbuf_scale_simple(
				pixbuf,
				allocation->width,
				allocation->height,
				GDK_INTERP_BILINEAR
			)
		);
		g_object_unref(image_pixbuf);
	}

	return FALSE;
}
