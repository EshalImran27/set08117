#include "globals.h"
void enableANSI()
{
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE); // for setting console text color
    DWORD dwMode = 0;
    GetConsoleMode(hOutput, &dwMode);
    SetConsoleMode(hOutput, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING); //
}
// Initialization of each global variable in the main cpp file to avoid multiple definition errors; 
// these variables are declared as extern in the header file to allow them to be accessed across different 
// cpp files that include the header
int LastMoveX=0, LastMoveY=0, gameNumber=1, frozenColumn=-1, pl1Moves=0, pl2Moves=0, threeInARowCountpl1=0, threeInARowCountpl2=0; // initialize frozenColumn to -1 to indicate no column is currently frozen
bool hasFreezepl1=false, goPowerP1=false, hasFreezepl2=false, goPowerP2=false, goPowerusedP1=false, goPowerusedP2=false; // initialize freeze and go power flags to false at the start of the game
string player1, player2, winner, gameMode;
stack <Move> undoStackPlayer1;
stack <Move> redoStackPlayer1;
stack <Move> undoStackPlayer2;
stack <Move> redoStackPlayer2;
int board_info[HEIGHT][WIDTH] = {0}; // initialize the board with 0s, which represent empty spaces
bool timeMode = false;
bool timeUp = false;
volatile long timeLeftSeconds = 0;
HANDLE timerThreadHandle=NULL;

DWORD WINAPI TimerThreadFunction(LPVOID) {
    while (!timeUp && timeLeftSeconds > 0) {
        Sleep(1000); // Sleep for 1 second
        InterlockedDecrement(&timeLeftSeconds); // Atomically decrement the time left
    }
    if (timeLeftSeconds <= 0) {
        timeUp = true; // Set the timeUp flag when time runs out
    }
    return 0;
}

void startTimer(int seconds){
    timeLeftSeconds = seconds;
    timeUp = false;
    if(timerThreadHandle){
        CloseHandle(timerThreadHandle);
    }
    timerThreadHandle = CreateThread(NULL, 0, TimerThreadFunction, NULL, 0, NULL);
}
void stopTimer(){
    timeUp = true;
    if(timerThreadHandle){
        WaitForSingleObject(timerThreadHandle, INFINITE);
        CloseHandle(timerThreadHandle);
        timerThreadHandle = NULL;
    }
}
// a helper function which calls the respective player movement function based on the selected game mode 
//to avoid code duplication in the main game loop for each mode since the logic is mostly the same except 
//for the player movement function called
void helperModes(string mode){
    cleanUnsavedMoves();
    draw_board();
    while(true){
        if(timeUp && timeMode){
            cout << "\nTime's up! It's a draw!\n";
            winner = "Time Up - Draw";
            saveGameInfo(player1, player2, winner);
            stopTimer();
            menu();
            break;
        }
        printTimer();
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
            stopTimer();
            menu();
            break;
        }
        // check if player has earned the GO power-up in hard mode after their move and set the flag 
        //accordingly to allow them to use it in their next turn; we check after the win condition to 
        //allow the player to win immediately with a 3 in a row without having to wait for another turn
        // to use the GO power-up that they earned with that 3 in a row
        if(mode=="Hard" && goPowerEarned(1)){
            goPowerP1 = true; 
        }
        if(timeUp && timeMode){
            cout << "\nTime's up! It's a draw!\n";
            winner = "Time Up - Draw";
            saveGameInfo(player1, player2, winner);
            stopTimer();
            menu();
            break;
        }
        printTimer();
        if(mode == "Easy") player_movement(player2, 2);
        else if(mode == "Medium") player_movement_medium(player2, 2);
        else if(mode == "Hard") player_movement_hard(player2, 2);
        draw_board();
        if (check_for_winner(LastMoveX, LastMoveY, 2)){
            draw_board();
            cout << player2 << " wins! Congratulations!\n";
            winner = player2;
            saveGameInfo(player1, player2, winner);
            stopTimer();
            menu();
            break;
        }
         if(mode=="Hard" && goPowerEarned(2)){
            goPowerP2 = true; 
        }
        for(int i=0;i<WIDTH;i++){
            if(board_info[0][i] == 0) break; // if there's an empty space in the top row, the board is not full yet
            if(i == WIDTH - 1){ // if we reached the end of the loop and all spaces in the top row are filled, it's a draw
                cout << "It's a draw! Well played both!\n";
                winner = "Draw";
                saveGameInfo(player1, player2, winner);
                stopTimer();
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
                    //https://www.geeksforgeeks.org/cpp/string-find-in-cpp/ for find() and npos
                    if(player1.find(',') != string::npos || player1.empty() || player1.find(' ') != string::npos){
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
                cout<<"\n";
                cout<<"play againt timer? (1 = Yes, 2 = No): ";
                int timerChoice;
                cin >> timerChoice;
                if(timerChoice == 1){
                    timeMode=true;
                    cout << "Timer mode enabled for 5 minutes to make all their moves. If time runs out, the game will end in a draw.\n\n";
                    startTimer(300);
                }
                else{
                    timeMode=false;
                }
               switch(mode){
                    case 1:
                        cout<<"Easy mode selected. Good luck!\n";
                        gameMode = "Easy";
                        helperModes("Easy");
                        reset_board(); // reset the board and game state variables for the next game after finishing an easy mode game, since easy mode doesn't have additional state variables that need to be reset compared to medium and hard mode; for medium and hard mode, will rely on the reset_board call at the start of the menu function since they have additional state variables that need to be reset compared to easy mode
                        break;
                    case 2:
                        cout<<"Medium mode selected. Good luck!\n";
                        gameMode = "Medium";
                        helperModes("Medium");
                        reset_board(); // reset the board and game state variables for the next game after finishing a medium mode game, since medium mode has additional state variables that need to be reset compared to easy mode; for hard mode, will rely on the reset_board call at the start of the menu function since hard mode doesn't have additional state variables compared to medium mode
                        break;
                    case 3:
                        cout<<"Hard mode selected. Good luck!\n";
                        gameMode = "Hard";
                        helperModes("Hard");
                        reset_board(); // reset the board and game state variables for the next game after finishing a hard mode game; hard mode doesn't have additional state variables compared to medium mode, so we can rely on the reset_board call at the start of the menu function to reset everything for the next game
                        break;
                    default:
                        cout<<"Invalid choice. Starting in Easy mode by default.\n";
                        gameMode = "Easy";
                        helperModes("Easy");
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
                cout<<"1. EASY 2. MEDIUM 3. HARD\n\\n";

                cout<<"In Easy mode, players take turns dropping their pieces into the columns of the board.\n The first player to connect four pieces in a row (horizontally, vertically, or diagonally) wins the game.\n";
                cout<<"The players are given the choice to either undo or redo their last moves such that they can revert to the intial state of the board at the start of their turn if they want to change their move after seeing the opponent's response, but they cannot undo past the start of their turn or redo past the end of their turn.\n";
                cout<<"\nIn Medium mode, players have the option to use a 'freeze' power-up once they have made 5 moves.\n A freeze allows a player to temporarily block an opponent's column for 1 turn, preventing them from placing pieces in that column.\n";
                cout<<"\nIn Hard mode, The player has a power to change the pieces present on its corners to their color.\nIf a player has 2 three in a rows they will be given this power-up. \n this power can be used once per game and can be used in the turn after it's earned or saved for later, but not in the same turn it's earned since the player has to wait for the next turn to use it after earning it.\n";
                cout<<"\nEnter a choice: \n ";
                cout<<"1. Back to menu\t 2. Exit\n";
                int howtoChoice;
                bool validHowtoChoice = false;
                while(!validHowtoChoice){
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