#ifndef _allocate_RR_module_H_
#define _allocate_RR_module_H_


#include"general.h"
#include "read_RR_module.h"
#include "sim_annealing_moves.h"
#include "white_space.h"

	vector <filled_reg> filled_region_cords;
	int ROWS, COLUMN;
	
	
filled_reg calculate_module_cordinate(int ROWS, int COLUMS,  int module_num, bool replcmt); 
void allocate_recon_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num);     
void extend_vertically(int clb_cnt, int clb_left, int y_cord, int col_num, resource rec, int clb_alloted, int module_num);  
void clb_allot_dsp_or_bram_g_t_0(int clb_cnt, int rec_cnt,int clb_alloted, resource rec_main, resource rec_comp, int y_min_module, int bram_cnt, bool right_edge_found, int dsp_col_num, int module_num);
void allocate_only_clb(rec_reg RR1 , int module_num);   
void allocate_only_clb_2(rec_reg RR1, int module_num, int dsp_first_col, int bram_first_col); 
void allocate_static_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num);

#endif