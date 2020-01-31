#ifndef _GENH_
#define _GENH_

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <fstream>
#include<string>
#include <math.h>
#include<cmath>
#include <inttypes.h>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <numeric>
#include <iterator> 
#include <time.h>
#include <utility>
#include <ctime>        
#include <cstdlib>     
#include <time.h>

#define COLUMNS 64

using namespace std;


enum resource{rec_clb, rec_bram,rec_dsp, rec_iob};
enum direction{dir_left, dir_right, dir_up, dir_down};
enum moves{shuffle, swap_both, remove_replace};

typedef struct
{
    int row;
	int column;
	bool usage;
} clb;


typedef struct
{
    int row;
	int column;
	bool usage;
} bram;

typedef struct
{
    int row;
	int column;
	bool usage;
} dsp;

typedef struct
{
    int row;
	int column;
	bool usage;
} iob;

typedef struct
{
	int rw_cost;
	int cnt;
	int	total_area;
	long int total_WL;
	long int weighted_WL;
	long int FINAL_COST;
} cost_param;

typedef struct
{
	int clb_free;
	int dsp_free;
	int bram_free;
	int iob_free;
	int x_min;
    int y_min;
    int x_max;
    int y_max;
	int module_num;
} white_space;

typedef struct
{
    resource rec_type;
	string rec_name;
	int module_num;
	string rec_reg_name;
	bool usage;
} fpga;

typedef struct
{
	int row_number;
    int total_clb;
    int total_bram;
    int total_dsp;
    int total_iob;
	int free_clb;
    int free_bram;
    int free_dsp;
    int free_iob;
} fpga_row;

typedef struct
{
	int x_min;
    int y_min;
    int x_max;
    int y_max;
	int module_num;
	int centroid[2];
} filled_reg;              //filled_reg: filled region

typedef struct
{
	int X_TL;
	int Y_TL;
	int X_BR;
	int Y_BR;
	string name;
	int module_num;
	int centroid[2];
    int clb;
	int bram;
	int dsp;
	map < int, int> map_wires;
} rec_reg;              //rec_reg: reconfigurable region

	extern vector <clb> clb_vec;
	extern vector <bram> bram_vec;
	extern vector <dsp> dsp_vec;
	extern vector <iob> iob_vec;
	extern vector <int> clb_col;
	extern vector <int> dsp_col;
	extern vector <int> bram_col;
	extern vector <int> iob_col;
	extern vector <rec_reg> rec_region_list;
	extern vector <rec_reg> sorted_region_list;
	extern vector <rec_reg> shuffled_region_list;
	extern vector <rec_reg> only_clb_list;
	extern vector <filled_reg> filled_region_cords;
	extern vector <filled_reg> seq_pair_a;
	extern vector <filled_reg> seq_pair_b;
	extern vector <filled_reg> seq_pair_min_a;
	extern vector <filled_reg> seq_pair_min_b;
	extern vector <white_space> white_space_list ;
	

	//extern fpga my_fpga[20][64], temp_fpga[20][64], min_fpga[20][64];
	//extern fpga my_fpga[16][64], temp_fpga[16][64], min_fpga[16][64];
	extern fpga my_fpga[40][64], temp_fpga[40][64], min_fpga[40][64];
	extern rec_reg static_mod;
	extern int ROWS, COLUMN;
	extern int num_modules, total_clb, total_bram, total_dsp, total_iob, clb_per_tile, bram_per_tile, dsp_per_tile, iob_per_tile;
	extern long int min_cost, current_cost;
	extern ofstream outputfile;
	extern ofstream wasted_rec_fie;
	


#endif