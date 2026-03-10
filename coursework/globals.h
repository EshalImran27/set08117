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
using namespace std; 
#define HEIGHT 6 
#define WIDTH 7
struct Move {
    int col; 
    int row;
};
int board_info[HEIGHT][WIDTH] = { {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0},
                                 {0,0,0,0,0,0,0} };
int LastMoveX, LastMoveY, gameNumber;
string player1, player2, winner, gameMode;
stack <Move> undoStackPlayer1;
stack <Move> redoStackPlayer1;
stack <Move> undoStackPlayer2;
stack <Move> redoStackPlayer2;
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

#endif