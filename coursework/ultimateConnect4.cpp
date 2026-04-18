#include "globals.h"
void enableANSI()
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // for setting console text color
    DWORD dwMode = 0;
    GetConsoleMode(hOutput, &dwMode);
    SetConsoleMode(hOutput, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); //
}
//void EasyMode();
//void menu();
int LastMoveX=0, LastMoveY=0, gameNumber=1, frozenColumn=-1, pl1Moves=0, pl2Moves=0 ; // initialize frozenColumn to -1 to indicate no column is currently frozen
bool hasFreezepl1=false, hasFreezepl2=false; // initialize freeze power flags to false at the start of the game
string player1, player2, winner, gameMode;
stack <Move> undoStackPlayer1;
stack <Move> redoStackPlayer1;
stack <Move> undoStackPlayer2;
stack <Move> redoStackPlayer2;
int board_info[HEIGHT][WIDTH] = {0}; // initialize the board with 0s, which represent empty spaces
int main(){
    enableANSI();
    cout<<"Welcome to Ultimate Connect 4!\n";
    cout<<"Let's get started!\n";
    menu();
    return 0;
}
void EasyMode(){
    gameMode = "Easy";
    draw_board();
    while(true){
        // gave a numeric symbol for the player for easier logging and replay; the symbol is not shown to the user
        // might change it afterwards if decide to add more players
        player_movement(player1, 1);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 1)){
            draw_board();
            cout << player1 << " wins! Congratulations!\n";
            winner = player1;
            //save the game information to the log as soon as we know the winner so that even if the program crashes later we have a record of the game and its outcome; also allows replaying even if the program is closed right after the game ends
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
        player_movement(player2, 2);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 2)){
            draw_board();
            cout << player2 << " wins! Congratulations!\n";
            winner = player2;
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
        for(int i=0;i<WIDTH;i++){
            if(board_info[0][i] == 0) break; // if there's an empty space in the top row, the board is not full yet
            if(i == WIDTH - 1){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
                cout << "It's a draw! Well played both!\n";
                winner = "Draw";
                saveGameInfo(player1, player2, winner);
                menu();
                break;
            }
        }

    }
}
void MediumMode(){
    gameMode = "Medium";
    frozenColumn = -1; // initialize frozenColumn to -1 to indicate no column is currently frozen at the start of the game
    draw_board();
    while(true){
        player_movement_medium(player1, 1);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 1)){
            draw_board();
            cout << player1 << " wins! Congratulations!\n";
            winner = player1;
            //save the game information to the log as soon as we know the winner so that even if the program crashes later we have a record of the game and its outcome; also allows replaying even if the program is closed right after the game ends
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
        {
            bool full=true;
            for(int i=0;i<WIDTH;i++){
                if(board_info[0][i] == 0) {
                    full=false; // if there's an empty space in the top row, the board is not full yet
                    break;
                }
            }
            if(full){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
                cout << "It's a draw! Well played both!\n";
                winner = "Draw";
                saveGameInfo(player1, player2, winner);
                menu();
                break;
            }
        }
        player_movement_medium(player2, 2);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 2)){
            draw_board();
            cout << player2 << " wins! Congratulations!\n";
            winner = player2;
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
        {
            bool full=true;
            for(int i=0;i<WIDTH;i++){
                if(board_info[0][i] == 0) {
                    full=false; // if there's an empty space in the top row, the board is not full yet
                    break;
                }
            }
            if(full){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
                cout << "It's a draw! Well played both!\n";
                winner = "Draw";
                saveGameInfo(player1, player2, winner);
                menu();
                break;
            }
        }
    }
}
// void HardMode(){
//     gameMode = "Hard";
//     draw_board();
//    while(true){
//         player_movement_hard(player1, 1);
//         draw_board();
//         if (check_for_winner(LastMoveX, LastMoveY, 1)){
//             draw_board();
//             cout << player1 << " wins! Congratulations!\n";
//             winner = player1;
//             //save the game information to the log as soon as we know the winner so that even if the program crashes later we have a record of the game and its outcome; also allows replaying even if the program is closed right after the game ends
//             saveGameInfo(player1, player2, winner);
//             menu();
//             break;
//         }
//         {
//             bool full=true;
//             for(int i=0;i<WIDTH;i++){
//                 if(board_info[0][i] == 0) {
//                     full=false; // if there's an empty space in the top row, the board is not full yet
//                     break;
//                 }
//             }
//             if(full){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
//                 cout << "It's a draw! Well played both!\n";
//                 winner = "Draw";
//                 saveGameInfo(player1, player2, winner);
//                 menu();
//                 break;
//             }
//         }
//         player_movement_hard(player2, 2);
//         draw_board();
//         if (check_for_winner(LastMoveX, LastMoveY, 2)){
//             draw_board();
//             cout << player2 << " wins! Congratulations!\n";
//             winner = player2;
//             saveGameInfo(player1, player2, winner);
//             menu();
//             break;
//         }
//         {
//             bool full=true;
//             for(int i=0;i<WIDTH;i++){
//                 if(board_info[0][i] == 0) {
//                     full=false; // if there's an empty space in the top row, the board is not full yet
//                     break;
//                 }
//             }
//             if(full){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
//                 cout << "It's a draw! Well played both!\n";
//                 winner = "Draw";
//                 saveGameInfo(player1, player2, winner);
//                 menu();
//                 break;
//             }
//         }
//     }
// }
void menu(){
    // reset the board and game state variables in case coming back from a finished game or replay
    reset_board();
    cout<<"Please select an option:\n";
    cout<<"1. Start Game\t 2. Replay a game\n";
    cout<<"3. See the scoreboard \t4. How to play?\n";
    cout<<"5. Exit\n";
    cout<<"Enter your choice: ";
    int choice;
    cin >> choice;
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    switch (choice){
        case 1:{
            cout<<"GAME STARTED...\n";
            cout<<"Player 1 enter your name: ";
            cin >> player1;
                if (cin.fail()){
                    cout << "Error!";
                    exit(1);
                }
            //https://www.geeksforgeeks.org/cpp/string-find-in-cpp/ for find() and npos
            while(player1.find(',') != string::npos || player1.empty() || player1.find(" ") != string::npos){
                cout << "Error: Name cannot contain commas(,) a space or be empty. Please enter again: ";
                cin >> player1;
                if (cin.fail()){
                    cout << "Error!";
                    exit(1);
                }
            }
            cout<<"Player 2 enter your name: ";
            cin >> player2;
                if (cin.fail()){
                    cout << "Error!";
                    exit(1);
                }
            while(player2.find(',') != string::npos || player2.empty() || player2.find(" ") != string::npos){
                cout << "Error: Name cannot contain commas(,) a space or be empty. Please enter again: ";
                cin >> player2;
                if (cin.fail()){
                    cout << "Error!";
                    exit(1);
                }
            }
            cout<<"Welcome, "<<player1<<" and "<<player2<<"!\n";
            cout<<player1<<" will be X and "<<player2<<" will be O.\n";
            cout<<"Please choose a game mode:\n";
            cout<<"1. Easy\t 2. Medium\t 3. Hard\n";
            cout<<"Enter your choice: ";
            int mode;
            cin >> mode;
            if (cin.fail()){
                cout << "Error!";
                exit(1);
            }
            switch(mode){
                case 1:
                    cout<<"Easy mode selected. Good luck!\n";
                    EasyMode();
                    winningCells.clear();
                    break;
                case 2:
                    cout<<"Medium mode selected. Good luck!\n";
                    MediumMode();
                    winningCells.clear();
                    frozenColumn = -1; // reset frozen column for next game
                    pl1Moves = 0; // reset player 1 move count for next game
                    pl2Moves = 0; // reset player 2 move count for next game
                    hasFreezepl1 = false; // reset player 1 freeze power for next game
                    hasFreezepl2 = false; // reset player 2 freeze power for next game
                    break;
                case 3:
                    cout<<"Hard mode selected. Good luck!\n";
                    //HardMode();
                    break;
                default:
                    cout<<"Invalid choice. Starting in Easy mode by default.\n";
            }
            break;
        }
        case 2:{
            cout<<"Replaying a game...\n";
            cout<<"Enter game number to replay: ";
            int gameNum;
            cin >> gameNum;
            gameReplay(gameNum);
            break;
        }
        case 3:{
            cout<<"Showing scoreboard...\n";
                scoreboard();
            break;
        }
        case 4:{
            cout<<"How to play:\n";
            cout<<"1. Easy 2. Medium 3. Hard\n";
            cout<<"In Easy mode, players take turns dropping their pieces into the columns of the board. The first player to connect four pieces in a row (horizontally, vertically, or diagonally) wins the game.\n";
            cout<<"In Medium mode, players have the option to use a 'freeze' power-up once they have made 5 moves. A freeze allows a player to temporarily block an opponent's column for 1 turn, preventing them from placing pieces in that column.\n";
            cout<<"In Hard mode, There will be a chain of bomb reactions such that a bomb will be placed randomly on the board. If a player places a piece adjacent to the bomb, it will trigger an explosion that clears all pieces in the surrounding 3x3 area, including the bomb itself.\n";
            cout<<"Enter a choice: ";
            cout<<"1. Back to menu\t 2. Exit\n";
            int howtoChoice;
            cin >> howtoChoice;
            if (cin.fail()){
                cout << "Error!";
                exit(1);
            }
            switch(howtoChoice){
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
            break;
        }
        case 5:{
            cout<<"Exiting...\n";
            exit(0);
        }
        default:
            cout<<"Invalid choice. Please select again.\n";
    }

}