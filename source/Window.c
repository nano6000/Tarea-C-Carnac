#include <cairo.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "../include/window.h"
#include "../include/board.h"

#define WIDTH 1000.0
#define HEIGHT 650.0

typedef struct stone
{
   int x;
   int y;
   int dest_x;
   int dest_y;
   char up_face;
   char north_face;
} stones[28];

char ***board;  //tablero del juego
int tamano;
int cells_in_x = 0; //cantidad de celdas horizontales
int cells_in_y = 0; //cantidad de celdas verticales
int turno = 0;
int stock = 28; //cantidad de piezas disponibles 

char* red_score;
char* white_score;

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

   //Asigno las acciones de los componentes

   g_signal_connect(G_OBJECT(draw_area), "draw", 
      G_CALLBACK(on_draw_event), NULL); 

   g_signal_connect(window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   g_signal_connect(G_OBJECT(buttonIA),"clicked",
      G_CALLBACK(on_button_clicked),entry);

   g_signal_connect(input_window, "destroy",
      G_CALLBACK(gtk_main_quit), NULL);

   g_signal_connect(G_OBJECT(entry),"activate",/* when you press ENTER */
      G_CALLBACK(on_entry_activate), window);

   //creacion del tablero y sus propiedades
   gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_window_set_default_size(GTK_WINDOW(window), WIDTH, HEIGHT); 
   gtk_window_set_title(GTK_WINDOW(window), "Carnac");

   gtk_window_set_position(GTK_WINDOW(input_window), GTK_WIN_POS_NONE);
   gtk_window_set_title(GTK_WINDOW(input_window), "Movimientos");

   //input del tamano del tablero
   int eleccion;
   printf("Seleccione el tamano del tablero:"
      "\n1)Vaticano\n2)Alemania (default) \n3)Siberia\n");
   scanf("%d", &eleccion);

   tamano = eleccion;

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
         tamano = 2;
   }

   board = create_board(board, cells_in_y, cells_in_x);
   red_score = malloc(2 * sizeof(char));
   white_score = malloc(2 * sizeof(char));

   strcpy(red_score, "0\0");
   strcpy(white_score, "0\0");
   
   gtk_widget_show_all(window);
   gtk_widget_show_all(input_window);


   gtk_main();

   free_board(board, cells_in_x, cells_in_y);

   return 0;

}

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
   gpointer user_data)
{
   draw_board(cr);
   draw_pieces(cr);
   draw_score(cr);

   return FALSE;
}


static void on_button_clicked(GtkToggleButton *button,gpointer data)
{
   GtkWidget *entry=(GtkWidget *)data;
   if (gtk_toggle_button_get_active(button))
      gtk_widget_set_sensitive (entry, FALSE);
   else
      gtk_widget_set_sensitive (entry, TRUE);

}

static void on_entry_activate(GtkEntry *entry, GtkWidget *widget)
{
   int exit_message = 0;
   g_printf("%s\n",gtk_entry_get_text(GTK_ENTRY(entry)));
   char *inst = (char *) gtk_entry_get_text(GTK_ENTRY(entry));

   exit_message = set_Stone(board, inst, tamano);
   *red_score = count_Dolmen(board, cells_in_x, cells_in_y, 'R', 'R') + '0';
   *white_score = count_Dolmen(board, cells_in_x, cells_in_y, 'B', 'R') + '0';

   stock--;

   if (exit_message!=1)
   {
      GtkWidget *error;
      char *message = (char *) malloc(82 * sizeof(char));
      stock++;

      switch(exit_message)
      {
         case -2:
            strcpy(message, "¡Error! La casilla ya esta ocupada\
                  \nseleccione otra casilla.");
            break;
         case -1:
            strcpy(message, "¡Error! La casilla indicada se encuentra\
                  \nfuera de los limites del tablero actual.");
            break;
         case 0:
            strcpy(message, "¡Error! Coordenadas de la casilla o colores\
                  \nerroneos.");
            break;
      }

      error = gtk_message_dialog_new(GTK_WINDOW(widget),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            message); 
      gtk_dialog_run(GTK_DIALOG (error) );
      gtk_widget_destroy(error);
   }

   gtk_entry_set_text (GTK_ENTRY(entry), "");

   gtk_widget_queue_draw(widget);

   if(!stock)
   {
      GtkWidget *win;
      char *win_message = (char *) malloc(100 * sizeof(char));
      int result_red = *red_score - '0';
      int result_white = *white_score - '0';

      //Gana el rojo
      if (result_red > result_white)
      {
         strcpy(win_message, "Fin de la partida!\nEl Ganador es el jugador rojo, por mayor cantidad de dolmens");
      }
      //Gana el blanco
      if (result_red < result_white)
         strcpy(win_message, "Fin de la partida!\nEl Ganador es el jugador blanco, por mayor cantidad de dolmens");
      //Quedan empates
      if (result_red == result_white)
      {
         *red_score = count_Dolmen(board, cells_in_x, cells_in_y, 'R', 'D') + '0';
         *white_score = count_Dolmen(board, cells_in_x, cells_in_y, 'B', 'D') + '0';

         result_red = *red_score - '0';
         result_white = *white_score - '0';

         //Gana el rojo
         if (result_red>result_white)
         {
            strcpy(win_message, "Fin de la partida!\nEl Ganador es el jugador rojo, por dolmen mas grande: ");
            strcat(win_message, red_score);
         }
         //Gana el blanco
         if (result_red < result_white)
         {
            strcpy(win_message, "Fin de la partida!\nEl Ganador es el jugador blanco, por dolmen mas grande: ");
            strcat(win_message, white_score);
         }
         //Volvieron a quedar empates
         if (result_red == result_white)
            strcpy(win_message, "Fin de la partida!\nEl Ganador es el jugador rojo, por mandato divino de Lord Kirstein");

      }

      win = gtk_message_dialog_new(GTK_WINDOW(widget),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            win_message); 
      gtk_dialog_run(GTK_DIALOG (win) );
      gtk_widget_destroy(win);
   }
}

static void draw_board(cairo_t *cr)
{  
   //Desplazamiento en el arreglo de coordenadas
   int desp_x = 0;
   int desp_y = 0;
   switch (tamano)
   {
      case 1:
         desp_x = 3;
         desp_y = 2;
         break;
      case 2:
         desp_x = 2;
         desp_y = 1;
         break;
   }
   char *pos_x[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", 
               "9", "10", "11", "12", "13", ""};
   char *pos_y[] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", ""};

   //pinto el fondo de la pantalla
   cairo_set_source_rgba(cr, 0, 0.4, 0, 0.75);
   cairo_set_line_width(cr, 2000);

   cairo_move_to(cr, 0, 0);
   cairo_line_to(cr, WIDTH, HEIGHT);

   cairo_stroke(cr);

   //inicializo las propiedades del texto
   cairo_select_font_face(cr, "Purisa",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);

   cairo_set_font_size(cr, 13);

   //inicializo las propiedades de las lineas
   cairo_set_source_rgb(cr, 0, 0, 0);
   cairo_set_line_width(cr, 1);

   //tamano de cada celda del tablero
   double size_cell_x = (WIDTH-100)/cells_in_x;
   double size_cell_y = (HEIGHT-200)/cells_in_y;

   //punto de origen de la esquina superior izquierda del tablero
   double origin_x = 50 - size_cell_x;
   double origin_y = 150;

   int i;
   for (i = 0; i <= cells_in_x; i++ ) 
   {
      origin_x += size_cell_x;

      //Dibujo lineas
      cairo_move_to(cr, origin_x, origin_y);
      cairo_line_to(cr, origin_x, origin_y + (HEIGHT-200));

      if (i<cells_in_x)
      {
         //Dibujo letras superiores
         cairo_move_to(cr, origin_x + (size_cell_x/2) - 5, origin_y - 10);
         cairo_show_text(cr, pos_x[i + desp_x]);
   
         //Dibujo letras inferiores
         cairo_move_to(cr, origin_x + (size_cell_x/2) - 5, 
                     origin_y + (HEIGHT-200) + 20);
         cairo_show_text(cr, pos_x[i + desp_x]);
      }
   }

   for (i = 0; i <= cells_in_y; i++ ) 
   {
      //Dibujo lineas
      cairo_move_to(cr, origin_x, origin_y);
      cairo_line_to(cr, origin_x - (WIDTH-100), origin_y);

      if (i<cells_in_y)
      {
         //Dibujo letras derechas
         cairo_move_to(cr, origin_x + 10, 
                        origin_y + (size_cell_y/2));
         cairo_show_text(cr, pos_y[i + desp_y]);
   
         //Dibujo letras izquierdas
         cairo_move_to(cr, origin_x - (WIDTH-100) - 20, 
                        origin_y + (size_cell_y/2));
         cairo_show_text(cr, pos_y[i + desp_y]);
      }

      origin_y += size_cell_y;
   }
   cairo_stroke(cr);

}

static void draw_pieces(cairo_t *cr)
{
   int i, j;
   char color;

   double size_cell_x = (WIDTH-100)/cells_in_x;
   double size_cell_y = (HEIGHT-200)/cells_in_y;

   double origin_x = 55;
   double origin_y = 155;

   double dest_x = 0;
   double dest_y = 0;

   cairo_set_line_cap(cr, CAIRO_LINE_CAP_ROUND); 
   cairo_set_line_width(cr, 5);

   //cairo_pattern_t *pat;

   for (i = 0; i<cells_in_y; i++)
   {
      for(j = 0; j<cells_in_x; j++)
      {
         //Verifico que haya una pieza en esa casilla
         if (*(*(*(board+i)+j))!=' ')
         {
            //###################################################
            //Pinto la casilla del color superior
            //###################################################

            color = *(*(*(board+i)+j)); //obtengo el color superior
            switch(color)
            {
               case 'R':
                  cairo_set_source_rgb(cr, 1, 0, 0);
                  break;
               case 'B':
                  cairo_set_source_rgb(cr, 1, 1, 1);
                  break;
            }
            /*
            pat = cairo_pattern_create_linear (0, 0, 0, 256);
            cairo_pattern_add_color_stop_rgba (pat, 0.75, 0, 0, 0, 1);
            cairo_pattern_add_color_stop_rgba (pat, 1, 1, 1, 1, 1);*/

            cairo_rectangle (cr, origin_x + (size_cell_x * j),
                                 origin_y + (size_cell_y * i),
                                 size_cell_x - 10,
                                 size_cell_y - 10);

            //cairo_set_source (cr, pat);
            cairo_fill(cr);

            //###################################################
            //Dibujo las lineas norte
            //###################################################

            color = *(*(*(board+i)+j)+1); //obtengo el color del norte
            switch(color)
            {
               case 'R':
                  cairo_set_source_rgb(cr, 1, 0, 0);
                  break;
               case 'B':
                  cairo_set_source_rgb(cr, 1, 1, 1);
                  break;
            }

            //Dibujo la linea de arriba del color del norte
            dest_y = origin_y + (size_cell_y * i);
            dest_x = origin_x + (size_cell_x * (j+1)) - 10;

            cairo_move_to(cr, origin_x + (size_cell_x * j),
                              origin_y + (size_cell_y * i));
            cairo_line_to(cr, dest_x, dest_y);

            cairo_stroke(cr);

            //###################################################
            //Dibujo las lineas este y oeste, las cuales son de
            //color opuesto a las del norte y sur
            //###################################################

            cairo_set_source_rgb(cr, 0, 0, 0);

            //Dibujo la linea de abajo del color superior
            dest_y = origin_y + (size_cell_y*(i+1)) - 10;
            dest_x = origin_x + (size_cell_x*(j+1)) - 10;

            cairo_move_to(cr, origin_x + (size_cell_x * j), 
                           dest_y);
            cairo_line_to(cr, dest_x, dest_y);

            cairo_stroke(cr);

            //Dibujo la linea izquierda del color superior
            dest_y = origin_y + (size_cell_y*(i+1)) - 10;
            dest_x = origin_x + (size_cell_x*j);

            cairo_move_to(cr, dest_x,
                              origin_y + (size_cell_y * i));
            cairo_line_to(cr, dest_x, dest_y);

            cairo_stroke(cr);

            //Dibujo la linea derecha del color superior
            dest_y = origin_y + (size_cell_y*(i+1)) - 10;
            dest_x = origin_x + (size_cell_x*(j+1)) - 10;

            cairo_move_to(cr, dest_x,
                              origin_y +  (size_cell_y * i));
            cairo_line_to(cr, dest_x, dest_y);

            cairo_stroke(cr);
         }
      }
      origin_x = 55;
      origin_y = 155;
   }
}


static void draw_score(cairo_t *cr)
{
   //Dibujo el marcador
   cairo_select_font_face(cr, "Courier",
      CAIRO_FONT_SLANT_NORMAL,
      CAIRO_FONT_WEIGHT_BOLD);

   cairo_set_font_size(cr, 100);

   cairo_set_source_rgba(cr, 0.8, 0, 0, 1);

   //Marcador Rojo
   cairo_move_to(cr, (WIDTH/2) - 110, 100);
   cairo_show_text(cr, red_score);

   //Separador
   cairo_set_source_rgba(cr, 0, 0, 0, 1);

   cairo_move_to(cr, (WIDTH/2) - 50, 100);
   cairo_show_text(cr, "-");

   //Marcador Blanco
   cairo_set_source_rgba(cr, 1, 1, 1, 1);

   cairo_move_to(cr, (WIDTH/2) + 10, 100);
   cairo_show_text(cr, white_score);

}

