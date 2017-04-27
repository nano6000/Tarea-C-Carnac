#ifndef STDIO
#define STDIO

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#endif

#include "../include/board.h"

/* 
Crea el tablero
Recibe: Puntero a la matriz del tablero, cantidad de filas y columnas
Retorna: vacio
*/
char*** create_board(char ***board, int rows, int columns)
{
	int y = 0, x = 0, k = 0, count = 0;
    char* string = "  ";

	board=(char ***) malloc(rows * sizeof(char **));

    for(y=0;y<rows;y++)
    {
        board[y]=(char **)malloc(columns * sizeof(char *));
        for(x=0;x<columns;x++)
            board[y][x]= (char *)malloc(3*sizeof(char));
    }
    
    for(y=0; y<rows; y++)
    {
    	for(x=0; x<columns; x++)
    	{
    		strcpy(board[y][x], string);
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
Recibe: Puntero a la matriz del tablero, posicion en el tablero, tamano del tablero
    punteros x y y para guardar la ubicacion de la ultima pieza
Retorna: entero, que sera tratado como codigos de salida (error y exito)
    -2 -> La casilla ya esta ocupada\
    -1 -> La casilla indicada se encuentra fuera de los limites del tablero actual
     0 -> Coordenadas de la casilla o colores erroneos
     1 -> Exito
*/
int set_Stone(char ***board, char *pos, int size, int *x, int *y)
{
    int k = 0;
    int mayor_10 = 0;

    //validaciones
    if(*pos=='\0')
        return 0;

    *y = *pos - 'A';
    *x = *(pos+1) - '0';

    if(*y > 8 || *y < 0)
        return 0;
    if(*x > 9 || *x < 0)
        return 0;
    if(*(pos+2)!='R' && *(pos+2)!='B') //verifico si se ingreso la casilla >10
    {
        if((*(pos+2) - '0') > 3 || (*(pos+2) - '0') < 0) //Verifico que se hay ingresado una casilla entre 10 y 13
            return 0;
        *x = (*x) * 10 + (*(pos+2) - '0');
        mayor_10++;
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
        case 1: //pequeno, empieza en la casilla 3C
            if(*x<3 || *x>10 || *y<2 || *y>6)
                return -1;
            *x -= 3;
            *y -= 2;
            break;
        case 2: //mediano
            if(*x<2 || *x>11 || *y<1 || *y>7)
                return -1;
            *x -= 2;
            *y -= 1;
    }

    if(*(*(*(board+(*y))+(*x))+1)!=' ')
        return -2;

    if (!mayor_10)
    {
        for(k=0;k<2;k++)
            *(*(*(board+(*y))+(*x))+k) = *(pos+2+k);
        return 1;
    }
    else
    {
        for(k=0;k<2;k++)
            *(*(*(board+(*y))+(*x))+k) = *(pos+3+k);
        return 1;
    }
}

/* 
Verifico si es posible botar la pieza en al menos una direccion
Recibe: Puntero a la matriz del tablero, posicion en el tablero, tamano del tablero
    punteros x y y para unicar la ubicacion de la ultima pieza
Retorna: entero, que sera tratado como codigos de salida (error y exito)
        0 -> No es posible
        1 -> Es posible
*/
int verif_TakeDown(char ***board, int size, int *x, int *y)
{
    int max_y = 0, max_x = 0;
    switch(size)
    {
        case 1:
            max_x = 7;
            max_y = 4;
            break;
        case 2:
            max_x = 9;
            max_y = 6;
            break;
        case 3:
            max_x = 13;
            max_y = 8;
            break;
    }

    if(*y >= 2 && *(*(*(board + (*y-1)) + (*x))) == ' ' &&
            *(*(*(board + (*y-2)) + (*x))) == ' ')

        return 1;

    if(*x >= 2 && *(*(*(board + (*y)) + (*x-1))) == ' ' &&
            *(*(*(board + (*y)) + (*x-2))) == ' ')

        return 1;

    if(*y < max_y-2 && *(*(*(board + (*y+1)) + (*x))) == ' ' &&
                *(*(*(board + (*y+2)) + (*x))) == ' ')
            
        return 1;
            
    if(*x < max_x-2 && *(*(*(board + (*y)) + (*x+1))) == ' ' &&
                *(*(*(board + (*y)) + (*x+2))) == ' ')
            
        return 1;
            
    *x = -1;
    *y = -1;
    return 0;
}

/* 
Bota la pieza hacia la direccion indicada
Recibe: Puntero a la matriz del tablero, posicion en el tablero, tamano del tablero
    punteros x y y para unicar la ubicacion de la ultima pieza
Retorna: entero, que sera tratado como codigos de salida (error y exito)
        -2 -> El jugador decidio no botarlo
        -1 -> No es posible botarlo en esa direccion
         0 -> Direccion no valida
         1 -> Botado exitoso
*/
int takeDown_Stone(char ***board, char *pos, int size, int *x, int *y)
{
    int max_y = 0, max_x = 0;
    char *cell_state = malloc(3*sizeof(char));
    switch(size)
    {
        case 1:
            max_x = 7;
            max_y = 4;
            break;
        case 2:
            max_x = 9;
            max_y = 6;
            break;
        case 3:
            max_x = 13;
            max_y = 8;
            break;
    }

    //validaciones
    if(*pos=='\0' || (*pos!='-' && 
                      *pos!='N' && *pos!='O' &&
                      *pos!='S' &&  *pos!='E'))
    {
        free(cell_state);
        return 0;
    }

    switch(*pos)
    {
        case '-':
            *x = -1;
            *y = -1;
            free(cell_state);
            return -2;
        case 'N':
            if(*y<2)
            {
                free(cell_state);
                return -1;
            }
            if( *(*(*(board + (*y-1)) + (*x))) == ' ' &&
                *(*(*(board + (*y-2)) + (*x))) == ' ')
            {
                *(cell_state) = *(*(*(board+(*y))+(*x))+1);
                *(cell_state + 1) = *(*(*(board+(*y))+(*x)));

                strcpy(board[*y-1][*x], cell_state);
                strcpy(board[*y-2][*x], cell_state);
                strcpy(board[*y][*x], "  ");
                *x = -1;
                *y = -1;
                free(cell_state);
                return 1;
            }
            else
            {
                free(cell_state);
                return -1;
            }
        case 'E':
            if(*x<2)
            {
                free(cell_state);
                return -1;
            }
            if( *(*(*(board + (*y)) + (*x-1))) == ' ' &&
                *(*(*(board + (*y)) + (*x-2))) == ' ')
            {
                if(*(*(*(board+(*y))+(*x))+1) == 'B') //El norte es blanco, entonces la superficie tiene que ser el contrario
                    *(cell_state) = 'R';
                else    //El norte es rojo, entonces la superficie tiene que ser el contrario
                    *(cell_state) = 'B';
                *(cell_state + 1) = *(*(*(board+(*y))+(*x))+1);

                strcpy(board[*y][*x-1], cell_state);
                strcpy(board[*y][*x-2], cell_state);
                strcpy(board[*y][*x], "  ");
                *x = -1;
                *y = -1;
                free(cell_state);
                return 1;
            }
            else
            {
                free(cell_state);
                return -1;
            }
        case 'S':
            if(*y>max_y-2)
            {
                free(cell_state);
                return -1;
            }
            if( *(*(*(board + (*y+1)) + (*x))) == ' ' &&
                *(*(*(board + (*y+2)) + (*x))) == ' ')
            {
                if(*(*(*(board+(*y))+(*x))) == 'B') //El norte es blanco, entonces la superficie tiene que ser el contrario
                    *(cell_state+1) = 'R';
                else    //El norte es rojo, entonces la superficie tiene que ser el contrario
                    *(cell_state+1) = 'B';
                *(cell_state) = *(*(*(board+(*y))+(*x))+1);

                strcpy(board[*y+1][*x], cell_state);
                strcpy(board[*y+2][*x], cell_state);
                strcpy(board[*y][*x], "  ");
                *x = -1;
                *y = -1;
                free(cell_state);
                return 1;
            }
            else
            {
                free(cell_state);
                return -1;
            }
        case 'O':
            if(*x>max_x-2)
            {
                free(cell_state);
                return -1;
            }
            if( *(*(*(board + (*y)) + (*x+1))) == ' ' &&
                *(*(*(board + (*y)) + (*x+2))) == ' ')
            {
                if(*(*(*(board+(*y))+(*x))+1) == 'B') //El norte es blanco, entonces la superficie tiene que ser el contrario
                    *(cell_state) = 'R';
                else    //El norte es rojo, entonces la superficie tiene que ser el contrario
                    *(cell_state) = 'B';
                *(cell_state + 1) = *(*(*(board+(*y))+(*x))+1);

                strcpy(board[*y][*x+1], cell_state);
                strcpy(board[*y][*x+2], cell_state);
                strcpy(board[*y][*x], "  ");
                *x = -1;
                *y = -1;
                free(cell_state);
                return 1;
            }
            else
            {
                free(cell_state);
                return -1;
            }
    }
}

/* 
Cuenta la cantidad de Dolmens u obtiene el tamano del dolmen mas grande
Recibe: Puntero a la matriz del tablero, posicion en el tablero y tipo de conteo
    ('R' -> cantidad de Dolmens 'D' -> desempate, tamano del dolmen mas grande)
Retorna: vacio
*/
int count_Dolmen(char ***board, int max_x, int max_y, char color, char type_count)
{
    int result = 0;
    int mayor = 0;
    
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
                if(y>0 && *(*(*(board+y-1)+x)) == color)
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

    switch(type_count)
    {
        case 'R':
            //Cuento la cantidad de bloques que hay
            for (y = 1; y < 20; y++)
                if (dolmen_sizes[y][1] > 2)
                    result++;
            return result;
        case 'D':
            for (y = 1; y < 20; y++)
                if (dolmen_sizes[y][1] > mayor)
                    mayor = dolmen_sizes[y][1];
            return mayor;
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
            g_printf("%d ", dolmen_sizes[y][x]);
        g_printf("%s", "\n");
    }

    return result;

}