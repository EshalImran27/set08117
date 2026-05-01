#include "globals.h"

// The file handling functions are responsible for saving the moves of the game as they are made to a file 
//named "game_moves.csv" and saving the overall game information (players, winner, game mode, etc.) to a 
//file named "game_log.csv" at the end of each game.
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
// The getGameNumber function reads the game_log.csv file to determine the current game number by 
// counting the number of lines in the file (excluding the header) and adding 1. This ensures that each new 
//game gets a unique, sequential game number for logging purposes.
int getGameNumber(){
    ifstream infile("game_log.csv");
    int gameNum = 0;
    if(infile){
        string line;
        getline(infile, line); // skip header
        while(getline(infile, line)){
            if(line.empty()){
                continue;
            }
            int commaPos = line.find(',');
            if(commaPos != string::npos){
                try{
                    int num = stoi(line.substr(0, commaPos));
                    if(num > gameNum){
                        gameNum = num;
                    }
                } catch(const invalid_argument& e){
                    cerr << "Error: Invalid game number in log file." << endl;
                    continue;
                }
            }
        }
        infile.close();
    }
    return gameNum+1;
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

// The cleanUnsavedMoves function is called at the start of the menu function to clean up any moves that 
// were logged to game_moves.csv for games that were never completed and thus never had their game 
// information logged to game_log.csv, to avoid having moves in the game_moves.csv file that don't 
// correspond to any actual games in the game_log.csv file. It does this by first reading the game_log.csv 
// file to find the highest valid game number, then reading through the game_moves.csv file and keeping only 
// the moves that have a game number less than or equal to that highest valid game number, and finally 
// writing those valid moves back to the game_moves.csv file (overwriting it).
void cleanUnsavedMoves(){
    ifstream logFile("game_log.csv");
    int lastGameNum = 0;
    if(logFile.is_open()){
        string line;
        getline(logFile, line); // skip header
        while(getline(logFile, line)){
            if(line.empty()) continue;
            stringstream ss(line);
            string gameNumStr;
            getline(ss, gameNumStr, ',');
            try{
                int num= stoi(gameNumStr);
                if(num > lastGameNum){
                    lastGameNum = num;
                }
            }
            catch(const invalid_argument& e){
                cerr << "Error: Invalid game number in moves file." << endl;
            }
        }
        logFile.close();
    }
    ifstream movesFile("game_moves.csv");
    if(!movesFile.is_open()) return;

    vector<string> validMoves;
    string line;
    while(getline(movesFile, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string gameNumStr;
        getline(ss, gameNumStr, ',');
        try{
            int num = stoi(gameNumStr);
            if(num <= lastGameNum){
                validMoves.push_back(line);
            }
        }
        catch(const invalid_argument& e){
            validMoves.push_back(line); // if the game number is invalid, we keep the move just in case it's from a future game that got logged before the game info for some reason, to avoid accidentally deleting valid moves
        }
    }
    movesFile.close();
    ofstream outMoves("game_moves.csv", ios::trunc);
    if(!outMoves.is_open()){
        cout << "Error: Could not open game_moves.csv for writing." << endl;
        return;
    }
    for(const auto& move : validMoves){
        outMoves << move << "\n";
    }
    outMoves.close();
}