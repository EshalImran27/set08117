#ifndef GLOBALS_H
#define GLOBALS_H
#include <fstream> // for file handling
#include <iostream> // for input/output
#include <stdlib.h> //needed for exit()
#include <string> // for string handling
#include <ctime> // for time-based seeding
#include <windows.h> // for Sleep
#include <sstream> // for stringstream
#include <stack> // for undo/redo stacks
#include <cctype> // for isdigit function
#include <vector> // for storing winning cell coordinates
#include <thread> // for timer thread
#include <chrono> // for time calculations in timer thread
using namespace std; 

// Global variables
#define HEIGHT 6 
#define WIDTH 7
// ANSI escape codes for text colors
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"
#define WIN "\033[32m"
struct Move {
    int col; 
    int row;
    int player_symbol; // 1 for player 1, 2 for player 2
};
struct Cell { int row; int col; };
extern vector<Cell> winningCells; // global variable to store the coordinates of the winning pieces for highlighting in the replay
extern int board_info[HEIGHT][WIDTH];
extern int LastMoveX, LastMoveY, gameNumber, pl1Moves, pl2Moves;
extern string player1, player2, winner, gameMode;
extern stack <Move> undoStackPlayer1;
extern stack <Move> redoStackPlayer1;
extern stack <Move> undoStackPlayer2;
extern stack <Move> redoStackPlayer2;
extern int frozenColumn, threeInARowCountpl1, threeInARowCountpl2; // global variable to keep track of the frozen column in medium mode; -1 if no column is currently frozen
extern bool hasFreezepl1, goPowerP1, goPowerusedP1; // flag to track if player 1 has earned the freeze power
extern bool hasFreezepl2, goPowerP2, goPowerusedP2; // flag to track if player 2 has earned the freeze power
extern bool timeMode; // flag to indicate if the game is in time mode
extern bool timeUp; // flag to indicate if time is up in time mode
extern volatile long timeLeftSeconds; // variable to track the time left in seconds for time mode
extern HANDLE timerThreadHandle; // handle for the timer thread
DWORD WINAPI TimerThreadFunction(LPVOID); // function prototype for the timer thread function

// Function prototypes
//file handling functions
bool saveGameMoves(int game_Number, int player_symbol, int column, int row, string action = "MOVE");
int getGameNumber();
bool saveGameInfo(string player1, string player2, string winner);
void cleanUnsavedMoves();
// game board functions
void reset_board();
void draw_board();
void printTimer();
//game replay functions
void gameReplay(int game_Number);
// player functions
void player_movement(string player, int player_symbol);
void player_movement_medium(string player, int player_symbol);
void player_movement_hard(string player, int player_symbol);
bool freezePower(int player_symbol); // function to check if the player has earned the freeze power in medium mode
int countThreeInARow(int player_symbol); // function to count the number of separate 3 in a rows a player has on the board, used for determining if they've earned the GO power-up in hard mode
bool goPowerEarned(int player_symbol); // function to check if the player has earned the GO power-up in hard mode by having 2 separate 3 in a rows at the same time
//scoreboard functions
void scoreboard();
void deleteRecord();
// win checking functions
int check_horizontal_combo(int x, int y, int player_symbol);
int check_vertical_combo(int x, int y, int player_symbol);
int check_diagonal_combo_SW_NE(int x, int y, int player_symbol);
int check_diagonal_combo_NW_SE(int x, int y, int player_symbol);
bool check_for_winner(int x, int y, int player_symbol);
// mode selection and menu functions
void startTimer(int seconds);
void stopTimer();
void helperModes(string mode);
void menu();
#endif