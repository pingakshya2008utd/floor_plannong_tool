#ifndef _read_RR_module_H_
#define _read_RR_module_H_


#include"general.h"
#include <vector>
#include <list>
#include <algorithm>

rec_reg static_mod;
vector <rec_reg> rec_region_list;

rec_reg read_static_module (string filename);   
void read_rec_module(string myfile_name);    
void print_module_list(vector <rec_reg> rec_region_list);  

#endif