#ifndef GLOBALS_H
#define GLOBALS_H
#include <fstream> // for file handling
#include <iostream> // for input/output
#include <stdlib.h> //needed for rand
#include <string> // for string handling
#include <ctime> // for time-based seeding
#include <windows.h> // for Sleep
#include <sstream> // for stringstream
#include <stack> // for undo/redo stacks
#include <cctype> 
#include <vector> // for storing winning cell coordinates
using namespace std; 
#define HEIGHT 6 
#define WIDTH 7

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
extern int LastMoveX, LastMoveY, gameNumber;
extern string player1, player2, winner, gameMode;
extern stack <Move> undoStackPlayer1;
extern stack <Move> redoStackPlayer1;
extern stack <Move> undoStackPlayer2;
extern stack <Move> redoStackPlayer2;
extern int frozenColumn; // global variable to keep track of the frozen column in medium mode; -1 if no column is currently frozen
void reset_board();
void draw_board();
void menu();
void EasyMode();
void MediumMode();
void HardMode();
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
bool saveGameMoves(int game_Number, int player_symbol, int column, int row, string action = "MOVE");
void player_movement_medium(string player, int player_symbol, int frozenColumn);
//int offerFreeze(string player, int player_symbol);
static int offerFreeze(const string& player, int player_symbol);

#endif