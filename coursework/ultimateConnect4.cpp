#include <fstream> // for file handling
#include <iostream> // for input/output
#include <stdlib.h> //needed for rand
#include <string> // for string handling
#include <ctime> // for time-based seeding
#include <windows.h> // for Sleep
#include <sstream> // for stringstream
using namespace std; 
#define HEIGHT 6 
#define WIDTH 7
int board_info[HEIGHT][WIDTH] = { {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0} };
int LastMoveX, LastMoveY, gameNumber;
string player1, player2, winner, gameMode;
void EasyMode();
void menu();
void reset_board();
void draw_board();
bool check_horizontal_combo(int x, int y, int player_symbol);
bool check_vertical_combo(int x, int y, int player_symbol);
bool check_diagonal_combo_SW_NE(int x, int y, int player_symbol);
bool check_diagonal_combo_NW_SE(int x, int y, int player_symbol);
bool check_for_winner(int x, int y, int player_symbol);
void player_movement(string player, int player_symbol);
void scoreboard();
int getGameNumber();
bool saveGameInfo(string player1, string player2, string winner);
void deleteRecord();
void gameReplay(int game_Number);
bool saveGameMoves(int game_Number, int player_symbol, int column);


int main(){
    cout<<"Welcome to Ultimate Connect 4!\n";
    cout<<"Let's get started!\n";
    menu();
    return 0;
}


void draw_board(){
    cout << "| 1 | 2 | 3 | 4 | 5 | 6 | 7 |" << endl;
    cout << "-----------------------------"<<endl;
    for(int i=0;i<HEIGHT;i++){
        for(int j=0;j<WIDTH;j++){
            cout<<"|";
             if (board_info[i][j] == 0) cout << "   ";
            else if (board_info[i][j] == 1) cout << " X ";
            else if (board_info[i][j] == 2) cout << " O ";
        }
        cout << "|\n-----------------------------" << endl;
    }
}
void player_movement(string player, int player_symbol){
    int choice;
    cout << "\n" << player << ", please select a number from 1 - 7: ";
    cin >> choice;
    //Error Checking
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    while (choice > WIDTH || choice <= 0){
        cout << "\nPlease select again: ";
        cin >> choice;
        if (cin.fail()){
            cout << "Error!";
            exit(1);
        }
    }
    int number = 0;
    while (board_info[(HEIGHT - 1) - number][(choice - 1)] != 0){
        number++;
        if (number > (HEIGHT - 1)){
            cout << "\nPlease select again: ";
            cin >> choice;
            if (cin.fail()){
                cout << "Error!";
                exit(1);
            }
            number = 0;  // reset
        }
    };
    board_info[(HEIGHT - 1) - number][choice - 1] = player_symbol;
    LastMoveY = (HEIGHT - 1) - number;
    LastMoveX = choice - 1;
    // save the last move of the player as soon as the move is made for replay purposes
    saveGameMoves(getGameNumber(), player_symbol, choice);
}
bool saveGameMoves(int gameNumber, int player_symbol, int column){
    string movesFilename = "game_moves.csv";
    ofstream outfile("game_moves.csv", ios::app);
     if(!outfile.is_open()){
            cout << "Error: Could not open game_moves.csv for writing." << endl;
            return false;
    }
    //might add some other things to log later like undo redo moves.
    outfile << gameNumber << "," << player_symbol << "," << column << "\n";
    outfile.close();
    return true;
}
int getGameNumber(){
    // the game number corresponds to the number of lines in the game_log.csv file (excluding header) + 1
    ifstream infile("game_log.csv");
    int gameNum = 1;
    if(infile){
        string line;
        int lineCount=0;
        getline(infile, line); // skip header
        while(getline(infile, line)){
            if(!line.empty()){
                lineCount++;
            }
        }
        gameNum = lineCount + 1;
        infile.close();
    }
    return gameNum;
}
bool check_for_winner(int x, int y, int player_symbol){
    if (check_horizontal_combo(x, y, player_symbol)) return true;
    else if (check_vertical_combo(x, y, player_symbol)) return true;
    else if (check_diagonal_combo_SW_NE(x, y, player_symbol)) return true;
    else if (check_diagonal_combo_NW_SE(x, y, player_symbol)) return true;
    else return false;
}
bool check_horizontal_combo(int x, int y, int player_symbol){
    int score=1;
    int count=1;
    while(count+x>=0 && count+x<WIDTH){
        if(board_info[y][x+count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(x-count<WIDTH && x-count>=0){
        if(board_info[y][x-count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4) return true;
    else return false;
}
bool check_vertical_combo(int x, int y, int player_symbol){
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT){
        if(board_info[y+count][x]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0){
        if(board_info[y-count][x]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4) return true;
    else return false;
}
bool check_diagonal_combo_SW_NE(int x, int y, int player_symbol){
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT && count+x>=0 && count+x<WIDTH){
        if(board_info[y+count][x+count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0 && x-count<WIDTH && x-count>=0){
        if(board_info[y-count][x-count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4) return true;
    else return false;
}
bool check_diagonal_combo_NW_SE(int x, int y, int player_symbol){
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT && x-count<WIDTH && x-count>=0){
        if(board_info[y+count][x-count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0 && count+x>=0 && count+x<WIDTH){
        if(board_info[y-count][x+count]==player_symbol){
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4) return true;
    else return false;
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
            cout << player2 << " wins! Congratulations!\n";
            winner = player2;
            saveGameInfo(player1, player2, winner);
            menu();
            break;
        }
    }
}
void menu(){
    // reset the board and game state variables in case coming back from a finished game or replay
    reset_board();
    cout<<"Please select an option:\n";
    cout<<"1. Start Game\t 2. replay a game\n";
    cout<<"3. see the scoreboard \t4. How to play?\n";
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
            while(player1.find(',') != string::npos || player1.empty()){
                cout << "Error: Name cannot contain commas(,) or be empty. Please enter again: ";
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
            while(player2.find(',') != string::npos || player2.empty()){
                cout << "Error: Name cannot contain commas(,) or be empty. Please enter again: ";
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
                    break;
                case 2:
                    cout<<"Medium mode selected. Good luck!\n";
                    break;
                case 3:
                    cout<<"Hard mode selected. Good luck!\n";
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

bool saveGameInfo(string player1, string player2, string winner){
    ifstream infile("game_log.csv");
    bool isEmpty=true;
    if(infile){
        // MAKE SURE TO UNDERSTAND IT ONCE AGAIN--- note to self
        isEmpty = infile.peek() == ifstream::traits_type::eof();
    }
    ofstream outfile("game_log.csv", ios::app);
        if(!outfile.is_open()){
            cout << "Error: Could not open game_log.csv for writing." << endl;
            return false;
        }
        if (isEmpty){
             outfile << "GameNumber,DateTime,Player1,Player2,Winner,GameMode\n";
        }
        infile.close();
    // https://www.w3schools.com/cpp/cpp_date.asp
    time_t timestamp= time(0);
    string dateTime = ctime(&timestamp);
    //UNDERSTAND THIS ASWELL ONCE AGAIN--- note to self
    dateTime.pop_back(); // remove trailing newline
    outfile << getGameNumber() << "," << dateTime << "," << player1 << "," << player2 << "," << winner << "," << gameMode << "\n";
    cout << "\nGame information saved to game_log.csv\n" << endl;
    outfile.close();
    return true;
}
void scoreboard(){
    ifstream file("game_log.csv");
    if (!file.is_open()){
        cout << "Error: Could not open game_log.csv for reading." << endl;
    }
    if(file.peek() == ifstream::traits_type::eof()){
        cout << "No games recorded yet." << endl;
        file.close();
    }
    else{
        string line;
        cout << "GameNumber | DateTime | Player1 | Player2 | Winner | GameMode" << endl;
        cout << "-------------------------------------------------------------" << endl;
        getline(file, line); // skip header
        while(getline(file, line)){
            if(!line.empty()){
                cout << line << endl;
            }
        }
        file.close();
    }
    cout<<"Enter a choice:\n";
    cout<<"1. Back to menu\t2.Delete a record  3. Exit\n";
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
            deleteRecord();
            break;
        case 3:
            cout<<"Exiting...\n";
            exit(0);
        default:
            cout<<"Invalid choice. Returning to menu.\n";
            menu();
    }
}
void deleteRecord(){};

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
void reset_board(){
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            board_info[i][j] = 0;
        }
    }
    LastMoveX = -1;
    LastMoveY = -1;
}