#ifndef _simulated_annealing_H_
#define _simulated_annealing_H_


#include"general.h"
#include "calculate_hpwl.h"

void simulated_annealing(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b, int curr_cost);
void simulated_annealing_main(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b, int curr_cost);
  
#endif   