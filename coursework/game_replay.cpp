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
    draw_board();
    Sleep(500);
// read each line in the file and when the game number matches the one we want to replay, make the move on the board and show the updated board;
// if game number doesn't match, skip the line
    while(getline(infile, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string gameNumStr, symbolStr, columnStr;
        int idx = 0;
        getline(ss, gameNumStr, ',');
        getline(ss, symbolStr, ',');
        getline(ss, columnStr, ',');
        int gameNum = stoi(gameNumStr);
        int symbol = stoi(symbolStr);
        int col = stoi(columnStr);
        if(gameNum != gameNumber) continue; // skip moves from other games
        int num = 0;
        // a simple gravity fall check
        while(num <= HEIGHT - 1 && board_info[(HEIGHT - 1) - num][(col - 1)] != 0){
            num++;
        }
        if(num <= HEIGHT - 1){
            board_info[(HEIGHT - 1) - num][(col - 1)] = symbol;
            system("cls");
            draw_board();
            Sleep(500);
    }
}
    infile.close();
    cout << "Replay finished.\n";
}