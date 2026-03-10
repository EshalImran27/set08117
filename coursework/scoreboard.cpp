#include "globals.h"

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
