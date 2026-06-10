#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h> 
#include "game.h"
#include "pieces.h"

void copy_piece(int dest[4][4], int src_index) {
    memcpy(dest, base_shapes[src_index], 16 * sizeof(int));
}

int check_collision(GameState *gs, int grid[4][4], int nx, int ny) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(grid[i][j]) {
                int bx = nx + j;
                int by = ny + i;

                if(bx < 0 || bx >= WIDTH || by >= HEIGHT) return 1; // Ściany i dół
                if(by >= 0 && gs->board[by][bx]) return 1;          // Inne klocki
            }
        }
    }
    return 0;
}

// Obrót w prawo
void rotate_piece(GameState *gs, int direction) {
    int temp[4][4];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if (direction == 1) // W prawo
                temp[j][3 - i] = gs->cgrid[i][j];
            else // W lewo
                temp[3 - j][i] = gs->cgrid[i][j];
        }
    }

    if(!check_collision(gs, temp, gs->cx, gs->cy)) {
        memcpy(gs->cgrid, temp, sizeof(temp));
    }
    else if(!check_collision(gs, temp, gs->cx - 1, gs->cy)) {
        gs->cx--;
        memcpy(gs->cgrid, temp, sizeof(temp));
    }
    else if(!check_collision(gs, temp, gs->cx + 1, gs->cy)) {
        gs->cx++;
        memcpy(gs->cgrid, temp, sizeof(temp));
    }
}



void clear_lines(GameState *gs) {
    int lines_cleared = 0;
    for(int i = HEIGHT - 1; i >= 0; i--) {
        int full = 1;
        for(int j = 0; j < WIDTH; j++) {
            if(!gs->board[i][j]) full = 0;
        }
        if(full) {
            lines_cleared++;
            gs->total_lines++;
            for(int k = i; k > 0; k--) {
                memcpy(gs->board[k], gs->board[k-1], WIDTH * sizeof(int));
            }
            memset(gs->board[0], 0, WIDTH * sizeof(int));
            i++; 
        }
    }
    if(lines_cleared == 1) gs->score += 100;
    else if(lines_cleared == 2) gs->score += 300;
    else if(lines_cleared == 3) gs->score += 500;
    else if(lines_cleared == 4) gs->score += 800;
}

void lock_piece(GameState *gs) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            if(gs->cgrid[i][j] && gs->cy + i >= 0) {
                gs->board[gs->cy + i][gs->cx + j] = 1;
            }
        }
    }
    clear_lines(gs);
    
    gs->current_piece = gs->next_piece;
    gs->next_piece = rand() % 7;
    copy_piece(gs->cgrid, gs->current_piece);
    gs->cx = 3; gs->cy = 0;
    gs->can_hold = 1; 
    
    if(check_collision(gs, gs->cgrid, gs->cx, gs->cy)) {
        system("cls");
        printf("\n\n   KONIEC GRY\n   Wynik: %d\n\n", gs->score);
        Sleep(3000);
        exit(0);
    }
}

void do_hold(GameState *gs) {
    if(!gs->can_hold) return; 
    if(gs->hold_piece == -1) {
        gs->hold_piece = gs->current_piece;
        gs->current_piece = gs->next_piece;
        gs->next_piece = rand() % 7;
    } else {
        int temp = gs->hold_piece;
        gs->hold_piece = gs->current_piece;
        gs->current_piece = temp;
    }
    copy_piece(gs->cgrid, gs->current_piece);
    gs->cx = 3; gs->cy = 0;
    gs->can_hold = 0; 
}