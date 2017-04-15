static void do_drawing(cairo_t *);
static void draw_board(cairo_t *);
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
	gpointer user_data);


static void on_entry_activate(GtkEntry *entry,gpointer data);
static void on_button_clicked(GtkToggleButton *button,gpointer data);