#ifndef STDIO
#define STDIO

#include <stdlib.h>
#include <string.h>

#endif

#include <stdio.h>
#include "../include/board.h"

/* 
Crea el tablero
Recibe: Puntero a la matriz del tablero, cantidad de filas y columnas
Retorna: vacio
*/
char*** create_board(char ***board, int rows, int columns)
{
	int i = 0, x = 0, k = 0, count = 0;
    char* string = "  ";

	board=(char ***) malloc(rows * sizeof(char **));

    for(i=0;i<rows;i++)
    {
        board[i]=(char **)malloc(columns * sizeof(char *));
        for(x=0;x<columns;x++)
            board[i][x]= (char *)malloc(3*sizeof(char));
    }
    
    for(i=0; i<rows; i++)
    {
    	for(x=0; x<columns; x++)
    	{
    		strcpy(board[i][x], string);
    	}
    }

	return board;
}


void free_board(char ***board, int columns, int rows)
{
    int currentPtr, i=0, x=0; 

    for(i=0;i<rows;i++)
    {
        for(x=0;x<columns;x++)
            free(board[i][x]);
        free(board[i]);
    }

    free(board);
}


/* 
Coloca la pieza en el lugar indicado del tablero
Recibe: Puntero a la matriz del tablero, posicion en el tablero
Retorna: vacio
*/
int set_Stone(char ***board, char *pos, int size)
{
    int k = 0;

    //validaciones
    if(*pos=='\0')
        return 0;

    int y = *pos - 'A';
    int x = *(pos+1) - '0';

    if(y > 8 || y < 0)
        return 0;
    if(x > 9 || x < 0)
        return 0;
    if(*(pos+2)!='R' && *(pos+2)!='B') //verifico si se ingreso la casilla >10
    {
        if((*(pos+2) - '0') > 3 || (*(pos+2) - '0') < 0) //Verifico que se hay ingresado una casilla entre 10 y 13
            return 0;
        x = x*10 + (*(pos+2) - '0');
        if( (*(pos+3)!='R' && *(pos+3)!='B') || //Como se ingreso una casilla de dos digitos, verifico
            (*(pos+4)!='R' && *(pos+4)!='B') )  //que se haya ingresado valores correctos en los caracteres 3 y 4, 
                                                //que corresponden a los colores superior y norte, respectivamente
        {
            return 0;
        }
    }
    else
        if( (*(pos+2)!='R' && *(pos+2)!='B') || //Como se ingreso una casilla de dos digitos, verifico
            (*(pos+3)!='R' && *(pos+3)!='B') )  //que se haya ingresado valores correctos en los caracteres 2 y 3, 
                                                //que corresponden a los colores superior y norte, respectivamente
        {
            return 0;
        }

    switch(size)
    {
        case 1: //pequeno, empieza en la casilla 2C
            if(x<3 || x>10 || y<2 || y>6)
                return -1;
        case 2: //mediano
            if(x<2 || x>11 || y<1 || y>7)
                return -1;

    }

    //int i;
    //for (i=0; i< sizeof(pos);i++)
      //  g_printf("%s\n", (pos+i));

    if(*(*(*(board+y)+x)+1)!=' ')
        return -2;

    if (x<10)
    {
        //g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+y)+x)+k) = *(pos+2+k);

        return 1;
    }
    else
    {
        //  g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+y)+x)+k) = *(pos+3+k);

        return 1;
    }
}

int count_Dolmen(char ***board, int max_x, int max_y, char color)
{
    int result = 0;
    
    int y = 0;
    int x = 0;

    int labeled_board[max_y][max_x];
    for (y = 0; y < max_y; y++)
        for (x = 0; x < max_x; x++)
            labeled_board[y][x] = 0;

    int equival_relationship[20][2];
    for (y = 0; y < 20; y++)
    {
        equival_relationship[y][0] = y;
        equival_relationship[y][1] = 0;
    }

    int dolmen_sizes[20][2];
    for (y = 0; y < 20; y++)
    {
        dolmen_sizes[y][0] = y;
        dolmen_sizes[y][1] = 0;
    }

    int curr_label = 0;

    //Connected-component labeling, 2 pass algorithm
    //https://en.wikipedia.org/wiki/Connected-component_labeling
    for (y = 0; y < max_y; y++)
    {
        for (x = 0; x < max_x; x++)
        {
            if(*(*(*(board+y)+x)) == color)
            {
                //Do both pixels to the North and West of the current pixel have the same value as the current 
                //pixel but not the same label?
                if((x>0 && y>0 )
                    && (*(*(*(board+y)+x-1)) == color)
                    && (*(*(*(board+y-1)+x)) == color)
                    && (labeled_board[y][x-1] != labeled_board[y-1][x]))
                {
                    //Busco el menor para ponerlo en la matriz de etiquetas

                    //El de la izquierda es menor que el de arriba
                    if(labeled_board[y][x-1] < labeled_board[y-1][x])
                    {
                        labeled_board[y][x] = labeled_board[y][x-1];
                        //Lo asigno en la tabla de equivalencia
                        equival_relationship[labeled_board[y-1][x]][1] = labeled_board[y][x-1];                        
                    }
                    //El de arriba es menor que el de la izquierda
                    else
                    {
                        labeled_board[y][x] = labeled_board[y-1][x];
                        //Lo asigno en la tabla de equivalencia
                        equival_relationship[labeled_board[y][x-1]][1] = labeled_board[y-1][x];
                    }
                    continue;
                }

                //Does the pixel to the left (West) have the same value as the current pixel?
                if(x>0 && *(*(*(board+y)+x-1)) == color)
                {
                    labeled_board[y][x] = labeled_board[y][x-1];
                    continue;
                }

                //Does the pixel to the left (West) have a different value and the one to the North 
                //the same value as the current pixel?
                if(x>0 && y>0 && *(*(*(board+y-1)+x)) == color)
                {
                    labeled_board[y][x] = labeled_board[y-1][x];
                    continue;
                }

                labeled_board[y][x] = ++curr_label;
                equival_relationship[curr_label][1] = curr_label;
            }
        }
    }

    //Reemplazo los labels de acuerdo de la tabla de relacion de equivalencia
    for (y = 0; y < max_y; y++)
    {
        for (x = 0; x < max_x; x++)
        {
            labeled_board[y][x] = equival_relationship[labeled_board[y][x]][1];
        }
    }

    //Cuento los tamaños de cada bloque que hay y los guardo
    for (y = 0; y < max_y; y++)
        for (x = 0; x < max_x; x++)
            dolmen_sizes[labeled_board[y][x]][1]++;

    //Cuento la cantidad de bloques que hay
    for (y = 1; y < 20; y++)
        if (dolmen_sizes[y][1] > 2)
        {
            result++;
        }






    for (y = 0; y < max_y; y++)
    {
        for (x = 0; x < max_x; x++)
            g_printf("%d ", labeled_board[y][x]);
        g_printf("%s", "\n");
    }

    for (y = 0; y < 20; y++)
    {
        for (x = 0; x < 2; x++)
            g_printf("%d ", equival_relationship[y][x]);
        g_printf("%s", "\n");
    }

    return result;

}