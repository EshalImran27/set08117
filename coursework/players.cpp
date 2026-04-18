#include "globals.h"

// static int offerFreeze(const string& player, int player_symbol) {
//     cout << "\n*** " << player << ", you have earned a FREEZE! ***\n";
//     cout << "You may freeze one of the opponent's columns for their next turn.\n";
//     cout << "Freeze a column? (1 = Yes, 2 = No): ";

//     int choice;
//     cin >> choice;
//     if(cin.fail()){ cin.clear(); cin.ignore(1000, '\n'); return -1; }

//     if(choice != 1) return -1; // player declined

//     cout << "Enter column to freeze (1-7): ";
//     int col;
//     cin >> col;
//     if(cin.fail()){ cin.clear(); cin.ignore(1000, '\n'); return -1; }

//     // Validate range
//     while(col < 1 || col > WIDTH){
//         cout << "Invalid. Enter column to freeze (1-7): ";
//         cin >> col;
//         if(cin.fail()){ cin.clear(); cin.ignore(1000, '\n'); return -1; }
//     }

//     // Warn if column is already full (freezing it is pointless but allowed)
//     if(board_info[0][col-1] != 0)
//         cout << "(Note: that column is already full — freeze will still block placement attempts.)\n";

//     return col - 1; // return 0-based
// }

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

// void player_movement_medium(string player, int player_symbol, int frozenColumn){
//     int playerChoice,move;
//         cout << "\n" << player << ", please select a number from 1 - 7: ";
//         if(frozenColumn != -1){
//             cout << "(Note: Column " << frozenColumn + 1 << " is frozen and cannot be selected this turn) ";
//         }
//         cin >> move;
//         // Error Checking
//         if (cin.fail()){
//             cout << "Error!";
//             exit(1);
//         }
//         while (move > WIDTH || move <= 0 || move-1 == frozenColumn){
//             if(move-1 == frozenColumn){
//                 cout << "\nColumn " << move << " is frozen. Please select another column: ";
//             }else{
//                 cout << "\nPlease select again: ";
//             }
//             cin >> move;
//             if (cin.fail())
//             {
//                 cout << "Error!";
//                 exit(1);
//             }
//         }
//         int number = 0;
//         while (board_info[(HEIGHT - 1) - number][(move - 1)] != 0)
//         {
//             number++;
//             if (number > (HEIGHT - 1)){
//                 cout << "\nPlease select again: ";
//                 cin >> move;
//                 if (cin.fail())
//                 {
//                     cout << "Error!";
//                     exit(1);
//                 }
//                 number = 0; // reset
//             }
//         };
//         board_info[(HEIGHT - 1) - number][move - 1] = player_symbol;
//         LastMoveY = (HEIGHT - 1) - number;
//         LastMoveX = move - 1;
//         Move m = {LastMoveX, LastMoveY};
//         saveGameMoves(getGameNumber(), player_symbol, move); // log using 1-based column
// }

