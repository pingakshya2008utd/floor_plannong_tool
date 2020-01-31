#ifndef _sequence_pair_H_
#define _sequence_pair_H_



#include "read_RR_module.h"



void gen_seq_pair(vector <filled_reg> filled_region_cords);  
void insert_seq(filled_reg main_mod, filled_reg seq_mod, direction dir, int seq_pair_num);     //sequence_pair.c
int find_pos(vector <filled_reg> seq_pair, filled_reg A);                                         //sequence_pair.c
void print_seq_pair(vector <filled_reg> seq_pair_a, vector <filled_reg> seq_pair_b);                 //sequence_pair.c 

#endif