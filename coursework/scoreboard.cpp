#include "globals.h"
#include <iomanip>

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

        cout << left << setw(12) << "GameNumber" << setw(28) << "DateTime" << setw(16) << "Player1" << setw(16) << "Player2" << setw(16) << "Winner" << setw(12) << "GameMode" << endl;
        cout << string(12 + 28 + 16 + 16 + 16 + 12, '-') << endl;
        getline(file, line); // skip header
        while(getline(file, line)){
            if(!line.empty()){
                stringstream ss(line);
                string gameNumberStr, dateTimeStr, player1Str, player2Str, winnerStr, gameModeStr;

                getline(ss, gameNumberStr, ',');
                getline(ss, dateTimeStr, ',');
                getline(ss, player1Str, ',');
                getline(ss, player2Str, ',');
                getline(ss, winnerStr, ',');
                getline(ss, gameModeStr, ',');

                cout << left << setw(12) << gameNumberStr << setw(28) << dateTimeStr << setw(16) << player1Str << setw(16) << player2Str << setw(16) << winnerStr << setw(12) << gameModeStr << endl;
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
void deleteRecord(){
    cout << "Enter the game number of the record you want to delete: ";
    int gameNum;
    cin >> gameNum;
    if (cin.fail()){
        cout << "Error!";
        exit(1);
    }
    ifstream infile("game_log.csv");
    if (!infile.is_open()){
        cout << "Error: Could not open game_log.csv for reading." << endl;
        return;
    }
    ofstream tempFile("temp.csv");
    if (!tempFile.is_open()){
        cout << "Error: Could not open temp.csv for writing." << endl;
        infile.close();
        return;
    }
    string line;
    bool recordFound = false;
    getline(infile, line); // copy header
    tempFile << line << "\n";
    while(getline(infile, line)){
        if(!line.empty()){
            stringstream ss(line);
            string gameNumberStr;
            getline(ss, gameNumberStr, ',');
            int currentGameNum = stoi(gameNumberStr);
            if(currentGameNum == gameNum){
                recordFound = true;
                continue; // skip writing this line to temp file
            }
            tempFile << line << "\n"; // write all other lines to temp file
        }
    }
    infile.close();
    tempFile.close();
    if(recordFound){
        if (remove("game_log.csv") != 0){
            cout << "Error: Could not delete original game_log.csv file." << endl;
            return;
        }
        if (rename("temp.csv", "game_log.csv") != 0){
            cout << "Error: Could not rename temp.csv to game_log.csv." << endl;
            return;
        }
        cout << "Record for game number " << gameNum << " deleted successfully.\n";
    }
    else{
        cout << "No record found for game number " << gameNum << ".\n";
        remove("temp.csv"); // clean up temp file if no record was deleted
    }
    cout<<"Enter a choice:\n";
    cout<<"1. Back to menu\t2. Exit\n";
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
            cout<<"Exiting...\n";
            exit(0);
        default:
            cout<<"Invalid choice. Returning to menu.\n";
            menu();
    }
    
};
