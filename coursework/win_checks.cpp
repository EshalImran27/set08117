#include "globals.h"

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
