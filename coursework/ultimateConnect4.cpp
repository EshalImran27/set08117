#include "globals.h"
void enableANSI()
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // for setting console text color
    DWORD dwMode = 0;
    GetConsoleMode(hOutput, &dwMode);
    SetConsoleMode(hOutput, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); //
}
int LastMoveX=0, LastMoveY=0, gameNumber=1, frozenColumn=-1, pl1Moves=0, pl2Moves=0, threeInARowCountpl1=0, threeInARowCountpl2=0; // initialize frozenColumn to -1 to indicate no column is currently frozen
bool hasFreezepl1=false, goPowerP1=false, hasFreezepl2=false, goPowerP2=false, goPowerusedP1=false, goPowerusedP2=false; // initialize freeze and go power flags to false at the start of the game
string player1, player2, winner, gameMode;
stack <Move> undoStackPlayer1;
stack <Move> redoStackPlayer1;
stack <Move> undoStackPlayer2;
stack <Move> redoStackPlayer2;
int board_info[HEIGHT][WIDTH] = {0}; // initialize the board with 0s, which represent empty spaces

void helperModes(string mode){
    cleanUnsavedMoves();
    draw_board();
    while(true){
        if(mode == "Easy") player_movement(player1, 1);
        else if(mode == "Medium") player_movement_medium(player1, 1);
        else if(mode == "Hard") player_movement_hard(player1, 1);
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
        if(mode=="Hard" && goPowerEarned(1)){
            goPowerP1 = true; 
            // cout << "\n*** " << player1 << ", you have earned a GO POWER-UP! ***\n";
            // cout << "You may place a GO piece on your next turn to convert one of your opponent's pieces into your own and earn an additional turn.\n";
        }
        if(mode == "Easy") player_movement(player2, 2);
        else if(mode == "Medium") player_movement_medium(player2, 2);
        else if(mode == "Hard") player_movement_hard(player2, 2);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 2)){
            draw_board();
            cout << player2 << " wins! Congratulations!\n";
            winner = player2;
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
         if(mode=="Hard" && goPowerEarned(2)){
            goPowerP2 = true; 
            // cout << "\n*** " << player2 << ", you have earned a GO POWER-UP! ***\n";
            // cout << "You may place a GO piece on your next turn to convert one of your opponent's pieces into your own and earn an additional turn.\n";
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

// GAME STARTS HERE
int main(){
    enableANSI();
    cout<<"Welcome to Ultimate Connect 4!\n";
    cout<<"Let's get started!\n";
    menu();
    return 0;
}
void EasyMode(){
    gameMode = "Easy";
    helperModes("Easy");
}
void MediumMode(){
    gameMode = "Medium";
    frozenColumn = -1; // initialize frozenColumn to -1 to indicate no column is currently frozen at the start of the game
    helperModes("Medium");
}
void HardMode(){
    gameMode = "Hard";
    helperModes("Hard");
}
void menu(){
    // reset the board and game state variables in case coming back from a finished game or replay
    reset_board();
    bool validoptionMenu = false;
    cout<<"Please select an option:\n";
    cout<<"1. Start Game\t 2. Replay a game\n";
    cout<<"3. See the scoreboard \t4. How to play?\n";
    cout<<"5. Exit\n";
    while(!validoptionMenu){
        bool validchoiceMenu = false;
        int choice;
        while(!validchoiceMenu){
            cout<<"Enter your choice: ";
            cin >> choice;
            if (cin.fail()){
                cout << "Error!";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            if(choice >= 1 && choice <= 5) validchoiceMenu = true;
            else cout << "Invalid choice. Please select again.\n";
        }
        switch (choice){
            case 1:{
                bool validName1 = false, validName2 = false;
                validoptionMenu = true;
                cout<<"GAME STARTED...\n";
                while(!validName1){
                    cout<<"Player 1 enter your name: ";
                    cin >> player1;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    if(player1.find(',') != string::npos || player1.empty() || player1.find(' ') != string::npos){//https://www.geeksforgeeks.org/cpp/string-find-in-cpp/ for find() and npos
                        cout << "Error: Name cannot contain commas(,) a space or be empty. Please enter again.\n";
                    }
                    else validName1 = true;
                }
                while(!validName2){
                    cout<<"Player 2 enter your name: ";
                    cin >> player2;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    if(player2.find(',') != string::npos || player2.empty() || player2.find(' ') != string::npos){
                        cout << "Error: Name cannot contain commas(,) a space or be empty. Please enter again.\n";
                    }
                    else validName2 = true;
                }
                cout<<"Welcome, "<<player1<<" and "<<player2<<"!\n";
                cout<<player1<<" will be X and "<<player2<<" will be O.\n";
                cout<<"\n";
                cout<<"Please choose a game mode:\n";
                cout<<"1. Easy\t 2. Medium\t 3. Hard\n";
                int mode;
                bool validMode = false;
                while(!validMode){
                    cout<<"Enter your choice: ";
                    cin >> mode;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid choice. Please select again: ";
                    }
                    else if(mode >= 1 && mode <= 3) validMode = true;
                    else cout << "Invalid choice. Please select again: ";
                }
                switch(mode){
                    case 1:
                        cout<<"Easy mode selected. Good luck!\n";
                        EasyMode();
                        reset_board(); // reset the board and game state variables for the next game after finishing an easy mode game, since easy mode doesn't have additional state variables that need to be reset compared to medium and hard mode; for medium and hard mode, will rely on the reset_board call at the start of the menu function since they have additional state variables that need to be reset compared to easy mode
                        break;
                    case 2:
                        cout<<"Medium mode selected. Good luck!\n";
                        MediumMode();
                        reset_board(); // reset the board and game state variables for the next game after finishing a medium mode game, since medium mode has additional state variables that need to be reset compared to easy mode; for hard mode, will rely on the reset_board call at the start of the menu function since hard mode doesn't have additional state variables compared to medium mode
                        break;
                    case 3:
                        cout<<"Hard mode selected. Good luck!\n";
                        HardMode();
                        reset_board(); // reset the board and game state variables for the next game after finishing a hard mode game; hard mode doesn't have additional state variables compared to medium mode, so we can rely on the reset_board call at the start of the menu function to reset everything for the next game
                        break;
                    default:
                        cout<<"Invalid choice. Starting in Easy mode by default.\n";
                        EasyMode();
                        reset_board();
                }
                break;
            }
        //case 1 ends here
            case 2:{
                validoptionMenu = true;
                cout<<"Replaying a game...\n";
                bool validGameNum = false;
                int gameNum;
                while(!validGameNum){
                    cout<<"Enter game number to replay: ";
                    cin >> gameNum;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        continue;
                    }
                    if(gameNum <= 0){
                        cout << "Invalid game number. Please enter a positive integer.\n";
                    }
                    else validGameNum = true;
                }                    
                gameReplay(gameNum);
                break;
            }
            case 3:{
                validoptionMenu = true;
                cout<<"Showing scoreboard...\n";
                scoreboard();
                break;
            }
            case 4:{
                validoptionMenu = true;
                cout<<"How to play:\n";
                cout<<"1. Easy 2. Medium 3. Hard\n";
                cout<<"In Easy mode, players take turns dropping their pieces into the columns of the board.\n The first player to connect four pieces in a row (horizontally, vertically, or diagonally) wins the game.\n";
                cout<<"\nIn Medium mode, players have the option to use a 'freeze' power-up once they have made 5 moves.\n A freeze allows a player to temporarily block an opponent's column for 1 turn, preventing them from placing pieces in that column.\n";
                cout<<"\nIn Hard mode, The player has a power to chnage the pieces present on its corners to their color.\nIf a player has 2 three in a rows they will be given this power-up. \n";
                cout<<"Enter a choice: \n ";
                cout<<"1. Back to menu\t 2. Exit\n";
                int howtoChoice;
                bool validHowtoChoice = false;
                while(!validHowtoChoice){
                    cout<<"Enter your choice: ";
                    cin >> howtoChoice;
                    if (cin.fail()){
                        cout << "Error!";
                        cin.clear();
                        cin.ignore(1000, '\n');
                        cout << "Invalid choice. Please select again: ";
                    }
                    else if(howtoChoice == 1 || howtoChoice == 2) validHowtoChoice = true;
                    else cout << "Invalid choice. Please select again: ";
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
                validoptionMenu = true;
                cout<<"Exiting...\n";
                exit(0);
            }
            default:
                cout<<"Invalid choice. Please select again.\n";
                break;
        }
    }
}




// check for right go power goes to right person
// check for how go power is earned