#ifndef _allocate_RR_module_H_
#define _allocate_RR_module_H_


#include"general.h"
#include "fpga.h"
	
filled_reg calculate_module_cordinate(int ROWS, int COLUMS,  int module_num, bool replcmt, int module_2);
void allocate_recon_module(rec_reg RR1,  int module_num);     
void extend_vertically(int clb_cnt, int clb_left, int y_cord, int col_num, resource rec, int clb_alloted, int module_num);  
void clb_allot_dsp_or_bram_g_t_0(int clb_cnt, int rec_cnt,int clb_alloted, resource rec_main, int dsp_col_num, int y_min_module, int bram_cnt, bool right_edge_found, int module_num, int dsp_fpga_row, int bram_fpga_row);
void allocate_only_clb(rec_reg RR1 , int module_num);   
void allocate_only_clb_2(rec_reg RR1, int module_num, int dsp_first_col, int bram_first_col); 
void allocate_static_module(rec_reg RR1, int module_num);
int allocate_clb_both_bram_dsp_greater_then_0(rec_reg RR1, int dsp_col_num, int bram_col_num, int bram_col_used, int y_min_module, int dsp_fpga_row, int bram_fpga_row);

#endif