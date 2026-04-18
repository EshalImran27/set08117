#include "globals.h"

bool saveGameMoves(int gameNumber, int player_symbol,  int column,  int row, string action){
    string movesFilename = "game_moves.csv";
    ofstream outfile("game_moves.csv", ios::app);
     if(!outfile.is_open()){
            cout << "Error: Could not open game_moves.csv for writing." << endl;
            return false;
    }
    //might add some other things to log later like undo redo moves.
    outfile << gameNumber << "," << player_symbol << "," << column << "," << row << "," << action << "\n";
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