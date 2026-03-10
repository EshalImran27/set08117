#include "globals.h"
void draw_board(){
    cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |" << endl;
    cout << "-----------------------------"<<endl;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            cout<<"|";
             if (board_info[i][j] == 0) cout << "   ";
            else if (board_info[i][j] == 1) cout << " X ";
            else if (board_info[i][j] == 2) cout << " O ";
        }
        cout << "|\n-----------------------------" << endl;
    }
}

void reset_board(){
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            board_info[i][j] = 0;
        }
    }
    LastMoveX = -1;
    LastMoveY = -1;
}