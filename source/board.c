#include <stdlib.h>
#include <stdio.h>
#include "../include/board.h"

char*** create_board(char ***board, int rows, int columns)
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
/*
    for(i=0; i<rows; i++)
    {
    	for(j=0; j<columns; j++)
    	{
    		board[i][j] = "AB";
    	}
    }*/

    for(i=0; i<rows; i++)
    {
    	for(j=0; j<columns; j++)
    	{
    		printf("%s, %d\n", board[i][j], count++);
    	}
    }

	/*board = malloc(rows * sizeof(char *));
    for(j = 0; j < rows; j++) 
    	board[i] = malloc(3 * columns * sizeof(char)); 
    			// 3 chars because a string has to be terminated by \0

    printf("%s\n", board[0][0]);

    /*for(i=0; i<rows; i++)
    {
    	for(j=0; j<columns; j++)
    	{
    		printf("%s\n", board[i][j]);
    	}
    }*/

	return board;
}

