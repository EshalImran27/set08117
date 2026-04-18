#include "globals.h"
void player_movement(string player, int player_symbol)
{
        int playerChoice,move;
        cout << player << " Select a choice:\n";
        cout << "1. Place a piece\t 2. Undo\t 3. Redo\n";
        cin >> playerChoice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please try again." << endl;
            //continue;
            player_movement(player, player_symbol); // restart the move selection for the player
        }
        if (playerChoice == 1)
        {
            int move;
            cout << "\n" << player << ", please select a number from 1 - 7: ";
            cin >> move;
            // Error Checking
            if (cin.fail()){
                cout << "Error!";
                exit(1);
            }
            while (move > WIDTH || move <= 0){
                cout << "\nPlease select again: ";
                cin >> move;
                if (cin.fail())
                {
                    cout << "Error!";
                    exit(1);
                }
            }
            int number = 0;
            while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0)
            {
                number++;
                if (number > (HEIGHT - 1)){
                    cout << "\nPlease select again: ";
                    cin >> move;
                    if (cin.fail())
                    {
                        cout << "Error!";
                        exit(1);
                    }
                    number = 0; // reset
                }
            };
            board_info[(HEIGHT - 1) - number][move - 1] = player_symbol;
            LastMoveY = (HEIGHT - 1) - number;
            LastMoveX = move - 1;
            Move m = {LastMoveX, LastMoveY};
            saveGameMoves(getGameNumber(), player_symbol, move, LastMoveY); // log using 1-based column and row
            if(player_symbol == 1){
                undoStackPlayer1.push(m); // push the move onto the undo stack of player 1
                redoStackPlayer1 = stack<Move>(); // clear the redo stack of player 1 since a new move has been made
            }
            else{
                undoStackPlayer2.push(m); // push the move onto the undo stack of player 2
                redoStackPlayer2 = stack<Move>(); // clear the redo stack of player 2 since a new move has been made
            }
        }
        else if (playerChoice == 2)
        {
            if(player_symbol == 1){
                cout << "Undoing last move for " << player1 << "...\n";
                if(!undoStackPlayer1.empty()){
                    Move m = undoStackPlayer1.top();
                    undoStackPlayer1.pop();
                    board_info[m.row][m.col] = 0; // remove the piece from the board
                     // collapse the column after removal
                    redoStackPlayer1.push(m);
                    saveGameMoves(getGameNumber(), player_symbol, m.col+1, m.row, "UNDO"); // log using 1-based column and row
                    draw_board(); // redraw the board after undoing the move
                    player_movement(player, player_symbol); // allow the player to make another choice after undoing
                }
                else{
                    cout << "No moves to undo for " << player1 << ".\n";
                    draw_board(); // redraw the board even if there's no move to undo to maintain consistency in the user interface
                    player_movement(player, player_symbol); // allow the player to make another choice even if
                }
            }
            else{
                cout << "Undoing last move for " << player2 << "...\n";
                if(!undoStackPlayer2.empty()){
                    Move m = undoStackPlayer2.top();
                    undoStackPlayer2.pop();
                    board_info[m.row][m.col] = 0; // remove the piece from the board
                    redoStackPlayer2.push(m);
                    saveGameMoves(getGameNumber(), player_symbol, m.col + 1,m.row, "UNDO"); // log using 1-based column and row
                    draw_board(); // redraw the board after undoing the move
                    player_movement(player, player_symbol); // allow the player to make another choice after undoing
                }
                else{
                    cout << "No moves to undo for " << player2 << ".\n";
                    draw_board(); // redraw the board even if there's no move to undo to maintain consistency in the user interface
                    player_movement(player, player_symbol); // allow the player to make another choice even if
                }
            }
        }
        else if (playerChoice == 3)
        {
            if(player_symbol == 1){
                cout << "Redoing last undone move for " << player1 << "...\n";
                if(!redoStackPlayer1.empty()){
                    Move m = redoStackPlayer1.top();
                    redoStackPlayer1.pop();
                    board_info[m.row][m.col] = player_symbol; // restore the piece on the board
                     // restore the column to its original state before the undo so the piece returns to its saved row.
                    LastMoveX = m.col;
                    LastMoveY = m.row;
                    undoStackPlayer1.push(m);
                    saveGameMoves(getGameNumber(), player_symbol, m.col + 1, LastMoveY); // log using 1-based column
                    draw_board(); // redraw the board after redoing the move
                    player_movement(player, player_symbol); // allow the player to make another choice after redoing
                }
                else{
                    cout << "No moves to redo for " << player1 << ".\n";
                    draw_board(); // redraw the board even if there's no move to redo to maintain consistency in the user interface

                    player_movement(player, player_symbol); // allow the player to make another choice even if there's no move to redo
                }
            }
            else if(player_symbol == 2){
                cout << "Redoing last undone move for " << player2 << "...\n";
                if(!redoStackPlayer2.empty()){
                    Move m = redoStackPlayer2.top();
                    redoStackPlayer2.pop();
                    board_info[m.row][m.col] = player_symbol; // restore the piece on the board
                     // restore the column to its original state before the undo so the piece returns to its saved row.
                    LastMoveX = m.col;
                    LastMoveY = m.row;
                    undoStackPlayer2.push(m);
                    saveGameMoves(getGameNumber(), player_symbol, m.col + 1, LastMoveY); // log using 1-based column
                    draw_board(); // redraw the board after redoing the move
                    player_movement(player, player_symbol); // allow the player to make another choice after redoing
                }
                else{
                    cout << "No moves to redo for " << player2 << ".\n";
                    draw_board(); // redraw the board even if there's no move to redo to maintain consistency in the user interface
                    player_movement(player, player_symbol); // allow the player to make another choice even if there's no move to redo
                }
            }
        }
        else
        {
            cout << "Invalid choice. Please select again.\n";
        }
    }
void player_movement_medium(string player, int player_symbol){
    int move, freezeChoice;
    //cout << "\n" << player << ", please select a number from 1 - 7: ";
    if(frozenColumn != -1){
        bool onlyFrozenColumnAvailable = true;
        for ( int i=0; i<WIDTH; i++){
            if (i == frozenColumn) continue;
            if(board_info[0][i] == 0){ // if there's at least one
                onlyFrozenColumnAvailable = false;
                break;
            }
        }
        if(onlyFrozenColumnAvailable){
            cout << "\n(Note: Column " << frozenColumn + 1 << " is frozen but it's the only available column, so you can select it this turn) ";
            frozenColumn = -1; // reset frozen column since the player is allowed to select it this turn
            return; // skip the rest of the function to allow the player to select the frozen column without being prompted again
        }
    }
        cout << "\n" << player << ", please select a number from 1 - 7: ";
        if(frozenColumn != -1){
            cout << "(Note: Column " << frozenColumn + 1 << " is frozen and cannot be selected this turn) ";
        }
    cin >> move;
    // Error Checking
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    while (move >  WIDTH || move <=0 || move-1 == frozenColumn){
        if(move-1 == frozenColumn){
            cout << "\nColumn " << move << " is frozen. Please select another column: ";
        }else{
            cout << "\nPlease select again: ";
        }
        cin >> move;
        if (cin.fail())
        {
            cout << "Error!";
            exit(1);
        }
    }
    int number = 0;
    while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0)
    {
        number++;
        if (number > (HEIGHT - 1)){
            cout << "\nPlease select again: ";
            cin >> move;
            if (cin.fail())
            {
                cout << "Error!";
                exit(1);
            }
            number = 0; // reset
        }
    };  
    board_info[(HEIGHT - 1) - number][move - 1] = player_symbol;
    LastMoveY = (HEIGHT - 1) - number;
    LastMoveX = move - 1;
    Move m = {LastMoveX, LastMoveY};
    saveGameMoves(getGameNumber(), player_symbol, move, LastMoveY); // log using 1-based column and row
    frozenColumn = -1; // reset frozen column after the move is made
    if(player_symbol == 1){
        pl1Moves++;
    }
    else{
        pl2Moves++;
    }
        if (freezePower(player_symbol)){
        cout << "\n*** " << player << ", you have earned a FREEZE! ***\n";
        cout << "You may freeze one of the opponent's columns for their next turn.\n";
        cout << "Freeze a column? (1 = Yes, 2 = No): ";
        int choice;
        cin >> choice;
        if(cin.fail()){
            cout << "Error!";
            exit(1);
        }
        if(choice == 1){
            cout << "Enter column to freeze (1-7): ";
            int col;
            cin >> col;
            if(cin.fail()){
                cout << "Error!";
                exit(1);
            }
            while(col < 1 || col > WIDTH){
                cout << "Invalid. Enter column to freeze (1-7): ";
                cin >> col;
                if(cin.fail()){
                    cout << "Error!";
                    exit(1);
                }
            }
            frozenColumn = col - 1; // set the global frozenColumn variable
            saveGameMoves(getGameNumber(), player_symbol, col, -1, "FREEZE"); // log using 1-based column; row is not applicable for freeze so set to -1
            cout << "Column " << col << " has been frozen for your opponent's next turn!\n";
            if (player_symbol == 1){
                hasFreezepl1 = false; // reset player 1's freeze power after using it
            }
            else if(player_symbol == 2){
                hasFreezepl2 = false; // reset player 2's freeze power after using it
            }
            draw_board();
        }
    }
}

bool freezePower(int player_symbol){
    if(player_symbol==1){
        if(hasFreezepl1) return true;
        if (pl1Moves > 0 && pl1Moves % 5 == 0){ // if player 1 has made at least one move and is at a multiple of 5 moves, they earn the freeze power
            hasFreezepl1 = true; // set the flag to indicate player 1 has earned the freeze power
            return true;
        }
    }
    else if(player_symbol==2){
        if(hasFreezepl2) return true;
        if (pl2Moves > 0 && pl2Moves % 5 == 0){ // if player 2 has made at least one move and is at a multiple of 5 moves, they earn the freeze power
            hasFreezepl2 = true; // set the flag to indicate player 2 has earned the freeze power
            return true;
        }
    }
    return false;
}

void player_movement_hard(string player, int player_symbol){
    
}
