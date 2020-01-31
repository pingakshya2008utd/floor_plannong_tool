#ifndef _sim_annealing_moves_H_
#define _sim_annealing_moves_H_


#include"general.h"


vector <rec_reg>  shuffle_rr_list(vector <rec_reg> rec_region_list);                          
vector <rec_reg>  swap_two_modules(vector <rec_reg> rec_region_list, int pos1, int pos2);     
cost_param shift_func(direction dir, int module_num, int num_blocks);                         
vector <rec_reg>  remove_n_replace(vector <rec_reg> rec_region_list, int module_num);         
int find_pos_rec(vector <rec_reg> seq_pair, rec_reg A);

#endif                                     