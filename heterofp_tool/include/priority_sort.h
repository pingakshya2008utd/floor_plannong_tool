#ifndef _priority_sort_H_
#define _priority_sort_H_


#include"general.h"
#include "read_RR_module.h"

vector <rec_reg> sorted_region_list;
vector <rec_reg> only_clb_list;

void priority_sort(vector <rec_reg> rec_region_list);                    
void sort_rr_list(vector <rec_reg> L1 , resource res_type );             

#endif   