#include "globals.h"
void player_movement(string player, int player_symbol){
    int playerChoice,move;
    bool validPlayerChoice = false;
    while(!validPlayerChoice){
        cout << player << " Select a choice:\n";
        cout << "1. Place a piece\t 2. Undo\t 3. Redo\n";
        cin >> playerChoice;
        if (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input. Please try again." << endl;
            continue;
        }
        if(playerChoice >= 1 && playerChoice <= 3) validPlayerChoice = true;
        else cout << "Invalid choice. Please select again: ";
    }
    switch(playerChoice){
        case 1:{
            int move;
            bool validMove = false;
            while(!validMove){
                cout << "\n" << player << ", please select a number from 1 - 7: ";
                cin >> move;
            // Error Checking
                if (cin.fail()){
                    cout << "Error!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                if(move >= 1 && move <= WIDTH) validMove = true;
                else cout << "Invalid choice. Please select again: ";
            }
            int number = 0;
            while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0){
                number++;
                if (number > (HEIGHT - 1)){
                    cout << "\nPlease select again: ";
                    cin >> move;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    number = 0; // reset
                }
            }
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
            break;
        }
        case 2:{
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
            break;
        }
        case 3:{
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
            break;
        }
        default:{
            cout << "Invalid choice. Please select again: ";
            player_movement(player, player_symbol); // allow the player to make another choice if they somehow got past the initial validation (shouldn't happen but added as a safeguard)
            break;
        }
    }
}
void player_movement_medium(string player, int player_symbol){
    int move, freezeChoice;
    //cout << "\n" << player << ", please select a number from 1 - 7: ";
    if(frozenColumn != -1){
        bool onlyFrozenColumnAvailable = true;
        for ( int i=0; i<WIDTH; i++){
            if (i == frozenColumn) continue;
            if(board_info[0][i] == 0){ // if there's at least one empty space in the top row
                onlyFrozenColumnAvailable = false;
                break;
            }
        }
        if(onlyFrozenColumnAvailable){
            cout << "\n(Note: Column " << frozenColumn + 1 << " is frozen but it's the only available column, so you cant select it this turn) ";
            frozenColumn = -1; 
            return; 
        }
    }
    bool validMove = false;
    while(!validMove){
        cout << "\n" << player << ", please select a number from 1 - 7: ";
        if(frozenColumn != -1){
            cout << "\n(Note: Column " << frozenColumn + 1 << " is frozen and cannot be selected this turn) ";
        }
        cin >> move;
        if (cin.fail()){
            cout << "Error!";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        if(move-1 == frozenColumn){
            cout << "\nColumn " << move << " is frozen. Please select another column: ";
            continue;
        }

        if(move >= 1 && move <= WIDTH) validMove = true;
        else cout << "Invalid choice. Please select again: ";
    }
    int number = 0;
    while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0){
        number++;
        if (number > (HEIGHT - 1)){
            cout << "\nPlease select again: ";
            cin >> move;
            if (cin.fail()){
                cout << "Error!";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
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
    draw_board();
    if (freezePower(player_symbol)){
        cout << "\n*** " << player << ", you have earned a FREEZE! ***\n";
        cout << "You may freeze one of the opponent's columns for their next turn.\n";
        int choice;
        bool validChoice = false;
        while(!validChoice){
            cout << "Freeze a column? (1 = Yes, 2 = No):";
            cin >> choice;
            if (cin.fail()){
                cout << "Error!";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            if(choice == 1 || choice == 2) validChoice = true;
            else cout << "Invalid choice. Please select again: ";
        }
        if(choice == 1){
            cout << "Enter column to freeze (1-7): ";
            int col;
            bool validCol = false;
            while(!validCol){
                cin >> col;
                if(cin.fail()){
                    cout << "Error!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                if(col >= 1 && col <= WIDTH) validCol = true;
                else cout << "Invalid. Enter column to freeze (1-7): ";
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
        }
        else{
            cout << "You chose not to use your freeze power-up.\n";
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
int countThreeInARow(int player_symbol){
    int count=0;
    //horizontal check
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<=WIDTH-3;j++){
            if(board_info[i][j] == player_symbol && 
                board_info[i][j+1] == player_symbol && 
                board_info[i][j+2] == player_symbol){
                count++;
            }
        }
    }
    //vertical check
    for(int i=0;i<WIDTH;i++){
        for(int j=0;j<=HEIGHT-3;j++){
            if(board_info[j][i] == player_symbol && 
                board_info[j+1][i] == player_symbol && 
                board_info[j+2][i] == player_symbol){
                count++;
            }
        }
    }
    //diagonal check (bottom left to top right)
    for(int i=2;i<HEIGHT;i++){
        for(int j=0;j<=WIDTH-3;j++){
            if(board_info[i][j] == player_symbol && 
                board_info[i-1][j+1] == player_symbol && 
                board_info[i-2][j+2] == player_symbol){
                count++;
            }
        }
    }
    //diagonal check (top left to bottom right)
    for(int i=0;i<=HEIGHT-3;i++){
        for(int j=0;j<=WIDTH-3;j++){
            if(board_info[i][j] == player_symbol && 
                board_info[i+1][j+1] == player_symbol && 
                board_info[i+2][j+2] == player_symbol){
                count++;
            }
        }
    }
    return count;
}
bool goPowerEarned(int player_symbol){
    return countThreeInARow(player_symbol) >= 2; // if the player has at least one instance of 3 in a row, they earn the GO power-up
}

void player_movement_hard(string player, int player_symbol){
    bool usedGoPower = false;
    bool &hasGo = (player_symbol == 1) ? goPowerP1 : goPowerP2; // check if the current player has earned the GO power-up
    bool &usedGo = (player_symbol == 1) ? goPowerusedP1 : goPowerusedP2; // check if the current player has already used the GO power-up
    if (hasGo && !usedGo){
        cout << "\n*** " << player << ", you have earned a GO power-up by having a 3 in a row blocked by your opponent! ***\n";
        cout << "You may place a GO piece on the board to convert an opponent's piece and its adjacent pieces to your color.\n";
        cout << "Place a GO piece? (1 = Yes, 2 = No): ";
        bool validChoice = false;
        int choice;
        while(!validChoice){
            cin >> choice;
            if(cin.fail()){
                cout << "Error!";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }
            switch(choice){
                case 1:{
                    usedGoPower = true; // set the flag to indicate the GO power-up has been used
                    cout <<" Enter column to place GO piece (1-7): ";
                    int col;
                    bool validCol = false;
                    while(!validCol){
                        cin >> col;
                        if(cin.fail()){
                            cout << "Error!";
                            cin.clear();
                            cin.ignore(1000, '\n');
                            continue;
                        }
                        if(col >= 1 && col <= WIDTH) validCol = true;
                        else cout << "Invalid. Enter column to place GO piece (1-7): ";
                    }
                    int number = 0;
                    while (board_info[(HEIGHT - 1) - number][(col - 1)] != 0){
                        number++;
                        if (number > (HEIGHT - 1)){
                            cout << "\nColumn is full. Enter column to place GO piece (1-7): ";
                            cin >> col;
                            if (cin.fail()){
                            cout << "Error!";
                            cin.clear();
                            cin.ignore(1000, '\n');
                            continue;
                            }
                            number = 0; // reset
                        }
                    }
                    int row = (HEIGHT - 1) - number;
                    board_info[row][col - 1] = player_symbol; // place the GO piece on the board
                    saveGameMoves(getGameNumber(), player_symbol, col, row, "GO"); // log using 1-based column and row
                    // convert adjacent pieces of the opponent to the current player's symbol
                    for(int i = row - 1; i <= row + 1; i+=2){
                        for(int j = col - 2; j <= col; j+=2){
                            if(i >= 0 && i < HEIGHT && j >= 0 && j < WIDTH){ // check bounds
                                if(board_info[i][j] != 0 && board_info[i][j] != player_symbol){ // if there's an opponent's piece
                                board_info[i][j] = player_symbol; // convert it to the current player's symbol
                                }
                            }
                        }
                    }
                    if (player_symbol == 1){
                        threeInARowCountpl1 = 0; // reset player 1's 3 in a row count after using the GO power-up
                    }
                    else if(player_symbol == 2){
                        threeInARowCountpl2 = 0; // reset player 2's 3 in a row count after using the GO power-up
                    }
                    hasGo = false; // reset the flag to indicate the GO power-up has been used
                    usedGo = true; // set the flag to indicate the GO power-up has been used
                    cout << "GO piece placed at column " << col << " for player " << player << " and adjacent opponent pieces have been converted!\n";
                    draw_board();
                    bool winFound = false;
                    for(int r=0; r<HEIGHT; r++){
                        for(int c=0; c<WIDTH; c++){
                            if(board_info[r][c] == player_symbol){
                                if (check_for_winner(c, r, player_symbol)){
                                    winFound = true;
                                    LastMoveX = c;
                                    LastMoveY = r;
                                    break;
                                }
                            }    
                        }
                    }
                    if(winFound){
                        draw_board();
                        cout << player << " wins! Congratulations!\n";
                        winner = player;
                        saveGameInfo(player1, player2, winner);
                        menu();
                    }
                    
                    validChoice = true;
                    break;
                }
                case 2:{
                    cout << "You chose not to use your GO power-up.\n";
                    validChoice = true;
                    // allow the player to proceed with their normal move without using the GO power-up
                    break;
                }
                default:
                    cout << "Invalid choice. Place a GO piece? (1 = Yes, 2 = No): ";
                    break;
            }
        }
    }
    if (!usedGoPower){ // if the player has not used their GO power-up, proceed with normal move selection
        cout << player << " enter a number from 1 - 7: ";
        int move;
        bool validMove = false;
        while(!validMove){
            cin >> move;
            // Error Checking
            if (cin.fail()){
                cout << "Error!";
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
                }
            if(move >= 1 && move <= WIDTH) validMove = true;
            else cout << "\nPlease select again: ";
        }
        int number = 0;
        while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0){
            number++;
            if (number > (HEIGHT - 1)){
                cout << "\nPlease select again: ";
                cin >> move;
                if (cin.fail()){
                    cout << "Error!";
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }
                number = 0; // reset
            }
        }
        board_info[(HEIGHT - 1) - number][move - 1] = player_symbol;
        LastMoveY = (HEIGHT - 1) - number;
        LastMoveX = move - 1;
        Move m = {LastMoveX, LastMoveY};
        bool earnedGoPowerpl1 = false, earnedGoPowerpl2 = false;
        if (player_symbol ==1) {
            earnedGoPowerpl1 = goPowerEarned(player_symbol);
        }
        else if(player_symbol == 2){
            earnedGoPowerpl2 = goPowerEarned(player_symbol);
        }
        saveGameMoves(getGameNumber(), player_symbol, move, LastMoveY); // log using 1-based column and row           
    }
}
