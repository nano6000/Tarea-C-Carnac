#include <stdlib.h>
#include <stdio.h>
#include "../include/board.h"

/* 
Crea el tablero
Recibe: Puntero a la matriz del tablero, cantidad de filas y columnas
Retorna: vacio
*/
void create_board(char ***board, int rows, int columns)
{
	int i = 0;
	int j = 0, count = 0;

	board=(char ***) malloc(rows * sizeof(char **));

    for(i=0;i<rows;i++)
    {
        board[i]=(char **)malloc(columns * sizeof(char *));
        for(j=0;j<columns;j++)
            board[i][j]=(char *)malloc(3 * sizeof(char));
    }
    
    for(i=0; i<rows; i++)
    {
    	for(j=0; j<columns; j++)
    	{
    		board[i][j] = "  ";
    	}
    }

	return ;
}

/* 
Coloca la pieza en el lugar indicado del tablero
Recibe: Puntero a la matriz del tablero, posicion en el formato 'YXDD'
    en el tablero
Retorna: vacio
*/
int set_Stone(char ***board, char *pos, int size)
{
    if(*pos=='\0')
    {
    //    g_printf("%d\n", sizeof(*pos));

        return 0;
    }
    //int i;
    //for (i=0; i< sizeof(pos);i++)
      //  g_printf("%s\n", (pos+i));

    int x = *pos - 'A';
    int y = *(pos+1) - '0';
    if (!(*(pos+2)-'A'>=0))
        y = y*10 + (*(pos+2) - '0');

    g_printf("%d %d", x ,y);
    board[y][x] = (pos+3);

    return 0;
}


void free_board(char ***board, int columns, int rows)
{
    int currentPtr, i=0, j=0; 

    for(i=0;i<rows;i++)
    {
        for(j=0;j<columns;j++)
            free(board[i][j]);
        free(board[i]);
    }

    free(board);
}