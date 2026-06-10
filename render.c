#include <stdio.h>
#include <windows.h>
#include "render.h"
#include "pieces.h"

// Funkcja prywatna dla modułu render.c, nie musi być w .h
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hide_cursor() {
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
}

void draw(GameState *gs) {
    gotoxy(0, 0); 
    
    for(int i = 0; i < HEIGHT; i++) {
        printf("|");
        for(int j = 0; j < WIDTH; j++) {
            int is_piece = 0;
            for(int pi = 0; pi < 4; pi++) {
                for(int pj = 0; pj < 4; pj++) {
                    if(gs->cgrid[pi][pj] && gs->cy + pi == i && gs->cx + pj == j) {
                        is_piece = 1;
                    }
                }
            }
            if(is_piece) printf("@");
            else if(gs->board[i][j]) printf("#");
            else printf(".");
        }
        printf("|");
        
        if(i == 2) printf("   WYNIK: %d", gs->score);
        else if(i == 4) printf("   NASTEPNY:");
        else if(i >= 5 && i < 9) {
            printf("   ");
            for(int j = 0; j < 4; j++) {
                if(base_shapes[gs->next_piece][i-5][j]) printf("#");
                else printf(" ");
            }
        }
        else if(i == 10) printf("   SCHOWEK:");
        else if(i >= 11 && i < 15) {
            printf("   ");
            for(int j = 0; j < 4; j++) {
                if(gs->hold_piece != -1 && base_shapes[gs->hold_piece][i-11][j]) printf("#");
                else printf(" ");
            }
        }
        else if(i == 17) printf("   [Strzalki] Ruch/Obrot");
        else if(i == 18) printf("   [Spacja] Zrzut  [C] Schowek");
        else if(i == 19) printf("   [Q] Wyjscie");
        
        printf("          \n");
    }
    for(int j = 0; j < WIDTH + 2; j++) printf("-");
    printf("\n");
}