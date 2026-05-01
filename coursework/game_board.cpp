#include "globals.h"
void draw_board(){
    cout<<"\n";
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
    winningCells.clear();
    frozenColumn = -1; // reset frozen column for next game
    pl1Moves = 0; // reset player 1 move count for next game
    pl2Moves = 0; // reset player 2 move count for next game
    hasFreezepl1 = false; // reset player 1 freeze power for next game
    hasFreezepl2 = false; // reset player 2 freeze power for next game
    goPowerP1 = false; // reset player 1 GO power for next game
    goPowerP2 = false; // reset player 2 GO power for next game
    goPowerusedP1 = false; // reset player 1 GO power usage for next game
    goPowerusedP2 = false; // reset player 2 GO power usage for next game
    undoStackPlayer1 = stack<Move>(); // clear player 1 undo stack for next game
    redoStackPlayer1 = stack<Move>(); // clear player 1 redo stack for next game
    undoStackPlayer2 = stack<Move>(); // clear player 2 undo stack for next game
    redoStackPlayer2 = stack<Move>(); // clear player 2 redo stack for next game
}

void printTimer(){
    if(!timeMode) return; // if not in time mode, don't print timer
    int t=timeLeftSeconds;
    int min = t/60;
    int sec = t%60;
    // print the time left in the format "Time left: mm:ss", with a leading zero for minutes and seconds if they are less than 10
    cout << "\nTime left: " 
        << (min<10?"0":"") << min << "m " << (sec<10?"0":"") << sec << "s\n";
}