#ifndef GAME_H
#define GAME_H

#define WIDTH 10
#define HEIGHT 20

typedef struct {
    int board[HEIGHT][WIDTH];
    int current_piece;
    int next_piece;
    int hold_piece;
    int can_hold;
    int score;
    int total_lines;
    int cx, cy;
    int cgrid[4][4];
} GameState;

void copy_piece(int dest[4][4], int src_index);
int check_collision(GameState *gs, int grid[4][4], int nx, int ny);
void rotate(GameState *gs);
void lock_piece(GameState *gs);
void do_hold(GameState *gs);

#endif 