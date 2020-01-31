#ifndef _FPGA_H_
#define _FPGA_H_


#include"general.h"


vector <clb> clb_vec;
vector <bram> bram_vec;
vector <dsp> dsp_vec;
vector <int> clb_col;
vector <int> dsp_col;
vector <int> bram_col;

fpga my_fpga[10][23], temp_fpga[10][23], min_fpga[10][23];
int num_modules, total_clb, total_bram, total_dsp, current_cost, min_cost;

void print_fpga(int ROWS, int COLUMS);   
void load_fpga(string myfile_name, int ROWS);  
void read_row_column(string my_file_name); 
void clear_fpga(int ROWS, int COLUMS); 

#endif