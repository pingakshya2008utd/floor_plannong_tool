#ifndef _cost_calc_H_
#define _cost_calc_H_

#include"general.h"
#include "allocate_recon_module.h"
#include "sequence_pair.h"
#include "fpga.h"



pair <long int,long int>  calculate_wirelength(vector <rec_reg> rec_region_list, bool clb_exist);    
int calculate_wasted_res(vector <filled_reg> filled_region_cords);          
int calculate_area(int ROWS, int COLUMS);       
cost_param allocate_list(vector <rec_reg> module_list, bool clb_exist);     
int calculate_hpwl(vector <rec_reg> rec_region_list) ;          

#endif                       