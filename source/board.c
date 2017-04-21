#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/board.h"

/* 
Crea el tablero
Recibe: Puntero a la matriz del tablero, cantidad de filas y columnas
Retorna: vacio
*/
char*** create_board(char ***board, int rows, int columns)
{
	int i = 0, j = 0, k = 0, count = 0;
    char* string = "  ";

	board=(char ***) malloc(rows * sizeof(char **));

    for(i=0;i<rows;i++)
    {
        board[i]=(char **)malloc(columns * sizeof(char *));
        for(j=0;j<columns;j++)
            board[i][j]= (char *)malloc(3*sizeof(char));
    }
    
    for(i=0; i<rows; i++)
    {
    	for(j=0; j<columns; j++)
    	{
    		strcpy(board[i][j], string);
    	}
    }

	return board;
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
        if((*(pos+2) - '0') > 3 || (*(pos+2) - '0') < 0)
            return 0;
        x = x*10 + (*(pos+2) - '0');
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

    if (y<10)
    {
        g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+y)+x)+k) = *(pos+2+k);

        return 1;
    }
    else
    {
        g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+x)+y)+k) = *(pos+3+k);

        return 1;
    }
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