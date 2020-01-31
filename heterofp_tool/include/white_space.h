#ifndef _white_space_H_
#define _white_space_H_

#include"general.h"

//fpga my_fpga[10][23];
vector <white_space> white_space_list ;

void calculate_white_space(int ROWS, int COLUMS);                                //white_space.c
void calculate_white_space_cordinate(int ROWS, int COLUMS,  int curr_module_min);           //white_space.c
white_space calculate_ws_cordinate(int ROWS, int COLUMS,  int module_num);                  //white_space.c

#endif