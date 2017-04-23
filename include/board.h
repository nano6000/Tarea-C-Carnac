char*** create_board(char ***board, int rows, int columns);
void free_board(char ***board, int x, int y);
int set_Stone(char ***board, char *pos, int size);
int count_Dolmen(char ***board, int max_x, int max_y, char color);