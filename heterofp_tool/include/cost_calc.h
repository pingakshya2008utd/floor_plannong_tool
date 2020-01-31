#ifndef _cost_calc_H_
#define _cost_calc_H_

#include"general.h"
#include "allocate_recon_module.h"
#include "sequence_pair.h"
#include "fpga.h"



pair <int,int> calculate_wirelength(vector <rec_reg> rec_region_list);      
int calculate_wasted_res(vector <filled_reg> filled_region_cords);          
int calculate_area(int ROWS, int COLUMS);       
cost_param allocate_list(vector <rec_reg> module_list);                  

#endif                       