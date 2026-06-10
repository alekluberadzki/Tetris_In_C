#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <conio.h>

#include "game.h"
#include "render.h"

int main() {
    srand(time(NULL));
    system("cls"); 
    hide_cursor(); 
    
    GameState state;
    memset(&state, 0, sizeof(GameState));
    
    state.hold_piece = -1;
    state.can_hold = 1;
    state.current_piece = rand() % 7;
    state.next_piece = rand() % 7;
    copy_piece(state.cgrid, state.current_piece);
    state.cx = 3; 
    state.cy = 0;
    state.total_lines = 0;
    
    int timer = 0;

    
    while(1) {
        int delay = 400 - (state.total_lines / 10) * 50;
        if (delay < 50) delay = 50; 
        draw(&state);
        
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 224) { 
                ch = _getch();
                if (ch == 75) { if(!check_collision(&state, state.cgrid, state.cx - 1, state.cy)) state.cx--; } 
                else if (ch == 77) { if(!check_collision(&state, state.cgrid, state.cx + 1, state.cy)) state.cx++; } 
                else if (ch == 80) { if(!check_collision(&state, state.cgrid, state.cx, state.cy + 1)) state.cy++; } 
                else if (ch == 72) { rotate(&state); } 
            }
            else if(ch == 'q' || ch == 'Q') break;
            else if(ch == ' ') {
                while(!check_collision(&state, state.cgrid, state.cx, state.cy + 1)) state.cy++;
                lock_piece(&state);
                timer = 0;
            }
            else if(ch == 'c' || ch == 'C') { do_hold(&state); }
        }
        
        Sleep(50);
        timer += 50;
        
        if(timer >= delay) {
            if(!check_collision(&state, state.cgrid, state.cx, state.cy + 1)) state.cy++;
            else lock_piece(&state);
            timer = 0;
        }
    }
    
    system("cls");
    return 0;
}