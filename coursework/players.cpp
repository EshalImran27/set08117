#include "globals.h"

void player_movement(string player, int player_symbol){
    int playerChoice,move;
    cout<<player<<" Select a choice:\n";
    cout<<"1. Place a piece\t 2. Undo\t 3. Redo\n";
    cin >> playerChoice;
    if (cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input. Please try again." << endl;
        player_movement(player, player_symbol);
        return;
    }
    if (playerChoice == 1){
    cout << "\n" << player << ", please select a number from 1 - 7: ";
    cin >> move;
    //Error Checking
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    while (move > WIDTH || move <= 0){
        cout << "\nPlease select again: ";
        cin >> move;
        if (cin.fail()){
            cout << "Error!";
            exit(1);
        }
    }
    int number = 0;
    while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0){
        number++;
        if (number > (HEIGHT - 1)){
            cout << "\nPlease select again: ";
            cin >> move;
            if (cin.fail()){
                cout << "Error!";
                exit(1);
            }
            number = 0;  // reset
        }
    };
    board_info[(HEIGHT - 1) - number][move - 1] = player_symbol;
    LastMoveY = (HEIGHT - 1) - number;
    LastMoveX = move - 1;
    Move m = {LastMoveX, LastMoveY};   
    // save the last move of the player as soon as the move is made for replay purposes
    saveGameMoves(getGameNumber(), player_symbol, move);
    if(player_symbol ==1){
        undoStackPlayer1.push(m);
        redoStackPlayer1 = stack<Move>(); // clear redo stack when a new move is made
    }
    else if(player_symbol ==2){
        undoStackPlayer2.push(m);
        redoStackPlayer2 = stack<Move>(); // clear redo stack when a new move is made
    }
}
    else if (playerChoice == 2){
        cout << "Undoing last move...\n";
        if(player_symbol == 1){
            if(!undoStackPlayer1.empty()){
                Move m = undoStackPlayer1.top();
                undoStackPlayer1.pop();
                board_info[m.row][m.col]=0;
                redoStackPlayer1.push(m);
                saveGameMoves(getGameNumber(), 0 , m.col); // log the undo action with the column of the undone move for replay purposes
                draw_board();
                player_movement(player, player_symbol);
            }
            else{
                cout << "No moves to undo.\n";
                draw_board();
                player_movement(player, player_symbol);
                return;
            }
        }
        else if(player_symbol == 2){
            if(!undoStackPlayer2.empty()){
                Move m = undoStackPlayer2.top();
                undoStackPlayer2.pop();
                board_info[m.row][m.col]=0;
                redoStackPlayer2.push(m);
                saveGameMoves(getGameNumber(), 0 , m.col); // log the undo action with the column of the undone move for replay purposes
                player_movement(player, player_symbol);
            }
            else{
                cout << "No moves to undo.\n";
                player_movement(player, player_symbol);
                return;
            }
        }
        
    }
    else if (playerChoice == 3){
        cout << "Redoing last undone move...\n";
        // redo logic to be implemented
        if(player_symbol == 1){
            if(!redoStackPlayer1.empty()){
                Move m = redoStackPlayer1.top();
                redoStackPlayer1.pop();
                board_info[m.row][m.col]=1;
                undoStackPlayer1.push(m);
                saveGameMoves(getGameNumber(), 1 , m.col); // log the redo action with the column of the redone move for replay purposes
            }
            else{
                cout << "No moves to redo.\n";
                player_movement(player, player_symbol);
                return;
            }
        }
        else if(player_symbol == 2){
            if(!redoStackPlayer2.empty()){
                Move m = redoStackPlayer2.top();
                redoStackPlayer2.pop();
                board_info[m.row][m.col]=2;
                undoStackPlayer2.push(m);
                saveGameMoves(getGameNumber(), 2 , m.col); // log the redo action with the column of the redone move for replay purposes
            }
            else{
                cout << "No moves to redo.\n";
                player_movement(player, player_symbol);
                return;
            }
        }
    }
    else{
        cout << "Invalid choice. Please select again.\n";
        player_movement(player, player_symbol);
        return;
    }
}