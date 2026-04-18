#include "globals.h"

vector<Cell> winningCells; // global variable to store the coordinates of the winning pieces for highlighting in the replay
bool check_for_winner(int x, int y, int player_symbol){
    if (check_horizontal_combo(x, y, player_symbol)) return true;
    else if (check_vertical_combo(x, y, player_symbol)) return true;
    else if (check_diagonal_combo_SW_NE(x, y, player_symbol)) return true;
    else if (check_diagonal_combo_NW_SE(x, y, player_symbol)) return true;
    else return false;
}
bool check_horizontal_combo(int x, int y, int player_symbol){
    vector<Cell> tempWinningCells; // temporary vector to store the coordinates of the pieces in the current combo being checked; if a winning combo is found, these coordinates will be copied to the global winningCells vector for highlighting in the replay
    tempWinningCells.push_back({y, x}); // add the current piece to the temp vector
    int score=1;
    int count=1;
    while(count+x>=0 && count+x<WIDTH){
        if(board_info[y][x+count]==player_symbol){
            tempWinningCells.push_back({y, x+count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(x-count<WIDTH && x-count>=0){
        if(board_info[y][x-count]==player_symbol){
            tempWinningCells.push_back({y, x-count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4){
        winningCells = tempWinningCells; // copy the coordinates of the winning pieces to the global vector
        return true;
    }
    else return false;
}
bool check_vertical_combo(int x, int y, int player_symbol){
    vector<Cell> tempWinningCells;
    tempWinningCells.push_back({y, x});
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT){
        if(board_info[y+count][x]==player_symbol){
            tempWinningCells.push_back({y+count, x});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0){
        if(board_info[y-count][x]==player_symbol){
            tempWinningCells.push_back({y-count, x});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4){
        winningCells = tempWinningCells; // copy the coordinates of the winning pieces to the global vector
        return true;
    }
    else return false;
}
bool check_diagonal_combo_SW_NE(int x, int y, int player_symbol){
    vector<Cell> tempWinningCells;
    tempWinningCells.push_back({y, x});
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT && count+x>=0 && count+x<WIDTH){
        if(board_info[y+count][x+count]==player_symbol){
            tempWinningCells.push_back({y+count, x+count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0 && x-count<WIDTH && x-count>=0){
        if(board_info[y-count][x-count]==player_symbol){
            tempWinningCells.push_back({y-count, x-count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4){
        winningCells = tempWinningCells; // copy the coordinates of the winning pieces to the global vector
        return true;
    }
    else return false;
}
bool check_diagonal_combo_NW_SE(int x, int y, int player_symbol){
    vector<Cell> tempWinningCells;
    tempWinningCells.push_back({y, x});
    int score=1;
    int count=1;
    while(count+y>=0 && count+y<HEIGHT && x-count<WIDTH && x-count>=0){
        if(board_info[y+count][x-count]==player_symbol){
            tempWinningCells.push_back({y+count, x-count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    count=1;
    while(y-count<HEIGHT && y-count>=0 && count+x>=0 && count+x<WIDTH){
        if(board_info[y-count][x+count]==player_symbol){
            tempWinningCells.push_back({y-count, x+count});
            score++;
            count++;
        }
        else break;  //If no combo is detected break from the loop
    }
    if (score >= 4){
        winningCells = tempWinningCells; // copy the coordinates of the winning pieces to the global vector
        return true;
    }
    else return false;
}
