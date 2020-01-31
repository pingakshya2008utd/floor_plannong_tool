#ifndef _simulated_annealing_H_
#define _simulated_annealing_H_


#include"general.h"
#include "fpga.h"
#include "sim_annealing_moves.h"
#include "white_space.h"
#include "priority_sort.h"

	ofstream wasted_rec_fie, outputfile;

void simulated_annealing(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b); 
  
#endif   