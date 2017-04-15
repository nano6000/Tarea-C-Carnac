#include <cairo.h>
#include <gtk/gtk.h>
#include "../include/window.h"
#include "../include/board.h"

#define WIDTH 1000
#define HEIGHT 600

char ***board;  //tablero del juego
int cells_in_x = 10; //cantidad de celdas horizontales
int cells_in_y = 10; //cantidad de celdas verticales

main(int argc, char *argv[])
{
   GtkWidget *window;
   GtkWidget *draw_area;
   GtkWidget *hbox;
   GtkWidget *entry;
   GtkWidget *input_window;
   GtkWidget *buttonIA;

   gtk_init(&argc, &argv);

   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   input_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

   draw_area = gtk_drawing_area_new();

   hbox=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,5); 
   gtk_container_add(GTK_CONTAINER(input_window),hbox);

   entry=gtk_entry_new();
   gtk_box_pack_start(GTK_BOX(hbox),entry,TRUE,TRUE,5);

   buttonIA=gtk_toggle_button_new_with_label("IA On/Off");
   gtk_box_pack_start(GTK_BOX(hbox),buttonIA,FALSE,FALSE,0);


   gtk_container_add(GTK_CONTAINER(window), draw_area);

   g_signal_connect(G_OBJECT(draw_area), "draw", 
      G_CALLBACK(on_draw_event), NULL); 

   g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   g_signal_connect(G_OBJECT(buttonIA),"clicked",
      G_CALLBACK(on_button_clicked),entry);

   g_signal_connect(input_window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   g_signal_connect(G_OBJECT(entry),"activate",/* when you press ENTER */
      G_CALLBACK(on_entry_activate),NULL);

   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT); 
   gtk_window_set_title(GTK_WINDOW(window), "Carnac");

   gtk_window_set_position(GTK_WINDOW(input_window), GTK_WIN_POS_CENTER);
   gtk_window_set_title(GTK_WINDOW(input_window), "Movimientos");

   gtk_widget_show_all(window);
   gtk_widget_show_all(input_window);

   int eleccion;
   printf("Seleccione el tamano del tablero:"
      "\n1)Vaticano\n2)Alemania\n3)Siberia\n");
   scanf("%d", &eleccion);

   switch(eleccion)
   {
      case 1:
         cells_in_x = 8;
         cells_in_y = 5;
         break;
      case 2:
         cells_in_x = 10;
         cells_in_y = 7;
         break;
      case 3:
         cells_in_x = 14;
         cells_in_y = 9;
         break;
      default:
         cells_in_x = 10;
         cells_in_y = 7;
   }

   create_board(board, 5,5);

   gtk_main();

   return 0;

}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
   gpointer user_data)
{
   draw_board(cr);

   return FALSE;
}

/*static void do_drawing(cairo_t *cr)
{
   cairo_set_source_rgb(cr, 0, 0, 0);
   cairo_set_line_width(cr,1);


}*/

static void on_button_clicked(GtkToggleButton *button,gpointer data)
{
   GtkWidget *entry=(GtkWidget *)data;
   if (gtk_toggle_button_get_active(button))
      gtk_widget_set_sensitive (entry, FALSE);
   else
      gtk_widget_set_sensitive (entry, TRUE);

}

static void on_entry_activate(GtkEntry *entry,gpointer data)
{
    g_printf("%s\n",gtk_entry_get_text(GTK_ENTRY(entry)));

    gtk_entry_set_text (GTK_ENTRY(entry), "");
}

static void draw_board(cairo_t *cr)
{
   cairo_set_source_rgb(cr, 0, 0, 0);
   cairo_set_line_width(cr, 1);

   int size_cell_x = (WIDTH-100)/cells_in_x;
   int size_cell_y = (HEIGHT-100)/cells_in_y;

   int origin_x = 50 - size_cell_x;
   int origin_y = 50;

   int i;
   for (i = 0; i <= cells_in_x; i++ ) 
   {
      origin_x += size_cell_x;

      cairo_move_to(cr, origin_x, origin_y);
      cairo_line_to(cr, origin_x, origin_y + (HEIGHT-100));
   }
   for (i = 0; i <= cells_in_y; i++ ) 
   {
      cairo_move_to(cr, origin_x, origin_y);
      cairo_line_to(cr, origin_x - (WIDTH-100), origin_y);

      origin_y += size_cell_y;
   }

   cairo_stroke_preserve(cr);
}

