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
Recibe: Puntero a la matriz del tablero, posicion en el formato 'YXDD'
    en el tablero
Retorna: vacio
*/
int set_Stone(char ***board, char *pos, int size)
{
    int k = 0;

    int x = *pos - 'A';
    int y = *(pos+1) - '0';

    //validaciones
    if(*pos=='\0')
        return 0;
    if(x > 8 || x < 0)
        return 0;
    if(y > 9 || y < 0)
        return 0;
    if(*(pos+2)!='R' && *(pos+2)!='B')
    {
        if((*(pos+2) - '0') > 3 || (*(pos+2) - '0') < 0)
            return 0;
        y = y*10 + (*(pos+2) - '0');
    }

    switch(size)
    {
        case 0: //cambiar a 1
            if(x<2 || x>6 || y<3 || y>10)
                return 0;
        case 1: //cambiar a 2
            if(x<1 || x>7 || y<2 || y>11)
                return 0;

    }

    //int i;
    //for (i=0; i< sizeof(pos);i++)
      //  g_printf("%s\n", (pos+i));

    
    if (y<10)
    {
        g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+x)+y)+k) = *(pos+2+k);

        return 1;
    }
    else
    {
        g_printf("%d %d", x ,y);
        
        for(k=0;k<2;k++)
            *(*(*(board+x)+y)+k) = *(pos+3+k);

        return 1;
    }
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