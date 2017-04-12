#include <stdio.h>
#include <allegro5/allegro.h>
#include "../include/board.h"

#define BG_COLOR 128,128,128

float FPS = 60;

char ***board;

int main(int size, int argc, char **argv){

   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool redraw = true;

   switch (size)
   {
   		case 1:
   			create_board(board, 8,5);
   			break;
   		case 2:
   			create_board(board, 10,7);
   			break;
   		case 3:
   			create_board(board, 14,9);
   			break;
   		default:
   			create_board(board, 10, 7);
   }
   
   

   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }

   display = al_create_display(640, 480);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      al_destroy_timer(timer);
      return -1;
   }
 
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
 
   al_register_event_source(event_queue, al_get_display_event_source(display));

   al_register_event_source(event_queue, al_get_timer_event_source(timer));

   al_clear_to_color(al_map_rgb(BG_COLOR));// Limpia la pantalla del color indicado
   										// al_map_rgb obtiene el parametro ALLEGRO_COLOR 
   										//necesario para rellenar la pantalla del color deseado

   al_flip_display(); // Hace que la operacion de dibujo sea visible

   al_start_timer(timer);
 
   while(1)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);

      if(ev.type == ALLEGRO_EVENT_TIMER) {
         redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }

      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
         al_clear_to_color(al_map_rgb(BG_COLOR));
         al_flip_display();
      }
   }

   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 	

   free(board);
   return 0;
}