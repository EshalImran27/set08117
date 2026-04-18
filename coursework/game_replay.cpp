#include "globals.h"

void gameReplay(int gameNumber){
    // AT FIRST I THOUGHT TO USE LINKED LIST TO STORE THE MOVES OF THE GAME TO MAKE REPLAYING EASIER BUT THEN DECIDED TO JUST READ FROM THE FILE DIRECTLY FOR SIMPLICITY SINCE THE FILE IS NOT EXPECTED TO BE VERY LARGE AND THIS WAY WE ALSO don't have to worry about memory management for the linked list
    ifstream infile("game_moves.csv");
    if (!infile.is_open()){
        cout << "Error: Could not open game_moves.csv for reading." << endl;
        return;
    }
    string line;
    cout << "Replaying game " << gameNumber << "...\n";
    // clear board once before replay
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            board_info[i][j] = 0;
        }
    }
    system("cls");
    winningCells.clear(); // clear winning cells in case replaying a game right after another one where there was a win, to avoid incorrectly highlighting pieces from the previous game
    draw_board();
    Sleep(500);
// read each line in the file and when the game number matches the one we want to replay, make the move on the board and show the updated board;
// if game number doesn't match, skip the line
    while(getline(infile, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string gameNumStr, symbolStr, columnStr, actionStr, rowStr;
        int idx = 0;
        getline(ss, gameNumStr, ',');
        getline(ss, symbolStr, ',');
        getline(ss, columnStr, ',');
        getline(ss, rowStr, ',');
        getline(ss, actionStr, ',');
        
        int gameNum = stoi(gameNumStr);
        int symbol = stoi(symbolStr);
        //string action = actionStr;
        int col = stoi(columnStr);
        int row = stoi(rowStr);
        actionStr.erase(actionStr.find_last_not_of(" \n\r\t")+1); // trim any whitespace from the action string
        if(gameNum != gameNumber) continue; // skip moves from other games
        if(actionStr == "UNDO"){
           board_info[row][col-1] = 0; // remove the piece from the board; log is saved using 1-based column and row so convert to 0-based for indexing the board
        }else if(actionStr == "FREEZE"){
            frozenColumn = col - 1; // set the global frozenColumn variable; log is saved using 1-based column so convert to 0-based for indexing

        }else {
        //if(action == "MOVE"){
            int num = 0;
            // a simple gravity fall check
            while(num <= HEIGHT - 1 && board_info[(HEIGHT - 1) - num][(col - 1)] != 0){
                num++;
            }
            if(num <= HEIGHT - 1){
                board_info[(HEIGHT - 1) - num][(col - 1)] = symbol;
            }
            frozenColumn = -1; // reset frozen column after the move is made
        }
        system("cls");
        draw_board();
        Sleep(500);
    }
    infile.close();
    cout << "Replay finished.\n";
    cout<<"Enter a choice:\n";
    cout<<"1. Back to menu\t 2. Exit\n";
    int scoreChoice;
    cin >> scoreChoice;
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    switch(scoreChoice){
        case 1:
            menu();
            break;
        case 2:
            cout<<"Exiting...\n";
            exit(0);
        default:
            cout<<"Invalid choice. Returning to menu.\n";
            menu();
    }
}