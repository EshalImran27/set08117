#include "globals.h"
void draw_board(){
    cout << "| ";
    for (int i = 1; i <= WIDTH; i++){
        if(i==frozenColumn) cout << "(F)" << i << " | "; // indicate the frozen column in medium mode
        else
            cout << " " << i << " | ";
    }
    cout << endl;
    cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |" << endl;
    cout << "-----------------------------"<<endl;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            bool isWinningCell = false;
            for(auto& Cell : winningCells){
                if(Cell.row == i && Cell.col == j){
                    isWinningCell = true;
                    break;
                }
            }
            cout<<"|";
             if (board_info[i][j] == 0) {
                if(j==frozenColumn) cout <<"///";
                else cout << "   ";
            } else if (board_info[i][j] == 1) {
                cout <<(isWinningCell ? WIN : RED) << " X " << RESET;
            } else if (board_info[i][j] == 2) {
                cout <<(isWinningCell ? WIN : YELLOW) << " O " << RESET;
            }
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