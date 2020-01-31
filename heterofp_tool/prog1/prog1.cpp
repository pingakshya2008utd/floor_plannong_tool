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


#define COLUMNS 23

using namespace std;


enum resource{rec_clb, rec_bram,rec_dsp};
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
	int rw_cost;
	int cnt;
	int	total_area;
	int total_WL;
	int weighted_WL;
	int FINAL_COST;
} cost_param;

typedef struct
{
	int clb_free;
	int dsp_free;
	int bram_free;
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
	int free_clb;
    int free_bram;
    int free_dsp;
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


	vector <clb> clb_vec;
	vector <bram> bram_vec;
	vector <dsp> dsp_vec;
	vector <int> clb_col;
	vector <int> dsp_col;
	vector <int> bram_col;
	vector <rec_reg> rec_region_list;
	vector <rec_reg> sorted_region_list;
	vector <rec_reg> shuffled_region_list;
	vector <rec_reg> only_clb_list;
	vector <filled_reg> filled_region_cords;
	vector <filled_reg> seq_pair_a;
	vector <filled_reg> seq_pair_b;
	vector <filled_reg> seq_pair_min_a;
	vector <filled_reg> seq_pair_min_b;
	vector <white_space> white_space_list ;

	fpga my_fpga[10][23], temp_fpga[10][23], min_fpga[10][23];
	rec_reg static_mod;
	int ROWS, COLUMN;
	int num_modules, total_clb, total_bram, total_dsp, current_cost, min_cost;;
	ofstream outputfile;
	ofstream wasted_rec_fie;
	

void allocate_static_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num);    // allocate_static_module.c
rec_reg read_static_module (string filename);          //read_RR_mocule.c
void print_fpga(int ROWS, int COLUMS);                    //fpga.c
void load_fpga(string myfile_name, int ROWS);             //fpga.c
void read_row_column(string my_file_name);                //fpga.c
void read_rec_module(string myfile_name);                   //read_RR_mocule.c
void print_module_list(vector <rec_reg> rec_region_list);     //read_RR_mocule.c
filled_reg calculate_module_cordinate(int ROWS, int COLUMS,  int module_num, bool replcmt);    // allocate_recon_module.c
void allocate_recon_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num);           // allocate_recon_module.c
void extend_vertically(int clb_cnt, int clb_left, int y_cord, int col_num, resource rec, int clb_alloted, int module_num);   // allocate_recon_module.c
void clb_allot_dsp_or_bram_g_t_0(int clb_cnt, int rec_cnt,int clb_alloted, resource rec_main, resource rec_comp, int y_min_module, int bram_cnt, bool right_edge_found, int dsp_col_num, int module_num);  // allocate_recon_module.c
void priority_sort(vector <rec_reg> rec_region_list);                    //priority_sort.c
void sort_rr_list(vector <rec_reg> L1 , resource res_type );                //priority_sort.c
pair <int,int> calculate_wirelength(vector <rec_reg> rec_region_list);      //cost_calc.c
int calculate_wasted_res(vector <filled_reg> filled_region_cords);           //cost_calc.c
int calculate_area(int ROWS, int COLUMS);                                     //cost_calc.c
void gen_seq_pair(vector <filled_reg> filled_region_cords);                                      //sequence_pair.c
void insert_seq(filled_reg main_mod, filled_reg seq_mod, direction dir, int seq_pair_num);     //sequence_pair.c
int find_pos(vector <filled_reg> seq_pair, filled_reg A);                                         //sequence_pair.c
void print_seq_pair(vector <filled_reg> seq_pair_a, vector <filled_reg> seq_pair_b);                 //sequence_pair.c
void clear_fpga(int ROWS, int COLUMS);                                 //fpga.c
cost_param allocate_list(vector <rec_reg> module_list);                   //cost_calc.c
int find_pos_rec(vector <rec_reg> seq_pair, rec_reg A);                         //sim_annealing_moves.c
void calculate_white_space(int ROWS, int COLUMS);                                //white_space.c
void calculate_white_space_cordinate(int ROWS, int COLUMS,  int curr_module_min);           //white_space.c
white_space calculate_ws_cordinate(int ROWS, int COLUMS,  int module_num);                  //white_space.c
void allocate_only_clb(rec_reg RR1 , int module_num);    // allocate_recon_module.c
void allocate_only_clb_2(rec_reg RR1, int module_num, int dsp_first_col, int bram_first_col);   // allocate_recon_module.c
///************** moves of SA***************************************////
vector <rec_reg>  shuffle_rr_list(vector <rec_reg> rec_region_list);                              //sim_annealing_moves.c
vector <rec_reg>  swap_two_modules(vector <rec_reg> rec_region_list, int pos1, int pos2);     //sim_annealing_moves.c
cost_param shift_func(direction dir, int module_num, int num_blocks);                             //sim_annealing_moves.c
vector <rec_reg>  remove_n_replace(vector <rec_reg> rec_region_list, int module_num);               //sim_annealing_moves.c

////************************* sumulated annealing******************************////

void simulated_annealing(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b);          //simulated_annealing.c


int main()
{	
	int cnt;
	cost_param cost_fac;
	outputfile.open ("cordinates.txt");
	wasted_rec_fie.open ("wasted_rec_fie.txt");
	outputfile<<"hi how r u";
	vector <rec_reg> ::iterator it;
	rec_reg RR1, RR2,RR3,RR4;
	int rr_list_size;
	string filename="static.txt";
	read_row_column("virtex.fpg");
	load_fpga("virtex.fpg", ROWS);   
	static_mod=read_static_module( filename);
	read_rec_module("recon_module_2.txt");
	priority_sort(rec_region_list);
	int i=0;
	print_module_list(rec_region_list);
	allocate_static_module(static_mod,  my_fpga, 0);
	
	//sorted_region_list 
	//cost_fac=allocate_list(rec_region_list);
	shuffled_region_list=swap_two_modules(rec_region_list, 0, 3);
//	cost_fac=allocate_list(shuffled_region_list);
	print_module_list(shuffled_region_list);
	print_module_list(sorted_region_list);
	shuffled_region_list=remove_n_replace(rec_region_list, 3);
	cout<<endl<<" module 1 removed and replaced";
	print_module_list(shuffled_region_list);
	//calculate_white_space(ROWS, COLUMNS);
	shuffled_region_list=shuffle_rr_list( sorted_region_list);
	//cost_fac=allocate_list(sorted_region_list);
	cost_fac=allocate_list(sorted_region_list);
	current_cost=cost_fac.FINAL_COST;
	min_cost=cost_fac.FINAL_COST;
	cout<<endl<<" cost imitial:  "<<cost_fac.FINAL_COST<<endl;
	outputfile<<endl<<" cost imitial:  "<<cost_fac.FINAL_COST<<endl;
	simulated_annealing(seq_pair_a, seq_pair_b);
	//cost_fac= shift_func(dir_left, 4, 3);
//	calculate_white_space(ROWS, COLUMNS);
	rr_list_size= filled_region_cords.size();
	outputfile.close();
}




void simulated_annealing(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b)
{
	
	float min_temp, next_temp, temp;
	int cost;
	cost_param cost_fac;
	//min_temp= 0.005* cost/(no_modules); 
	unsigned int max_iteration=12;
	int val=0;
	cout<<endl<<"pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp 2 iteration pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"<<endl;
	
	
	
	for (unsigned iteration = 0; iteration < max_iteration; iteration++)
	{
		val=iteration%4;
		clear_fpga(ROWS, COLUMNS);
		print_fpga(ROWS, COLUMNS);
		cout<<endl<<" fpga printed iteration number: "<<iteration<<endl;
		filled_region_cords.clear();
		allocate_static_module(static_mod,  my_fpga, 0);
		shuffled_region_list.clear();
		//shuffled_region_list=shuffle_rr_list( rec_region_list);
		//cost_fac=allocate_list(shuffled_region_list);*/
	
		switch (val) 
		 {
			 case 0:            
				shuffled_region_list=shuffle_rr_list( sorted_region_list);
				cost_fac=allocate_list(shuffled_region_list);
				break;
			case 1:            
				shuffled_region_list=remove_n_replace(sorted_region_list, 2);
				cost_fac=allocate_list(shuffled_region_list);
				break;
			case 2:   
				shuffled_region_list=swap_two_modules(sorted_region_list, 0, 2);
				//shuffled_region_list=shuffle_rr_list( rec_region_list);
				cost_fac=allocate_list(shuffled_region_list);				
				break;
			case 3:     
				shuffled_region_list=remove_n_replace(sorted_region_list, 2);
				cost_fac=allocate_list(shuffled_region_list);
				//cost_fac=shift_func(dir_left, 3, 5);	
				//shuffled_region_list=shuffle_rr_list( rec_region_list);
				//cost_fac=allocate_list(shuffled_region_list);					
				cout<<"Thank you for playing!\n";
				break;
			default:        
				cost_fac=allocate_list(sorted_region_list);
				break;
				
		}
		
		current_cost=cost_fac.FINAL_COST;
		if(current_cost<min_cost)
		{
			min_cost=cost_fac.FINAL_COST;
			seq_pair_min_a=seq_pair_a;
			seq_pair_min_b=seq_pair_b;
			
			cout<<endl;
			for (unsigned int j=0; j<ROWS;j++)
			{
				for(unsigned int i=0; i<COLUMNS; i++)
				{
					min_fpga[j][i]= my_fpga[j][i];
				}
			}
		}
		
		wasted_rec_fie<<endl<<"\n iteration number: "<<iteration;
		outputfile <<endl<< "iteration number: "<<iteration<<"  wasted resource: " << cost_fac.rw_cost<<"  total area: "<<cost_fac.total_area<<"  wiighed WL: "<<10*cost_fac.weighted_WL<<"   FINAL COST: "<<cost_fac.FINAL_COST<<endl;
	}
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			my_fpga[j][i]= min_fpga[j][i];
		}
	}
	
	calculate_white_space(ROWS, COLUMNS);
	cost_fac=allocate_list(only_clb_list);
	cout<<endl<<"  size od clb list:  "<<only_clb_list.size()<<endl;
	outputfile <<endl<< " max iteration number: "<<max_iteration<<"  wasted resource: " << cost_fac.rw_cost<<"  total area: "<<cost_fac.total_area<<"  wiighed WL: "<<10*cost_fac.weighted_WL<<"   FINAL COST: "<<cost_fac.FINAL_COST<<endl;
	cout<<endl<<"min cost: "<<min_cost<<endl;
	print_seq_pair(seq_pair_a, seq_pair_b);
	
}


void calculate_white_space(int ROWS, int COLUMS)
{

	int x_min_init, x_max_init, x_min_curr, x_max_curr, curr_module_num=-1, col_min, col_max, j, x_max_present, x_max_past, curr_module_min=1, cnt;
	white_space ws1;
	
	vector <filled_reg>:: iterator it;
	int row_cnt=0;
	filled_reg  RR1;
	
//////************************** from left boundary ***********************************************/////////////

		col_min=0;
		col_max=COLUMS;
		j=0;
		curr_module_num--;
		row_cnt=0;
		cnt=0;
L4:	x_max_past=0;
	row_cnt=0;
	
	x_max_present=0;
L3:	for(unsigned int i=0; i<COLUMS+9;i++)
	{
			if(i>=COLUMS)
			{
				if(row_cnt==0)
				{
					x_max_past=i-1;
					cout<<"\n row_cnt===0 && row J= "<<j<<" rown count: "<<row_cnt;
					row_cnt++;
					col_min=0;
					j=j+1;
					
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						break;
					}
					goto L3;
				}
				
				else if(row_cnt>0 && x_max_present==x_max_past)
				{
					j=j+1;
					col_min=0;
					//x_max_past=i;
				//	goto L3;
					cout<<"\n  row_cnt>0 && x_max_present==x_max_past row j= "<<j;
					
					
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<" break";
						break;
					}
					goto L3;
				}
				
				else if(x_max_present!=x_max_past)
				{
					j=j+0;
					x_max_past=i-1;
					for(unsigned int p=0; p<x_max_past+1; p++)
					{
						my_fpga[j][p].module_num=-1;	
					}
					row_cnt=0;
					cout<<endl;
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<" break";
						break;
					}
					curr_module_num--;
					row_cnt=0;
					goto L3;
				}
				
			}
			  
			
			if(my_fpga[j][i].module_num<=-1)
			{
				my_fpga[j][i].module_num=curr_module_num;
				x_max_present=i;
				cout<<endl<<" module number:  free space  column i= "<<i<<" row j= "<<j<<" assigned module: "<<curr_module_num<<"  row count: "<<row_cnt<<" x max present: "<<x_max_present<<"  x max past:  "<<x_max_past;
				cnt++;
			}
			
			else if(my_fpga[j][i].module_num>-1 )
			{
				if(row_cnt==0)
				{
					x_max_past=i-1;
					cout<<"\n row_cnt===0 && row J= "<<j<<" rown count: "<<row_cnt;
					row_cnt++;
					col_min=0;
					j=j+1;
					
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						break;
					}
					goto L3;
				}
				
				else if(row_cnt>0 && x_max_present==x_max_past)
				{
					j=j+1;
					col_min=0;
					//x_max_past=i;
				//	goto L3;
					cout<<"\n  row_cnt>0 && x_max_present==x_max_past row j= "<<j;
					
					
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<" break";
						break;
					}
					goto L3;
				}
				
				else if(x_max_present!=x_max_past)
				{
					j=j+0;
					x_max_past=i-1;
					for(unsigned int p=0; p<x_max_past+1; p++)
					{
						my_fpga[j][p].module_num=-1;	
					}
					row_cnt=0;
					cout<<endl;
					if(j>=ROWS)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<" break";
						break;
					}
					curr_module_num--;
					row_cnt=0;
					goto L3;
				}
				
				
			}
	}
		

/////************************* from module end to right *******************************************************////	

for (it=filled_region_cords.begin(); it!=filled_region_cords.end(); ++it)
	{
		RR1=*it;
		//it=filled_region_cords.begin();
		RR1=*it;
		col_min=RR1.x_max+1;
		col_max=COLUMS;
		j=RR1.y_min;
		curr_module_num--;
		row_cnt=0;
		cnt=0;
		x_max_past=RR1.x_max+1;
		x_max_present=RR1.x_max+1;
		cout<<endl<<"FIlled regin module number: "<<RR1.module_num;
L6:		for(unsigned int i=col_min; i<COLUMS+9 ;i++)
		{
			if(i>=COLUMS)
			{
				if(row_cnt==0 )
				{
					x_max_past=i;
					row_cnt++;
					col_min=RR1.x_max+1;
					cout<<" row_cnt==0 j= "<<j<<"  RR1.ymax:  "<<RR1.y_max;
					j=j+1;
					
					if(j>RR1.y_max)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						break;
					}
					goto L6;
				}
				
				else if (row_cnt>0)
				{
					x_max_past=i;
					row_cnt++;
					col_min=RR1.x_max+1;
					cout<<"  row_cnt>0 j= "<<j<<"  RR1.ymax:  "<<RR1.y_max;
					j=j+1;
					
					if(j>RR1.y_max)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<"  break"<<endl;
						break;
					}
					goto L6;
				}
			}
			
			if(my_fpga[j][i].module_num<=-1)
			{
				my_fpga[j][i].module_num=curr_module_num;
				x_max_present=i;
				cout<<endl<<" module number: "<<RR1.module_num<<" row= "<<j<<" column i= "<<i<<" assigned module: "<<curr_module_num;
				cnt++;
			}
			
			else if(my_fpga[j][i].module_num>-1 )
			{
				if(row_cnt==0)
				{
					x_max_past=i;
					row_cnt++;
					col_min=RR1.x_max+1;
					j=j+1;
					cout<<"\n row_cnt==0 && row j= "<<j<<"  RR1.ymax:  "<<RR1.y_max;
					if(j>RR1.y_max)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						break;
					}
					goto L6;
				}
				
				else if(row_cnt>0 && x_max_present==x_max_past)
				{
					j=j+1;
					col_min=RR1.x_max+1;
					x_max_past=i;
				//	goto L3;
					cout<<"\n  row_cnt>0 && x_max_present==x_max_past row j= "<<j<<"  RR1.ymax:  "<<RR1.y_max;
					if(j>RR1.y_max)
					{
						if(cnt==0)
						{
							curr_module_num++;
						}
						else if(cnt>0)
						{
							curr_module_num=curr_module_num;
						}
						cout<<" break";
						break;
					}
					goto L6;
				}
				else if(x_max_present!=x_max_past)
				{
					j=j+0;
					col_min=RR1.x_max+1;
					x_max_past=i;
					cout<<"\n  x_max_present!=x_max_past  j= "<<j<<"  RR1.ymax:  "<<RR1.y_max;
				//	goto L3;
					curr_module_num--;
				}
			}
		}
		
	
	
	}
	
	curr_module_min=curr_module_num;
	cout<<"\n curr module min: "<<curr_module_min<<endl;
	
	
	calculate_white_space_cordinate(ROWS, COLUMNS,curr_module_min);
}


void calculate_white_space_cordinate(int ROWS, int COLUMS,  int curr_module_min)
{
	filled_reg FR1;
	white_space WS1;
	vector <white_space>:: iterator it;
	
	cout<<endl<<" in calculate_white_space_cordinate  curr_module_min: "<<curr_module_min<<endl;
	for(int module_num=curr_module_min; module_num<-1; module_num++)
	{
		
		cout<<endl<<" "<<module_num;
		
		
		WS1=calculate_ws_cordinate(ROWS, COLUMNS, module_num);
		//FR1=calculate_module_cordinate(ROWS, COLUMNS, module_num, false);
		
		white_space_list.push_back(WS1);
	}
	
	for (it=white_space_list.begin(); it!=white_space_list.end(); ++it)
	{
		WS1=*it;
		cout<<endl<<"white space module: "<<WS1.module_num<<" ("<<WS1.x_min<<","<<WS1.y_min<<"),("<<WS1.x_max<<","<<WS1.y_max<<")"<< " clb free: "<<WS1.clb_free<<"  bram free:  "<<WS1.bram_free<<" dsp free:  "<<WS1.dsp_free;
	}
	
	cout<<endl;
}





cost_param allocate_list(vector <rec_reg> module_list)
{
	cost_param cost_fac;
	int total_wirelength=0, rw_cost=0, total_area=0, i=0, final_cost;
	rec_reg RR1;
	cost_fac.rw_cost= 0;
	cost_fac.total_area=0;
	cost_fac.total_WL=0;
	cost_fac.weighted_WL=0;
	cost_fac.cnt=0;
	cost_fac.FINAL_COST=0;
	vector <rec_reg> ::iterator it;
	vector <rec_reg> ::reverse_iterator it_rv;
	pair <int,int> length_pair;
	length_pair.first=0;
	length_pair.second=0;
	for (it=module_list.begin(); it!=module_list.end(); ++it)
    {
		i++;
		RR1=*it;
		cout<<endl<<" -------------------------------------------------------------------------------------------module number: "<<RR1.module_num<<endl;
		allocate_recon_module(RR1,  my_fpga, RR1.module_num);
		
	}
	

	
	//it_rv=module_list.rbegin();
	//RR1=*it_rv;
	//allocate_recon_module(RR1,  my_fpga, RR1.module_num);

	gen_seq_pair(filled_region_cords);
	length_pair=calculate_wirelength(rec_region_list);
	rw_cost= calculate_wasted_res(filled_region_cords);
	total_area= calculate_area(ROWS, COLUMNS);
	
	print_seq_pair(seq_pair_a, seq_pair_b);
	cost_fac.rw_cost= rw_cost;
	cost_fac.total_area=total_area;
	cost_fac.total_WL=length_pair.first;
	cost_fac.weighted_WL=length_pair.second;
	cost_fac.cnt=0;
	final_cost=rw_cost+total_area+10*length_pair.second;
	cost_fac.FINAL_COST=final_cost;
	
	cout<<endl<<"total wirelenth: "<<length_pair.first<<"\nweighted wire length:  "<<length_pair.second<<"\nwasted resource weigthed : "<<rw_cost<<"\ntotal area: "<<total_area<<"\nFINAL COST: "<<final_cost<<endl;
	print_fpga(ROWS, COLUMNS); 
	
	
	cout<<"done"<<endl;
	return(cost_fac);


}

void clear_fpga(int ROWS, int COLUMS)
{
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			temp_fpga[j][i]=my_fpga[j][i];
			if(my_fpga[j][i].module_num!=0)
			{
				my_fpga[j][i].usage=false;
				my_fpga[j][i].module_num=-1;
			}
			else if(my_fpga[j][i].module_num==0)
			{
				my_fpga[j][i].usage=false;
				my_fpga[j][i].module_num=-1;
			}
			
		}
	}
}

/******************************* moves for SA ***********************************************************/
vector <rec_reg>  shuffle_rr_list(vector <rec_reg> rec_region_list)
{
	vector <rec_reg> RR_region_temp;
	RR_region_temp.clear();
	RR_region_temp=rec_region_list;
	
	std::random_shuffle ( RR_region_temp.begin(), RR_region_temp.end() );  
	wasted_rec_fie<<endl<<"---------------------------------------------------------------------------------------------------size of list: "<<RR_region_temp.size();
	return(RR_region_temp);
}

vector <rec_reg>  swap_two_modules(vector <rec_reg> rec_region_list, int pos1, int pos2)
{
	vector <rec_reg> RR_region_temp;
	RR_region_temp.clear();
	RR_region_temp=rec_region_list;
	
	iter_swap(RR_region_temp.begin() + pos1, RR_region_temp.begin() + pos2);
	
	return(RR_region_temp);
}

vector <rec_reg>  remove_n_replace(vector <rec_reg> rec_region_list, int module_num)
{
	vector <rec_reg> RR_region_temp;
	vector <rec_reg> :: iterator it;;
	RR_region_temp.clear();
	RR_region_temp=rec_region_list;
	rec_reg RR1;
	RR1.module_num=module_num;
	int pos;
	pos= find_pos_rec(RR_region_temp, RR1);
	it=RR_region_temp.begin()+pos;
	RR1=*it;
	RR_region_temp.erase (RR_region_temp.begin()+pos);
	RR_region_temp.push_back(RR1);
	return(RR_region_temp);
		
}

cost_param shift_func(direction dir, int module_num, int num_blocks)
{
	int x_min, y_min, x_max, y_max, x_cord, x_other, y_cord, count=0;
	cost_param cost_fac;
	int total_wirelength, rw_cost, total_area, i=0;
	pair <int,int> length_pair;
	filled_reg SR=calculate_module_cordinate(ROWS, COLUMNS, module_num, true);
	x_min=SR.x_min;
	y_min=SR.y_min;
	x_max=SR.x_max;
	y_max=SR.y_max;
	cout<<endl<<"module number: "<<module_num<<" x_min: "<<x_min<<"   y_min:  "<<y_min<<"   x_max: "<<x_max<<"  y_mAX:  "<<y_max<<endl;
	if (dir==dir_left)
	{
		x_cord=x_min-1;
		x_other=x_max;
	}
	else if (dir==dir_right)
	{
		x_cord=x_max+1;
		x_other=x_min;
	}
	cout<<endl<<"num_blocks:  "<<num_blocks<<endl;
	while(count< num_blocks)
	{
		cout<<endl<<"while loop"<<endl;
		
		for(unsigned int j=y_min; j<=y_max;j++)
		{
			cout<<endl<<"j= "<<j<<endl;
			y_cord=j;
			if(my_fpga[j][x_cord].module_num>-1|| my_fpga[j][x_cord].rec_type!=rec_clb ||  my_fpga[j][x_other].rec_type!=rec_clb )
			{
				cout<<endl<<"my_fpga[j][x_cord].module_num>-1|| my_fpga[j][x_cord].rec_type!=rec_clb ||  my_fpga[j][x_other].rec_type!=rec_clb"<<endl;
				y_cord=j;
				break;
			}
		}
		
		cout<<"y_max= "<<y_max<<"  y_cord= "<<y_cord<<endl;
		
		if(y_cord!= y_max)
		{
			cout<<endl<<"(y_cord!= y_max"<<endl;
			break;
		}
		else if (y_cord==y_max)
		{
			for(unsigned int j=y_min; j<=y_max;j++)
			{
				
				my_fpga[j][x_cord].module_num=module_num;
				my_fpga[j][x_cord].usage=my_fpga[j][x_other].usage;
				my_fpga[j][x_other].usage=false;
				my_fpga[j][x_other].module_num=-1;
				
			}
		}
		
		if (dir==dir_left)
		{
			x_cord--;
			x_other--;
		}
		else if (dir==dir_right)
		{
			x_cord++;
			x_other++;
		}
		
		count++;
	}
	
	cout<<endl<<count<<endl;
	
	SR=calculate_module_cordinate(ROWS, COLUMNS, module_num, true);
	
	gen_seq_pair(filled_region_cords);
	length_pair=calculate_wirelength(rec_region_list);
	rw_cost= calculate_wasted_res(filled_region_cords);
	total_area= calculate_area(ROWS, COLUMNS);
	
	print_seq_pair(seq_pair_a, seq_pair_b);
	cost_fac.rw_cost= rw_cost;
	cost_fac.total_area=total_area;
	cost_fac.total_WL=length_pair.first;
	cost_fac.weighted_WL=length_pair.second;
	cost_fac.cnt=count;
	cost_fac.FINAL_COST=rw_cost+total_area+10*length_pair.second;
	
	cout<<endl<<"total wirelenth: "<<length_pair.first<<"\nweighted wire length:  "<<length_pair.second<<"\nwasted resource weigthed : "<<rw_cost<<"\ntotal area: "<<total_area<<"\nFINAL COST: "<<cost_fac.FINAL_COST<<endl;
	print_fpga(ROWS, COLUMNS); 
	return(cost_fac);
}

//********************************************************************************************************************************/
void gen_seq_pair(vector <filled_reg> filled_region_cords)
{
	seq_pair_a.clear();
	seq_pair_b.clear();
    vector <filled_reg>:: iterator it, it_2;
	int a_xL, b_xL, a_xR, b_xR, a_yU,b_yU, a_yD, b_yD, len_a, mod_cnt=0;
	filled_reg main_list_module,seq_pair_module, RR1;
	it=filled_region_cords.begin();
	RR1= *it;
	seq_pair_a.push_back(RR1);
	seq_pair_b.push_back(RR1);
	vector <filled_reg> seq_pair_temp;
	vector  <filled_reg> :: iterator it_sp;
	cout<<endl<<"0th module: RR"<<RR1.module_num<<endl;
	cout<<endl<<"size of seq pair:  "<<seq_pair_a.size()<<endl;
	for (it_2=filled_region_cords.begin()+1; it_2!=filled_region_cords.end(); ++it_2)
	{
		mod_cnt++;
		main_list_module=*it_2;
		a_xR=main_list_module.x_max;
		a_xL=main_list_module.x_min;
		a_yU=main_list_module.y_min;
		a_yD=main_list_module.y_max;
		len_a=seq_pair_a.size();
		seq_pair_temp=seq_pair_a;
		cout<<endl<<mod_cnt<<"th filled region module: RR"<<main_list_module.module_num<<endl;
		cout<<"-----------------------------------------------------------------------------------"<<endl;
		
		
		for (it_sp=seq_pair_temp.begin(); it_sp!=seq_pair_temp.end(); ++it_sp)
		{
			seq_pair_module=*it_sp;
			b_xR=seq_pair_module.x_max;
			b_xL=seq_pair_module.x_min;
			b_yU=seq_pair_module.y_min;
			b_yD=seq_pair_module.y_max;
			cout<<endl<<"seq pair mod num:  "<<seq_pair_module.module_num<<endl;
			if (a_xL<b_xR && b_yD<a_yU) 
			{
				cout<<"insert left"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_left, 0);
				cout<<" left done"<<endl;
			}
			else
			{
				cout<<"insert right"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_right,0);
				cout<<" right done"<<endl;
			}
			
			if (a_xL<b_xR && a_yD< b_yU )
			{
				cout<<"insert left"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_left, 1);
				cout<<" left done"<<endl;
			}
			else
			{
				cout<<"insert right"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_right,1);
				cout<<" right done"<<endl;
			}
		}
		
		cout<<endl<<"size of seq pair after module "<<mod_cnt<<"= "<<seq_pair_a.size()<<endl;
	}

	
}

void insert_seq(filled_reg main_mod, filled_reg seq_mod, direction dir, int seq_pair_num)
{
	vector <filled_reg> :: iterator it_MM, it_SP;
	int pos_mm=-1, pos_sp=-1;
	
	cout<<" main module postion in sp: "<<pos_mm<<endl;
	cout<<" seq pair module postion in sp: "<<pos_sp<<endl;
	//cout<<endl<<"size of seq pair after module "<<seq_pair_a.size()<<endl;
	
	if(seq_pair_num==0)
	{
		pos_sp= find_pos(seq_pair_a, seq_mod);
		pos_mm= find_pos(seq_pair_a, main_mod);
		if(pos_mm==-1 && dir==dir_left)
		{
			seq_pair_a.insert( seq_pair_a.begin()+pos_sp, main_mod );
		}
	
		else if(pos_mm==-1 && dir==dir_right)
		{
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp+1, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm<pos_sp)
		{
			seq_pair_a=seq_pair_a;
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm>pos_sp)
		{
			seq_pair_a.erase (seq_pair_a.begin()+pos_mm);
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_right && pos_mm>pos_sp)
		{
			seq_pair_a=seq_pair_a;
		}

		else if(pos_mm!=-1 && dir==dir_right && pos_mm<pos_sp)
		{
			seq_pair_a.erase (seq_pair_a.begin()+pos_mm);
			cout<<endl<<"size of seq pair AFTER delete:  "<<seq_pair_a.size()<<endl;
			pos_sp= find_pos(seq_pair_a, seq_mod);
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp+1, main_mod );
		}
	}
	else if(seq_pair_num==1)
	{
		pos_sp= find_pos(seq_pair_b, seq_mod);
		pos_mm= find_pos(seq_pair_b, main_mod);
		if(pos_mm==-1 && dir==dir_left)
		{
			seq_pair_b.insert( seq_pair_b.begin()+pos_sp, main_mod );
		}
	
		else if(pos_mm==-1 && dir==dir_right)
		{
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp+1, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm<pos_sp)
		{
			seq_pair_b=seq_pair_b;
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm>pos_sp)
		{
			seq_pair_b.erase (seq_pair_b.begin()+pos_mm);
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_right && pos_mm>pos_sp)
		{
			seq_pair_b=seq_pair_b;
		}

		else if(pos_mm!=-1 && dir==dir_right && pos_mm<pos_sp)
		{
			seq_pair_b.erase (seq_pair_b.begin()+pos_mm);
			pos_sp= find_pos(seq_pair_b, seq_mod);
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp+1, main_mod );
		}
	}
	
}

int find_pos(vector <filled_reg> seq_pair, filled_reg A)
{
	int pos=-1, cnt=0;
	vector <filled_reg> :: iterator it;
	filled_reg RR1;
	
	for (it=seq_pair.begin(); it!=seq_pair.end(); ++it)
	{
		RR1=*it;
		
		if(RR1.module_num==A.module_num)
		{
			pos=cnt;
			break;
		}
		cnt++;
	}
	return(pos);
		

}

int find_pos_rec(vector <rec_reg> seq_pair, rec_reg A)
{
	int pos=-1, cnt=0;
	vector <rec_reg> :: iterator it;
	rec_reg RR1;
	
	for (it=seq_pair.begin(); it!=seq_pair.end(); ++it)
	{
		RR1=*it;
		
		if(RR1.module_num==A.module_num)
		{
			pos=cnt;
			break;
		}
		cnt++;
	}
	return(pos);

}

void print_seq_pair(vector <filled_reg> seq_pair_a, vector <filled_reg> seq_pair_b)
{
	vector <filled_reg> ::reverse_iterator it_sp;
	filled_reg RR1;
	cout<<endl<<"<";
	for (it_sp=seq_pair_a.rbegin(); it_sp!=seq_pair_a.rend(); ++it_sp)
	{
		RR1=*it_sp;
		cout<<"RR"<<RR1.module_num<<",";
	}
	
	cout<<"> , <";
	
	for (it_sp=seq_pair_b.rbegin(); it_sp!=seq_pair_b.rend(); ++it_sp)
	{
		RR1=*it_sp;
		cout<<"RR"<<RR1.module_num<<",";
	}
	cout<<">"<<endl;
}

  
int calculate_area(int ROWS, int COLUMS)
{
	vector<int> x_cor;
	vector<int> y_cor;
	vector<int>::const_iterator it2; 
	int x_max, y_max, x_min, y_min, area;
	
	for( int i=0; i<COLUMS;i++)
	{
		for(int j=0;j<ROWS;j++)
		{
			if (my_fpga[j][i].module_num>-1)
			{
				x_cor.push_back(i);
				y_cor.push_back(j);
			}
		}
	}
	
	it2 = max_element(x_cor.begin(), x_cor.end());
	x_max=*it2;
	it2 = min_element(x_cor.begin(), x_cor.end()) ;
	x_min=*it2;
	it2 = max_element(y_cor.begin(), y_cor.end());
	y_max=*it2;
	it2 = min_element(y_cor.begin(), y_cor.end());
	y_min=*it2;
	area= abs(x_max-x_min)*abs(y_max-y_min);
	cout<<endl<<" extreme boundaries of the design: ("<<x_min<<","<<y_min<<"),("<<x_max<<", "<<y_max<<")"<<endl;
	return(area);
}
  
int calculate_wasted_res(vector <filled_reg> filled_region_cords)
{
	vector <filled_reg>:: iterator it;
	int clb_cost=1, bram_cost=9, dsp_cost=19, clb_wasted=0, bram_wasted=0, dsp_wasted=0, rw_cost=0, cnt=0;
	filled_reg RR1;
	wasted_rec_fie<<endl<<" rw_cost "<<rw_cost;
	for (it=filled_region_cords.begin()+1; it!=filled_region_cords.end(); ++it)
	{
		RR1=*it;
		cnt++;
		cout<<endl<<" filled region number "<<"  "<<RR1.module_num<<endl;
		wasted_rec_fie<<endl<<" filled region number "<<"  "<<RR1.module_num<<endl;
		clb_wasted=0, bram_wasted=0, dsp_wasted=0;
		for(unsigned int j=RR1.x_min; j<=RR1.x_max; j++)
		{
			for(unsigned int i=RR1.y_min; i<=RR1.y_max; i++)
			{
				if (my_fpga[i][j].usage==false)
				{
					if(my_fpga[i][j].rec_type==rec_clb )
					{
						cout<<endl<<" clb i= "<<i<<"  j="<<j;
						wasted_rec_fie<<endl<<" clb i= "<<i<<"  j="<<j;
						clb_wasted++;
					}
					else if(my_fpga[i][j].rec_type==rec_bram )
					{
						cout<<endl<<" bram i= "<<i<<"  j="<<j;
						wasted_rec_fie<<endl<<" bram i= "<<i<<"  j="<<j;
						bram_wasted++;
					}
					else if(my_fpga[i][j].rec_type==rec_dsp )
					{
						cout<<endl<<" dsp i= "<<i<<"  j="<<j;
						wasted_rec_fie<<endl<<" dsp i= "<<i<<"  j="<<j;
						dsp_wasted++;
					}
				}
				
			}
		}
		cout<<endl<<" Module "<<RR1.module_num<<"("<<RR1.x_min<<","<<RR1.y_min<<"),("<<RR1.x_max<<", "<<RR1.y_max<<") clb wasted:  "<<clb_wasted<<" bram wasted: "<<bram_wasted<<" dsp wasted: "<<dsp_wasted<<endl;
		rw_cost=rw_cost+clb_cost*clb_wasted + bram_cost*bram_wasted + dsp_cost*dsp_wasted;
	}
	outputfile<<endl<<"rw cost: "<<rw_cost;
	return(rw_cost);
	
}

pair <int,int>  calculate_wirelength(vector <rec_reg> rec_region_list)
{
	vector <rec_reg> :: iterator it_1;
	vector <rec_reg> :: iterator it_2;
	pair <int,int> length_pair ;
	int x_dist, y_dist, man_dist,  total_length=0, num_wires, weigthed_manh_dist, weig_tot_len=0;
	rec_reg RR1, RR2;

	for (it_1=rec_region_list.begin()+1; it_1!=rec_region_list.end(); ++it_1)
	{
		RR1=*it_1;
		
		if(RR1.dsp==0 && RR1.bram==0)
		{
			cout<<endl<<" RR1 module number   pqrst: "<<RR1.module_num<<"  module name: "<<RR1.name<<endl;
			continue;
		}
		for (it_2=rec_region_list.begin(); it_2!=rec_region_list.end()-1; ++it_2)
		{
			RR2=*it_2;
			if(RR2.dsp==0 && RR2.bram==0)
			{
			cout<<endl<<" RR2 module number abcde: "<<RR1.module_num<<"  module name: "<<RR1.name<<endl;
			continue;
			}
			if(RR1.module_num> RR2.module_num)
			{
			x_dist=abs(RR1.centroid[1]-RR2.centroid[1]);
			y_dist=abs(RR1.centroid[0]-RR2.centroid[0]);
			man_dist=x_dist+y_dist;
			total_length=total_length+man_dist;
			num_wires=RR1.map_wires.find(RR2.module_num)->second;
			weigthed_manh_dist=num_wires*man_dist;
			weig_tot_len= weig_tot_len+weigthed_manh_dist;
			length_pair = make_pair(total_length, weig_tot_len);
			cout<<endl<<"distance between "<<RR1.name<<" and  "<<RR2.name<<"= "<<man_dist<<" number of wires= "<<num_wires<<" weighted wires= "<<weigthed_manh_dist;
			}
		}
	}
	cout<<endl<<"weighted total ength= "<<weig_tot_len<<endl;
	return(length_pair);
	
}
 void priority_sort(vector <rec_reg> rec_region_list)
 {
	vector <rec_reg> L1;
	vector <rec_reg> L2;
	vector <rec_reg> L3;
	vector <rec_reg> L4;
	vector <rec_reg> list;
	vector <rec_reg> ::iterator it;
	map <int, rec_reg> map_temp;
	map <int, rec_reg> :: reverse_iterator it_m;
	rec_reg RR1;
	int i=0;
	
	for (it=rec_region_list.begin(); it!=rec_region_list.end(); ++it)
	{
		RR1=*it;
		cout<<endl<<"RR "<<i+1<<"   CLB: "<<RR1.clb<<"  BRAM: "<<RR1.bram<<"  DSP:  "<<RR1.dsp<<"   module number: "<<RR1.module_num;
		if(RR1.dsp>0 && RR1.bram>0)
		{
			L1.push_back(RR1);
		}
		else if(RR1.dsp>0 && RR1.bram==0)
		{
			L2.push_back(RR1);
		}
		else if(RR1.dsp==0 && RR1.bram>0)
		{
			L3.push_back(RR1);
		}
		else if(RR1.dsp==0 && RR1.bram==0)
		{
			L4.push_back(RR1);
			only_clb_list.push_back(RR1);
		}
		i++;
	}
	
	cout<<endl;
	
	 sort_rr_list(L1,  rec_dsp );
	 sort_rr_list(L2, rec_dsp );
	 sort_rr_list(L3, rec_bram );
	 //sort_rr_list(L4, rec_clb );
	 
 }
 
 void sort_rr_list(vector <rec_reg> L1,  resource res_type )
 {
	vector <rec_reg> ::iterator it;
	multimap <int, rec_reg> map_temp;
	multimap <int, rec_reg> :: reverse_iterator it_m;
	
	 for (it=L1.begin(); it!=L1.end(); ++it)
	{
		int key;
		rec_reg value;
		value=*it;
		if(res_type==rec_dsp)
		{
			key=value.dsp;
		}
		else if(res_type==rec_bram)
		{
			key=value.bram;
		}
		
		else if(res_type==rec_clb)
		{
			key=value.clb;
		}
		
		map_temp.insert ( std::pair<int,rec_reg>(key,value) );
	}
	
	for (it_m=map_temp.rbegin(); it_m!=map_temp.rend(); ++it_m)
	{
		rec_reg RR2;
		RR2= it_m->second;
		sorted_region_list.push_back(RR2);
		//cout<<endl<<"dsp:= "<<it_m->first <<"  "<<RR2.name<<" RR module number: "<<RR2.module_num;
		cout<<endl<<"RR name  "<<RR2.name<<" RR module number: "<<RR2.module_num<<"   RR CLB: "<<RR2.clb<<"  RR BRAM: "<<RR2.bram<<"   RR DSP: "<<RR2.dsp;
		cout<<" sorted_region_list size: "<<sorted_region_list.size();
		
	}
	
	cout<<endl;
 }
  

white_space calculate_ws_cordinate(int ROWS, int COLUMS,  int module_num)
{
	vector<int> x_cor;
	vector<int> y_cor;
	vector<int>::const_iterator it2;  
	white_space static_mod;
	int x_max, y_max, x_min, y_min;
	for( int i=0; i<COLUMS;i++)
	{
		for(int j=0;j<ROWS;j++)
		{
			if (my_fpga[j][i].usage==false && my_fpga[j][i].module_num==module_num)
			{
				x_cor.push_back(i);
				y_cor.push_back(j);
			}
		}
	}
	
	it2 = max_element(x_cor.begin(), x_cor.end());
	x_max=*it2;
	it2 = min_element(x_cor.begin(), x_cor.end());
	x_min=*it2;
	it2 = max_element(y_cor.begin(), y_cor.end());
	y_max=*it2;
	it2 = min_element(y_cor.begin(), y_cor.end());
	y_min=*it2;
	
	int clb_count=0, dsp_count=0, bram_count=0;
	for( int i=x_min; i<=x_max;i++)
	{
		for(int j=y_min;j<=y_max;j++)
		{
			if (my_fpga[j][i]. rec_type==rec_clb)
			{
				clb_count++;
			}
			else if (my_fpga[j][i]. rec_type==rec_bram)
			{
				bram_count++;
			}
			else if (my_fpga[j][i]. rec_type==rec_dsp)
			{
				dsp_count++;
			}
		}
	}
	
	   
	static_mod.x_min=x_min; 
	static_mod.y_min=y_min;
	static_mod.x_max=x_max;
	static_mod.y_max=y_max;
	static_mod.module_num=module_num;
	static_mod.clb_free=clb_count;
	static_mod.bram_free=bram_count;
	static_mod.dsp_free=dsp_count;
	return(static_mod);
}



filled_reg calculate_module_cordinate(int ROWS, int COLUMS,  int module_num, bool replcmt)
{
	vector<int> x_cor;
	vector<int> y_cor;
	vector<int>::const_iterator it2;  
	filled_reg static_reg;
	int x_max, y_max, x_min, y_min, centroid[2];

	for( int i=0; i<COLUMS;i++)
	{
		for(int j=0;j<ROWS;j++)
		{
			if (my_fpga[j][i].usage==true && my_fpga[j][i].module_num==module_num)
			{
				x_cor.push_back(i);
				y_cor.push_back(j);
			}
		}
	}
	
	it2 = max_element(x_cor.begin(), x_cor.end());
	x_max=*it2;
	it2 = min_element(x_cor.begin(), x_cor.end());
	x_min=*it2;
	it2 = max_element(y_cor.begin(), y_cor.end());
	y_max=*it2;
	it2 = min_element(y_cor.begin(), y_cor.end());
	y_min=*it2;
	   
	static_mod.X_TL=x_min; 
	static_mod.Y_TL=y_min;
	static_mod.X_BR=x_max;
	static_mod.Y_BR=y_max;
	
	static_reg.x_min=x_min;
	static_reg.y_min=y_min;
	static_reg.x_max=x_max;
	static_reg.y_max=y_max;
	static_reg.module_num=module_num;
	centroid[1]=(x_max+x_min)/2;
	centroid[0]=(y_max+y_min)/2;
	static_reg.centroid[0]=centroid[0];
	static_reg.centroid[1]=centroid[1];
	for ( int i=x_min; i<=x_max;i++)
	{
		for( int j=y_min; j<=y_max;j++)
		{
			my_fpga[j][i].module_num=module_num;
		}
	}
	
	if (replcmt==false)
	{
		filled_region_cords.push_back(static_reg);
	}
	else if (replcmt==true)
	{
		filled_region_cords[module_num].x_min=x_min;
		filled_region_cords[module_num].y_min=y_min;
		filled_region_cords[module_num].x_max=x_max;
		filled_region_cords[module_num].y_max=y_max;
		filled_region_cords[module_num].module_num=module_num;
		filled_region_cords[module_num].centroid[0]=centroid[0];
		filled_region_cords[module_num].centroid[1]=centroid[1];
	}
		
	if(module_num>0)
	{
	rec_region_list[module_num-1].centroid[0]=centroid[0];
	rec_region_list[module_num-1].centroid[1]=centroid[1];
	}
	
	
	cout <<endl<< "module "<<module_num<<"  cordinates: (" << static_mod.X_TL<<","<<static_mod.Y_TL<<"),("<<static_mod.X_BR<<","<<static_mod.Y_BR<<")"<<"   centoid ("<<static_reg.centroid[1]<<
																","<<static_reg.centroid[0]<<")"<<endl;
	 
	outputfile <<endl<< "module "<<module_num<<"  cordinates: (" << static_mod.X_TL<<","<<static_mod.Y_TL<<"),("<<static_mod.X_BR<<","<<static_mod.Y_BR<<")"<<"   centoid ("<<static_reg.centroid[1]<<
																","<<static_reg.centroid[0]<<")"<<endl;
	
	/*cout<<endl<<" RR1 details clb: "<<rec_region_list[0].clb<<"  bram: "<<rec_region_list[0].bram<<" dsp: "<<rec_region_list[0].dsp<<" centroid: ("<<rec_region_list[0].centroid[1]<<","<<rec_region_list[0].centroid[0]<<")"<<endl;
	cout<<endl<<" RR2 details clb: "<<rec_region_list[1].clb<<"  bram: "<<rec_region_list[1].bram<<" dsp: "<<rec_region_list[1].dsp<<" centroid: ("<<rec_region_list[1].centroid[1]<<","<<rec_region_list[1].centroid[0]<<")"<<endl;
	cout<<endl<<" RR3 details clb: "<<rec_region_list[2].clb<<"  bram: "<<rec_region_list[2].bram<<" dsp: "<<rec_region_list[2].dsp<<" centroid: ("<<rec_region_list[2].centroid[1]<<","<<rec_region_list[2].centroid[0]<<")"<<endl;
	cout<<endl<<" RR4 details clb: "<<rec_region_list[3].clb<<"  bram: "<<rec_region_list[3].bram<<" dsp: "<<rec_region_list[3].dsp<<" centroid: ("<<rec_region_list[3].centroid[1]<<","<<rec_region_list[3].centroid[0]<<")"<<endl;*/
	return(static_reg);
	 
	
}

void print_module_list(vector <rec_reg> rec_region_list)
{
	vector <rec_reg> :: iterator it;
	map<int,int>::iterator it1;
	int rr_num=1;
	rec_reg RR1;
	
	for (it=rec_region_list.begin(); it!=rec_region_list.end(); ++it)
	{
		RR1=*it;
		cout<<endl<<"number "<<rr_num;
		cout<<"  "<<RR1.name<<" CLB: "<<RR1.clb<<" BRAM: "<<RR1.bram<<" DSP: "<<RR1.dsp;
		 for (it1=RR1.map_wires.begin(); it1!=RR1.map_wires.end(); ++it1)
		 {
         cout <<"  RR"<< it1->first << " => " << it1->second ;
		 }
		
		rr_num++;
		
		
	}	
	cout<<endl;
	
}

void read_rec_module(string myfile_name)
{
	string line;
	int line_cnt=0;
	int mod_count=0;
	map < int, int> mymap;
	int num_clb, num_bram, num_dsp;
	rec_reg RR1;
	ifstream module_file;
	int clb_all_rr=0,bram_all_rr=0,dsp_all_rr=0, module_num=0;
    module_file.open(myfile_name.c_str(), ios::out);
	
	if (module_file.is_open())
    {
    
        while ( getline (module_file,line) )
        {
	 		std::istringstream iss(line); 
			string res, clbnameis;
            int  num;
			
			line_cnt++;
			
			if(line_cnt==2)
			{
				iss >> res >>num;
				num_modules=num;
				
			}
			
			
			else if (line_cnt>2 && line_cnt<(3+num_modules))
			{
			module_num++;
			iss >> res >> num_clb>>num_bram>>num_dsp; 
			RR1.name=res;
			RR1.clb=num_clb;
			RR1.bram=num_bram;
			RR1.dsp=num_dsp;
			RR1.module_num=module_num;
			clb_all_rr=clb_all_rr+RR1.clb;
			bram_all_rr=bram_all_rr+RR1.bram;
			dsp_all_rr=dsp_all_rr+RR1.dsp;
			rec_region_list.push_back(RR1);
			}
			
			else if(line_cnt>10 && line_cnt<16)
			{
				int rr[num_modules];
				iss>>res>>rr[0]>>rr[1]>>rr[2]>>rr[3]>>rr[4];
				
				//cout<<endl<<" "<<rr[0]<<" "<<rr[1]<<" "<<rr[2]<<" "<<rr[3]<<endl;
				mymap.clear();
				for(unsigned int i=0; i<num_modules;i++)
				{
					mymap.insert ( std::pair<int,int>(i+1,rr[i]) );
				}
				rec_region_list[mod_count].map_wires=mymap;
				mod_count++;
			
				
			}
		}
	}
	module_file.close();
	
	cout<<"\n clb in RR: "<<clb_all_rr<<" bram all rr: "<<bram_all_rr<<" dsp all rr:  "<<dsp_all_rr<<endl;
	
	
			
}

void read_row_column(string myfile_name) // int ROWS, int COLUMN)
{
	string line;
	int line_cnt=0;
	int col_count=0;
	ifstream myfile;
    myfile.open(myfile_name.c_str(), ios::out);
	
	
	if (myfile.is_open())
    {
		 cout<<"inside file open"<<endl;
        while ( getline (myfile,line) )
        {
			// cout<<"inside while "<<endl;
			std::istringstream iss(line); 
			string res, clbnameis;
            int  num;
			line_cnt++;
			
			if (line_cnt==1)
			{
				iss >> res >> num;
				ROWS=num;
				cout<<"rows  "<<ROWS<<endl;
				
				
			}
			
			else if (line_cnt==2)
			{
				iss >> res >> num;
				COLUMN=num;
				
				cout<<"Columns   "<<COLUMN<<"\n"; 
			}
		}
	}
	
	myfile.close();
}

void load_fpga(string myfile_name, int ROWS)
{
	string line;
	int line_cnt;
	int col_count=0;
	ifstream myfile1;
    myfile1.open(myfile_name.c_str(), ios::out);
	total_clb=0;
	total_bram=0;
	total_dsp=0;
	if (myfile1.is_open())
	{	
	while ( getline (myfile1,line) )
        {
			std::istringstream iss(line); 
			string res, clbnameis;
            int  num;
			
			line_cnt++;
		if (line_cnt>5)
			{
				
				
			
			iss >> res >> num;  
			
				if (res=="clb")
				{
					
					for (unsigned int i=0;i<num; i++)
					{
						clb cb[ROWS];
						clb_col.push_back(col_count);
						
						for (unsigned int j=0; j<ROWS; j++)
						{
						cb[j].row=j;
						cb[j].usage=false;
						cb[j].column=col_count;
						my_fpga[j][col_count].rec_type=rec_clb;
						my_fpga[j][col_count].rec_name="clb";
						my_fpga[j][col_count].module_num=-1;
						clb_vec.push_back(cb[j]);
						total_clb++;
						}
						col_count++;
						
					}
				}					
					
				else if (res=="bram")
				{
					
					for (unsigned int i=0;i<num; i++)
					{
						bram bm[ROWS];
						bram_col.push_back(col_count);
						for (unsigned int j=0; j<ROWS; j++)
						{
						bm[j].row=j;
						bm[j].usage=false;
						bm[j].column=col_count;
						my_fpga[j][col_count].rec_type=rec_bram;
						my_fpga[j][col_count].rec_name="bram";
						my_fpga[j][col_count].module_num=-1;
						bram_vec.push_back(bm[j]);
						total_bram++;
						}
						col_count++;
						
					}	
				}
				
				else if (res=="dsp")
				{
					
					for (unsigned int i=0;i<num; i++)
					{
						dsp dp[ROWS];
						dsp_col.push_back(col_count);
						for (unsigned int j=0; j<ROWS; j++)
						{
						dp[j].row=j;
						dp[j].usage=false;
						dp[j].column=col_count;
						my_fpga[j][col_count].rec_type=rec_dsp;
						my_fpga[j][col_count].rec_name="dsp";
						my_fpga[j][col_count].module_num=-1;
						dsp_vec.push_back(dp[j]);
						total_dsp++;
						}
						col_count++;
						
					}	
				}
			}
			
		}
	}
		
		myfile1.close();
}

void print_fpga(int ROWS, int COLUMS)
{
	cout<<endl;
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			if (my_fpga[j][i].rec_type==rec_clb)
			{
				cout<<" clb"<<i<<","<<j;
				
			}
		
			else if (my_fpga[j][i].rec_type==rec_bram)
			{
				cout<<"bram"<<i<<","<<j;
			}
		
			else
			{
				cout<<"dsp"<<i<<","<<j;
			
			}
			if (my_fpga[j][i].usage==true)
			{
				cout<<"L|";
			}
				
			else if (my_fpga[j][i].usage==false)
			{
				cout<<"F|";
			}
		}
		
		
		
		cout<<endl;
	}
}
 
rec_reg read_static_module (string filename)
{
  
  //cout<< "inside function"<<endl;
	
	string line;
	rec_reg RR1;
	int line_cnt=0;
	
	ifstream module_file;
    module_file.open(filename.c_str(), ios::out);
	
	//outputfile.open (filenameis.c_str(), ios::out);
	if (module_file.is_open())
    {
    //  cout<<endl<<"inside if "<<endl;
        while ( getline (module_file,line) )
        {
	 // cout<<endl<<"inside while "<<endl; 
			std::istringstream iss(line); 
			string res, clbnameis;
            int  num;
			
			line_cnt++;
			
			
			if (line_cnt>1)
			{
			iss >> res >> num; 
		//	cout<<"RES ="<<res << "  NUM= "<<num<<endl;
			
				if (res=="clb")
				{
					RR1.clb=num;
					//cout<<endl<<"clb= "<<num<<endl;
				}					
					
				else if (res=="bram")
				{
					RR1.bram=num;
				//	cout<<"bram =" <<num<<endl;
				}
				
				else if (res=="dsp")
				{
					RR1.dsp=num;
				//	cout<<"dsp= "<<num<<endl;
				}
			}
			
		}
		module_file.close();
	}

	else
	{
		cout<<"file cant find"<<endl;
	}
		return(RR1);
}

void allocate_recon_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num )
{
	int clb_cnt, bram_cnt, dsp_cnt, dsp_col_num, bram_col_num, clb_b_dsp_bram, y_min_module=0, row_num=0, first_ele, bram_colm_1, bram_colm_2, dsp_alloted=0, bram_alloted=0;;
	static int RR_count=0;
	cout<<endl<<"static int RR_count:  "<<RR_count<<endl;
	clb_cnt= RR1.clb;
	bram_cnt= RR1.bram;
	dsp_cnt=RR1.dsp;
	filled_reg filled_region;
	vector <filled_reg>::iterator it_f;
	vector <int> :: iterator it;
	map <int, int> dist_vec_map;
	map <int, int>:: iterator itm;
	map <int, int>:: reverse_iterator rit;
	dist_vec_map.clear();
	it=dsp_col.begin();
	dsp_col_num=*it;
	
	for (it=bram_col.begin(); it!=bram_col.end(); ++it)
	{
		int dist=abs(dsp_col_num-*it);
		cout<<endl<<"dsp col:=  "<<dsp_col_num<<"  bram col num: "<<*it<<"  distance: "<<dist;
		dist_vec_map.insert ( std::pair<int,int>(dist,*it) );
	
	}
				
				
	itm=dist_vec_map.begin();
	rit=dist_vec_map.rbegin();
				
	bram_colm_1=(*itm).second;
	bram_colm_2=(*rit).second;
	cout<<endl<<"clb_count: "<<clb_cnt<<"   bram count: "<<bram_cnt<<"  dsp count: "<<dsp_cnt<<endl;

//////****************************************** DSP ALLOCATION  FOR RECONFIGURABLE REGION*********************************////
	if(dsp_cnt>0)
	{
		it=dsp_col.begin();
		dsp_col_num=*it;
		it_f=filled_region_cords.begin();
		
		
		for(it_f=filled_region_cords.begin(); it_f!=filled_region_cords.end(); ++it_f)
		{
			filled_region=*it_f;
			if(dsp_col_num>=filled_region.x_min && dsp_col_num<=filled_region.x_max )
			{
				row_num=filled_region.y_max+1;
				y_min_module=row_num;
				
				if(row_num>=filled_region.y_max)
				{
					row_num=row_num;
					y_min_module=row_num;
				}
				else
				{
					row_num=filled_region.y_max+1;
					y_min_module=row_num;
				}
			}
			
			else if(dsp_col_num<filled_region.x_min || dsp_col_num>filled_region.x_max )
			{
				row_num=row_num;
				y_min_module=row_num;
			}
		}
		
		
	if(bram_cnt>0)
	{
		cout<<endl<<" ---------here bram cnt >0  --------------"<<endl;
bram_find: if(my_fpga[y_min_module][bram_colm_1].usage==true)
			{
				cout<<endl<<"my_fpga[y_min_module][bram_colm_1].usage==true"<<endl;
				if(my_fpga[y_min_module][bram_colm_2].usage==true)
				{
					cout<<endl<<"my_fpga[y_min_module][bram_colm_2].usage==true"<<endl;
					y_min_module++;
					row_num=y_min_module;
					goto bram_find;
							
				}
						
				else if(my_fpga[y_min_module][bram_colm_2].usage==false)
				{
					cout<<endl<<"my_fpga[y_min_module][bram_colm_2].usage==false"<<endl;
					first_ele=bram_colm_2;
							
				}
			}
					
			else if (my_fpga[y_min_module][bram_colm_1].usage==false)
				{
					cout<<endl<<"my_fpga[y_min_module][bram_colm_1].usage==false"<<endl;
					first_ele=bram_colm_1;
				}
	}
	
		 cout<<endl<<" first_ele:  "<<first_ele<<" colum left:  "<<bram_colm_2<<" colum right trtt:  "<<bram_colm_1<<endl;
		filled_region=*it_f;
				
		cout<<endl;
		for(unsigned int i=y_min_module; i<y_min_module+dsp_cnt; i++)   
		{
			dsp_alloted++;
			cout<<"dsp allotted: "<<dsp_alloted<<" i= "<<i<<"  j= "<<dsp_col_num<<" module num:  "<<module_num<<endl;
			my_fpga[i][dsp_col_num].usage=true;   
			my_fpga[i][dsp_col_num].module_num =module_num;   
		
		}    
		
	}
	else if (dsp_cnt==0)
	{
		dsp_col_num=0;
	}
	
	//////****************************************** BRAM ALLOCATION  FOR RECONFIGURABLE REGION*********************************////
	
	if(bram_cnt>0)
	{	
		it=bram_col.begin(); 
		
		if (dsp_cnt>0)
		{
			
		
			cout << "\n [" << (*itm).first << ':' << (*itm).second << ']';
			cout << "\n [" << (*rit).first << ':' << (*rit).second << ']';
			cout<<endl;
			 
			 cout<<endl<<"first element: "<<first_ele<<endl;
		
				for(unsigned int i=y_min_module; i<y_min_module+bram_cnt; i++)
				{
					bram_alloted++;
					cout<<"bram allotted: "<<bram_alloted<<" i= "<<i<<"  j= "<<first_ele<<" module num:  "<<module_num<<endl;
					my_fpga[i][first_ele].usage=true;   
					my_fpga[i][first_ele].module_num =module_num ;  
				}
			
		bram_col_num=first_ele;
		}
		
		else if(dsp_cnt==0)
		{
			int row_prev=0;
			int row_num=0; 
			int count=0;
			for (it=bram_col.begin(); it!=bram_col.end(); ++it)
			{
				cout<<endl<<"bram column number:  "<<*it<<endl;
				count++;
				for(it_f=filled_region_cords.begin(); it_f!=filled_region_cords.end(); ++it_f)
				{
					filled_region=*it_f;
				
					if(*it>=filled_region.x_min && *it<=filled_region.x_max )
					{
						row_num=filled_region.y_max+1;
						y_min_module=row_num;
	
						if(row_num>=filled_region.y_max)
						{
							row_num=row_num;
							y_min_module=row_num;
							first_ele=*it;
						}
						
						else
						{
							row_num=filled_region.y_max+1;
							y_min_module=row_num;
							first_ele=*it;
						}
					}
			
					else if(*it<filled_region.x_min || *it>filled_region.x_max )
					{
						row_num=row_num;
						y_min_module=row_num;
					}
					
				}
				
				if (count==1)
				{
				  row_prev=row_num;
				}
				
				else
				{
				
				  if(row_prev<=row_num)
				  {	
					cout<<endl<<"row_prev<=row_num "<<" row_prev: "<<row_prev<<"  row_num: "<<row_num<<endl;
					first_ele=bram_col[0];
					row_prev=row_prev;
					y_min_module=row_prev;
				  }
					
				else if(row_prev>row_num)
				  {
					cout<<endl<<"row_prev>row_num"<<endl;
					first_ele=*it;
					row_prev=row_num;
					y_min_module=row_prev;
				  }
				}
		
			}
			
			bram_col_num=first_ele;
			
			cout<<endl<<"-----bram_col_num=first_ele:   "<<first_ele<<" y_min_module:  "<<y_min_module<<endl;    
			for(unsigned int i=y_min_module; i<y_min_module+bram_cnt; i++)
			{
				bram_alloted++;
				cout<<"bram allotted: "<<bram_alloted<<" i= "<<i<<"  j= "<<first_ele<<" module num:  "<<module_num<<endl;
				my_fpga[i][first_ele].usage=true;   
				my_fpga[i][first_ele].module_num =module_num ;  
			}
			
		}
	}
	
	else if (bram_cnt==0)
	{
		bram_col_num=0;
	}
	
	//////****************************************** CLB ALLOCATION  FOR RECONFIGURABLE REGION*********************************////
	if(clb_cnt>0)   
	{
		bool locked_rec_found=false;
		bool right_edge_found=false;
		int clb_alloted=0;
		if(dsp_cnt>0 && bram_cnt>0)
		{
			clb_b_dsp_bram=abs(dsp_col_num-bram_col_num);
			cout<<"\n dsp count: "<<dsp_cnt<<" bram cnt : "<<bram_cnt<<endl;
			cout<<"number of clbs between bram & dsp cols: "<<(clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))<<endl;
	
			if(clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))))
			{   
				int source=0, dest=0;
				
				if(dsp_col_num<bram_col_num)
				{
					dest=bram_col_num;
					source=dsp_col_num+1;
				}
				else if(bram_col_num<dsp_col_num)
				{
					source=bram_col_num+1;
					dest=dsp_col_num;
				}
				for(unsigned int i=source; i<dest; i++)
				{
					for(unsigned int j= y_min_module; j< y_min_module+(clb_cnt/(clb_b_dsp_bram-1)); j++)
					{
						clb_alloted++;
						my_fpga[j][i].usage=true;
						my_fpga[j][i].module_num =module_num ; 
					}
				}
				int final_row=(clb_cnt/(clb_b_dsp_bram-1));
				int final_row_clbs=(clb_cnt)%(clb_b_dsp_bram-1);
		
				for(unsigned int n=source; n<(source+final_row_clbs); n++)
				{
					clb_alloted++;
					my_fpga[final_row][n].usage=true;
					my_fpga[final_row][n].module_num =module_num ; 
				}
			}
		
			else if(clb_cnt>((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))))		
			{
				int source=0, dest=0;
				
				if(dsp_col_num<bram_col_num)
				{
					dest=bram_col_num;
					source=dsp_col_num+1;
				}
				else if(bram_col_num<dsp_col_num)
				{
					source=bram_col_num+1;
					dest=dsp_col_num;
				}
				for(unsigned int i=source; i<dest; i++)
				{ 
					for(unsigned int j= y_min_module; j<y_min_module+max(bram_cnt, dsp_cnt); j++)
					{
						clb_alloted++;
						my_fpga[j][i].usage=true; 
						my_fpga[j][i].module_num =module_num ; 
					}
				}
		
				int clb_left= clb_cnt-((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt)));
				double num_cols_req= ((clb_left)/max(bram_cnt, dsp_cnt));
				int num_cols_reqd= (int)num_cols_req+1;
				int clb_last_col= (clb_left)%(max(bram_cnt, dsp_cnt));
		
				cout<<" number of colums reqd: "<<num_cols_reqd<<"  dsp column no: "<<dsp_col_num<<" last colum cordinate:" <<(dsp_col_num+1+num_cols_reqd)<<" clb left: "<<clb_left<<"  "<<ceil(1/3)<<endl;
		
				if(dsp_col_num>bram_col_num)                ////**** dsp column> bram column ************//////
				{
					for(unsigned int n=dsp_col_num+1; n<(dsp_col_num+1+num_cols_reqd); n++)
					{
						if(my_fpga[y_min_module][n].usage==false)
						{
							for(unsigned int j= y_min_module; j<(max(bram_cnt, dsp_cnt)); j++)
							{
								clb_alloted++;
								my_fpga[j][n].usage=true; 
								my_fpga[j][n].module_num =module_num ;  
							}
						}
						
						else if(my_fpga[y_min_module][n].usage==true)
						{
							locked_rec_found=true;
							break;
						}
						
						if(n==COLUMNS)
						{
							right_edge_found=true;
							cout<<endl<<" right_edge_found "<<endl;
							break;
						}
					}
					
					if(my_fpga[y_min_module][dsp_col_num+num_cols_reqd+1].usage==false)
					{
						for(unsigned int j= y_min_module; j< y_min_module+clb_last_col; j++)
						{
							clb_alloted++;
							my_fpga[j][dsp_col_num+num_cols_reqd+1].usage=true; 
							my_fpga[j][dsp_col_num+num_cols_reqd+1].module_num =module_num ; 
						}
						
						
					}
					
				}
		
		
				else if(bram_col_num>dsp_col_num)              ///************bram col > clb column*********************///
				{
					cout<<endl<<"y min module "<<y_min_module<<"///************bram col > clb column***************  num_cols_reqd: "<<num_cols_reqd<<endl;
					int clb_remain=0; 
					bool extend_vertically= false;
					for(unsigned int n=bram_col_num+1; n<bram_col_num+1+num_cols_reqd; n++)
					{
						cout<<endl<<" num_cols_reqd:  "<<num_cols_reqd<<endl;
						if(n==COLUMNS)
						{
							clb_remain=clb_cnt-clb_alloted;
							right_edge_found=true;
							
							break;
						}
						
						if(clb_alloted==clb_cnt)
						{
							break;
						}
						
						if(my_fpga[y_min_module][n].usage==false)
						{
							for(unsigned int j= y_min_module; j<y_min_module+(max(bram_cnt, dsp_cnt)); j++)
							{
								clb_alloted++;
								cout<<endl<<" clb allotted: "<<clb_alloted<<endl;
								my_fpga[j][n].usage=true; 
								my_fpga[j][n].module_num =module_num ; 
								if(clb_alloted==clb_cnt)
								{
									break;
								}
							}
						}
						
						
						else if(my_fpga[y_min_module][n].usage==true)
						{
							locked_rec_found=true;
							break;
						}
						
						
					}
					 
				
					if(my_fpga[y_min_module][dsp_col_num+num_cols_reqd+1].usage==false && right_edge_found==false && (dsp_col_num+num_cols_reqd+1)<23)
					{
						for(unsigned int j= y_min_module; j<y_min_module+clb_last_col; j++)
						{
							my_fpga[j][bram_col_num+num_cols_reqd+1].usage=true; 
							my_fpga[j][bram_col_num+num_cols_reqd+1].module_num =module_num ; 
						}
					}
					
					if(right_edge_found== true)
					{
						cout<<endl<<" right_edge_found  true "<<endl;
						int clb_left= clb_remain;
						int num_cols_reqd= (clb_left)/max(bram_cnt, dsp_cnt);
						int clb_last_col= (clb_left)%(max(bram_cnt, dsp_cnt));
						cout<<endl<<"(1)dsp_col_num-1: "<<dsp_col_num-1<<" (2)clb_left: "<<clb_left<<" (3)num_cols_reqd: "<<num_cols_reqd<<" (4)clb_last_col: "<<clb_last_col<<endl;
						
						for(unsigned int n=dsp_col_num-1; n>(dsp_col_num-1-num_cols_reqd); n--)
						{
							cout<<endl<<"colum number dsp::  "<<n<<endl;
							
							if(my_fpga[y_min_module][n].usage==false)
							{
								for(unsigned int j= y_min_module; j<y_min_module+(max(bram_cnt, dsp_cnt)); j++)
								{
									clb_alloted++;
									my_fpga[j][n].usage=true; 
									my_fpga[j][n].module_num =module_num ; 
								}
							}
							
							else if (my_fpga[y_min_module][n].usage==true)
							{
								extend_vertically=true;
								break;
							}
						}
						
						if(my_fpga[y_min_module][dsp_col_num-num_cols_reqd-1].usage==false)
						{
							cout<<endl<<"column number :  "<<dsp_col_num-num_cols_reqd-1<<endl;
							for(unsigned int j= y_min_module; j< y_min_module+clb_last_col; j++)
							{
								clb_alloted++;
								my_fpga[j][dsp_col_num-num_cols_reqd-1].usage=true; 
								my_fpga[j][dsp_col_num-num_cols_reqd-1].module_num =module_num ; 
							}
							cout<<endl<<"******** C L B COUNT ****   "<<clb_alloted<<endl;
						}
						
						
					}
				}
			}
			
			cout<<endl<<" ------------------------- module number "<<module_num<< " --------------------------------cccccccccc llllllllllll bbbbbbbbbbbbb alloted: "  <<clb_alloted<<endl;
		}

		else if(dsp_cnt>0 && bram_cnt==0)
		{
			int num_cols_reqd= clb_cnt/dsp_cnt;
			int clb_last_col=clb_cnt%dsp_cnt;
			bool bram_col_found= false;
			
			int col_count=0; 
			int clb_placed=0;
			int clb_left=0;
			
			clb_allot_dsp_or_bram_g_t_0(clb_cnt, dsp_cnt, clb_alloted, rec_dsp, rec_bram, y_min_module, bram_cnt, right_edge_found, dsp_col_num, module_num );

			
		}
		
		else if(bram_cnt>0 && dsp_cnt==0)
		{
			int num_cols_reqd= clb_cnt/bram_cnt;
			int clb_last_col=clb_cnt%bram_cnt;
			bool dsp_col_found= false;
			int col_count=0;
			int clb_placed=0;
			int clb_left=0;
			
			cout<<endl<<"bram_col_num:  "<<bram_col_num<<endl;
			clb_allot_dsp_or_bram_g_t_0(clb_cnt, bram_cnt, clb_alloted, rec_bram, rec_dsp, y_min_module, dsp_cnt, right_edge_found, bram_col_num, module_num );
			
			
		}
		
		else if (dsp_cnt==0 && bram_cnt==0)
		{
			
			allocate_only_clb(RR1,  module_num);
			int dsp_1=*dsp_col.begin();
			int bram_1=*bram_col.begin();
			//allocate_only_clb_2(RR1, module_num, dsp_1, bram_1);
			/*it_f=filled_region_cords.begin();
			filled_region=*it_f;
			y_min_module=filled_region.y_max+1;
			
			it=dsp_col.begin();
			int dsp_first_col=*it;
			it=bram_col.begin();
			int bram_first_col=*it;
			int clb_before_dsp_bram=0;
			y_min_module=0;
			
			for(unsigned int j=y_min_module; j<y_min_module+(max(bram_cnt, dsp_cnt)); j++)
			{
				for(unsigned int i=)
					clb_alloted++;
					cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
					my_fpga[j][i].usage=true; 
					my_fpga[j][i].module_num =module_num ; 
			}	*/
			
			
			
		}
	}
	RR_count++;
	
	filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false);
}

void allocate_only_clb_2(rec_reg RR1, int module_num, int dsp_first_col, int bram_first_col)
{
	int clb_cnt, rec_wast_cost=0, clb_alloted=0, clb_diff=0, first_col, y_min_module;
	clb_cnt=RR1.clb;
	first_col=min(dsp_first_col, bram_first_col)-1;
	
	for(unsigned int i=0; i<ROWS; i++)
	{
		cout<<endl<<"  my_fpga[i][first_col].module_num: "<<my_fpga[i][first_col].module_num<<endl;
		if(my_fpga[i][first_col].module_num<=-1)
		{
			y_min_module=i;
			break;
		}
	}
	
	
	int num_rows_reqd= clb_cnt/(first_col+1);
	
	cout<<endl<<" number of clb: "<<clb_cnt<<" number of rows reqd:  "<<num_rows_reqd<<" row number: "<<y_min_module<<" colum number: "<<first_col<<endl;
	for ( int i=first_col; i>=0; i--)
	{
		for(unsigned int j=y_min_module; j< y_min_module+num_rows_reqd; j++)
		{
			if(my_fpga[j][i].rec_type==rec_clb)
			{
				clb_alloted++;
				cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
				my_fpga[j][i].usage=true; 
				my_fpga[j][i].module_num =module_num ; 
			}
			
		}	
	}
	
	int clb_left= clb_cnt-clb_alloted;
	
	for ( int i=first_col; i>first_col-clb_left; i--)
	{
		clb_alloted++;
		cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<y_min_module+num_rows_reqd<<" module num:  "<<module_num<<endl;
		my_fpga[y_min_module+num_rows_reqd][i].usage=true; 
		my_fpga[y_min_module+num_rows_reqd][i].module_num =module_num ; 
	}

}


void allocate_only_clb(rec_reg RR1 , int module_num)
{
	int clb_cnt, rec_wast_cost=0, clb_alloted=0, clb_diff=0;
	vector <white_space> :: iterator it;
	multimap <int, white_space> map_ws;
	multimap <int, white_space> :: iterator it_m;
	white_space WS1;
	clb_cnt=RR1.clb;
	
	for (it=white_space_list.begin(); it!=white_space_list.end(); ++it)
	{
		WS1=*it;
		clb_diff=WS1.clb_free-clb_cnt;
		
		if(clb_diff>=0)
		{
			rec_wast_cost= 20*WS1.dsp_free + 10* WS1.bram_free+ 5*clb_diff;
			
			map_ws.insert ( std::pair<int,white_space>(rec_wast_cost,WS1) );
			
		}
	}
	
	it_m=map_ws.begin();
	WS1=it_m->second;
	
	cout<<endl<<" white space number: "<<WS1.module_num<<" ws free clb: "<<WS1.clb_free<<"  free dsp: "<<WS1.dsp_free<<" bram_free: "<<WS1.bram_free<<endl;
	cout<<"x min: "<<WS1.x_min<<" x max:  "<<WS1.x_max<<"  y_min:  "<<WS1.y_min<<" y_max: "<<WS1.y_max<<endl;
	
	for(unsigned int i=WS1.x_max; i>=WS1.x_min; i--)
	{
		if(clb_alloted>=clb_cnt)
		{
			break;
		}
		for(unsigned int j=WS1.y_min; j<=WS1.y_max; j++)
		{
			if(clb_alloted>=clb_cnt)
			{
				break;
			}
			if(my_fpga[j][i].rec_type==rec_clb)
			{
				clb_alloted++;
				cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
				my_fpga[j][i].usage=true; 
				my_fpga[j][i].module_num =module_num ; 
			}
			
		}	
	}
	white_space_list.clear();
	calculate_white_space(ROWS, COLUMNS);
	
	cout<<endl<<" size of white space list:  "<<white_space_list.size()<<endl;
}


void clb_allot_dsp_or_bram_g_t_0(int clb_cnt, int rec_cnt,int clb_alloted, resource rec_main, resource rec_comp, int y_min_module, int bram_cnt, bool right_edge_found, int dsp_col_num,  int module_num)
{
	int dsp_cnt= rec_cnt;
	int num_cols_reqd= (clb_cnt/dsp_cnt)+1;
	int clb_last_col=clb_cnt%dsp_cnt;
	bool bram_col_found= false;
	bool locked_clb_found=false;
			
	int col_count=0; 
	int clb_placed=0;
	int clb_left=0;
			
			
			
			for(unsigned int i=dsp_col_num+1; i<(dsp_col_num+1+num_cols_reqd); i++)
			{
				col_count++;
				if(i==COLUMNS)
				{
					clb_left=clb_cnt-clb_alloted;
					right_edge_found=true;
					break;
				}
				
				if(my_fpga[y_min_module][i].usage==true)
				{
					clb_placed=(col_count-1)*dsp_cnt;
					clb_left=clb_cnt-clb_placed;
					cout<<endl<<"locked_clb_found=true"<<endl;
					locked_clb_found=true;
					break;
				}
				
				if(my_fpga[y_min_module][i].module_num>-1)
				{
					clb_placed=(col_count-1)*dsp_cnt;
					clb_left=clb_cnt-clb_placed;
					cout<<endl<<"locked_clb_found=true"<<endl;
					locked_clb_found=true;
					break;
				}
				
				if(my_fpga[y_min_module][i].rec_type==rec_comp)
				{
					clb_placed=(col_count-1)*dsp_cnt;
					clb_left=clb_cnt-clb_placed;
					cout<<endl<<"bram_col_found=true"<<endl;
					bram_col_found=true;
					break;
				}
				 
				for(unsigned int j=y_min_module; j<y_min_module+(max(bram_cnt, dsp_cnt)); j++)
				{
					clb_alloted++;
					cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
					my_fpga[j][i].usage=true; 
					my_fpga[j][i].module_num =module_num ; 
				}	
				
				
			}
			
			

			if(bram_col_found==false && my_fpga[y_min_module][dsp_col_num+num_cols_reqd+1].usage==false && right_edge_found==false && (dsp_col_num+num_cols_reqd+1)<23 && locked_clb_found==false)
			{
				cout<<endl<<"   here jkdcnc "<<endl; 
				for(unsigned int j=y_min_module; j<y_min_module+clb_last_col; j++)
				{
					clb_alloted++;
					
					my_fpga[j][dsp_col_num+1+num_cols_reqd].usage=true; 
					my_fpga[j][dsp_col_num+1+num_cols_reqd].module_num =module_num ; 
				}
			}
			else if(bram_col_found==true ||  right_edge_found==true || locked_clb_found==true)
			{
				cout<<endl<<"   there "<<endl; 
				int num_cols_reqd= (clb_left/dsp_cnt)+1;
				int clb_last_col=clb_left%dsp_cnt; 
				bool col_less_zero= false;
				int shift_cnt=-1;
				cout<<"number of cols reqd: "<<num_cols_reqd<<endl;
				
				for(signed int i=dsp_col_num-1; i>(dsp_col_num-1-num_cols_reqd); i--)
				{
					
					if (i<0)
					{ 
						col_less_zero= true;
						int clb_left= clb_cnt-clb_alloted;
						int y_cord= y_min_module+dsp_cnt;
						int col_num= 0;
						resource rec= rec_main;
						extend_vertically(clb_cnt, clb_left, y_cord, col_num, rec, clb_alloted, module_num );
					}
					
					if(my_fpga[y_min_module][i].usage== true || my_fpga[y_min_module][i].module_num>-1 )
					{
						int clb_left= clb_cnt-clb_alloted;
						int y_cord= y_min_module+dsp_cnt;
						int colls_reqd=  clb_left/(max(bram_cnt, dsp_cnt));
						
						shift_cnt=shift_func(dir_left,  my_fpga[y_min_module][i].module_num, colls_reqd).cnt;
						cout<<endl<<"clb allotted: "<<clb_alloted<<" clb count:  "<<clb_cnt<<"  clb left: "<<clb_left;
						cout<<endl<<"module number: "<<module_num<<"  module shifted:  "<<my_fpga[y_min_module][i].module_num<<" shoft amount: "<<shift_cnt<<"  colls_reqd:  "<<colls_reqd<<endl;
					}
					
					
					if(my_fpga[y_min_module][i].rec_type!=rec_clb || shift_cnt==0 )
					{
						int clb_left= clb_cnt-clb_alloted;
						int y_cord= y_min_module+dsp_cnt;
						int col_num=0;
						cout<<endl<<"ooooooooooooooooooooooooooooooooooooooooooo locked cell found row: "<<y_min_module<<" column:  "<<i<<" clb allotted "<<clb_alloted<<endl;
						
						if(i==dsp_col_num-1)
						{
						  col_num= i+2;
						}
						else 
						{
						  col_num= i+1;
						}
						
						resource rec= rec_main;
						extend_vertically(clb_cnt, clb_left, y_cord, col_num, rec, clb_alloted, module_num );
						break;
					}
					
					if(my_fpga[y_min_module][i].usage== false && my_fpga[y_min_module][i].rec_type==rec_clb)
					{
						for(unsigned int j=y_min_module; j<y_min_module+(max(bram_cnt, dsp_cnt)); j++)
						{
							clb_alloted++;
							cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<endl;
							my_fpga[j][i].usage=true; 
							my_fpga[j][i].module_num =module_num ; 
							if(clb_alloted==clb_cnt)
							{
								cout<<endl<<"clb allotted 124:  "<<clb_alloted<<endl;
								break;
							}
						}
						
						
					}
					
				}
				
				if(my_fpga[y_min_module][dsp_col_num-1-num_cols_reqd].usage== false && my_fpga[y_min_module][dsp_col_num-1-num_cols_reqd].rec_type==rec_clb && col_less_zero== false && clb_alloted<clb_cnt)	
				{
					for(unsigned int j=y_min_module; j<y_min_module+clb_last_col; j++)
					{
						cout<<" hhhhd  "<<endl;
						clb_alloted++;
						my_fpga[j][dsp_col_num-1-num_cols_reqd].usage=true; 
						my_fpga[j][dsp_col_num-1-num_cols_reqd].module_num =module_num ; 
					}
				}
				
				else if((my_fpga[y_min_module][dsp_col_num-1-num_cols_reqd].usage== true || my_fpga[y_min_module][dsp_col_num-1-num_cols_reqd].rec_type!=rec_clb) && col_less_zero== false)
				{
					int clb_left= clb_cnt-clb_alloted;
					int y_cord= y_min_module+dsp_cnt;
					int col_num= dsp_col_num-num_cols_reqd;
					resource rec= rec_bram;
					cout<<endl<<"pingu......";
					extend_vertically(clb_cnt, clb_left, y_cord, col_num, rec, clb_alloted, module_num);
				}
				
				
			}
			cout<<endl;
			cout<<endl<<" ------------------------- module number "<<module_num<< " --------------------------------cccccccccc llllllllllll bbbbbbbbbbbbb alloted: "  <<clb_alloted<<endl;
		
}


void extend_vertically(int clb_cnt, int clb_left, int y_cord, int col_num, resource rec, int clb_allot,  int module_num)
{
	cout<<endl<<"=============||||||||||||||||======== MODULE NUMBER: "<<module_num<<"  clb left: "<<clb_left<<" clb allotted: "<<clb_allot<<endl;;
	int clb_alloted=clb_allot;
	resource rec_here;
	int col_cnt;
	int x_lim=COLUMNS;
	
	if(rec== rec_dsp)
	{
		cout<<endl<<" rec type:  dsp "<<rec_dsp<<endl;
		rec_here=rec_bram;
	}
	
	else if(rec== rec_bram)
	{
		cout<<endl<<" rec type:  bram "<<rec_bram<<endl;
		rec_here=rec_dsp;
	}
exten:	for(unsigned int i=col_num; my_fpga[y_cord][i].rec_type!=rec_here ; i++ )
	{
		col_cnt++;
		if(clb_alloted==clb_cnt)
		{
			break;
		}
		
		if (i==x_lim)
		{
			cout<<"x_lim=  "<<i<<endl;
			break;
		}
		if (my_fpga[y_cord][i].module_num>-1)
		{
			x_lim=i;
			break;
		}
		
		if(my_fpga[y_cord][i].rec_type==rec)
		{
			continue;
		}
		clb_alloted++;
		cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<y_cord<<endl;
		my_fpga[y_cord][i].usage=true;
		my_fpga[y_cord][i].module_num =module_num ; 
		
	}
	
	if (clb_alloted<clb_cnt)
	{
		y_cord++;
		goto exten;
	}
	else
	{
		cout<<endl<<"clb alloted final = : "<< clb_alloted<<"    "<<col_cnt<<endl;
	}
	
}


//////////////////****************** static module allocation ***************************//////


void allocate_static_module(rec_reg RR1,  fpga fpga_arch[10][23], int module_num)
{
	int clb_cnt, bram_cnt, dsp_cnt, dsp_col_num, bram_col_num, clb_b_dsp_bram;
	clb_cnt= RR1.clb;
	bram_cnt= RR1.bram;
	dsp_cnt=RR1.dsp;
	vector <int> :: iterator it;
	map <int, int> dist_vec_map;
	map <int, int>:: iterator itm;
	cout<<endl<<"clb cnt "<<clb_cnt;
	cout<<endl<<"bram cnt "<<bram_cnt;
	cout<<endl<<"dsp cnt "<<dsp_cnt<<endl<<endl;
	
	
	
	if(dsp_cnt>0)
	{
		for (it=dsp_col.begin(); it!=dsp_col.end(); ++it)
	{
		for(unsigned int i=0; i<dsp_cnt; i++)
		{
			my_fpga[i][*it].usage=true;   
			my_fpga[i][*it].module_num =module_num ;    
		}
		dsp_col_num=*it;
		break;
		
	}	
	}
	else if (dsp_cnt==0)
	{
		dsp_col_num=0;
	}

    
	if(bram_cnt>0)
	{
		if (dsp_cnt>0)
		{
			for (it=bram_col.begin(); it!=bram_col.end(); ++it)
			{
				int dist=abs(dsp_col_num-*it);
				dist_vec_map.insert ( std::pair<int,int>(dist,*it) );
			}
			int first_ele=dist_vec_map.begin()->second;
			for(unsigned int i=0; i<bram_cnt; i++)
			{
				my_fpga[i][first_ele].usage=true;   
				my_fpga[i][first_ele].module_num =module_num ;  
			}
			bram_col_num=first_ele;
		}
		else if(dsp_cnt==0)
		{
			it=bram_col.begin();
			for(unsigned int i=0; i<bram_cnt; i++)
			{
				my_fpga[i][*it].usage=true;   
				my_fpga[i][*it].module_num =module_num ;   
			}
			bram_col_num=*it;
		}
	}
	
		
	else if(bram_cnt==0)  
	{
		bram_col_num=0;
	}
	
	
	if(clb_cnt>0)   
	{
		if(dsp_cnt>0 && bram_cnt>0)
		{
			clb_b_dsp_bram=abs(dsp_col_num-bram_col_num);
			cout<<"\n dsp count: "<<dsp_cnt<<" bram cnt : "<<bram_cnt<<endl;
			cout<<"number of clbs between bram & dsp cols: "<<(clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))<<endl;
	
			if(clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))))
			{   
				int source=0, dest=0;
				
				if(dsp_col_num<bram_col_num)
				{
					dest=bram_col_num;
					source=dsp_col_num+1;
				}
				else if(bram_col_num<dsp_col_num)
				{
					source=bram_col_num+1;
					dest=dsp_col_num;
				}
				for(unsigned int i=source; i<dest; i++)
				{
					for(unsigned int j=0; j<(clb_cnt/(clb_b_dsp_bram-1)); j++)
					{
						my_fpga[j][i].usage=true;
						my_fpga[j][i].module_num =module_num ; 
					}
				}
				int final_row=(clb_cnt/(clb_b_dsp_bram-1));
				int final_row_clbs=(clb_cnt)%(clb_b_dsp_bram-1);
		
				for(unsigned int n=source; n<(source+final_row_clbs); n++)
				{
					my_fpga[final_row][n].usage=true;
					my_fpga[final_row][n].module_num =module_num ; 
				}
			}
		
			else if(clb_cnt>((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt))))		
			{
				int source=0, dest=0;
				
				if(dsp_col_num<bram_col_num)
				{
					dest=bram_col_num;
					source=dsp_col_num+1;
				}
				else if(bram_col_num<dsp_col_num)
				{
					source=bram_col_num+1;
					dest=dsp_col_num;
				}
				for(unsigned int i=source; i<dest; i++)
				{
					if(bram_cnt>=dsp_cnt)
					{  
						for(unsigned int j=0; j<bram_cnt; j++)
						{
							my_fpga[j][i].usage=true; 
							my_fpga[j][i].module_num =module_num ; 
						}
					}
					else if(dsp_cnt>bram_cnt)
					{
						for(unsigned int j=0; j<dsp_cnt; j++)
						{
							my_fpga[j][i].usage=true; 
							my_fpga[j][i].module_num =module_num ; 
						}
					}
				}
		
				int clb_left= clb_cnt-((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt)));
				int num_cols_reqd= (clb_left)/max(bram_cnt, dsp_cnt);
				int clb_last_col= (clb_left)%(max(bram_cnt, dsp_cnt));
		
				cout<<" number of colums reqd: "<<num_cols_reqd<<"  dsp column no: "<<dsp_col_num<<" last colum cordinate:" <<(dsp_col_num+1+num_cols_reqd)<<endl;
		
				if(dsp_col_num>bram_col_num)
				{
					for(unsigned int n=dsp_col_num+1; n<(dsp_col_num+1+num_cols_reqd); n++)
					{
						for(unsigned int j=0; j<(max(bram_cnt, dsp_cnt)); j++)
						{
							my_fpga[j][n].usage=true; 
							my_fpga[j][n].module_num =module_num ;
						}
					}
		
					for(unsigned int j=0; j<clb_last_col; j++)
					{
						my_fpga[j][dsp_col_num+num_cols_reqd+1].usage=true; 
						my_fpga[j][dsp_col_num+num_cols_reqd+1].module_num =module_num ; 
					}
				}
		
				else if(bram_col_num>dsp_col_num)
				{
					for(unsigned int n=bram_col_num+1; n<(bram_col_num+1+num_cols_reqd); n++)
					{
						for(unsigned int j=0; j<(max(bram_cnt, dsp_cnt)); j++)
						{
							my_fpga[j][n].usage=true; 
							my_fpga[j][n].module_num =module_num ;
						}
					}
		
					for(unsigned int j=0; j<clb_last_col; j++)
					{
						my_fpga[j][bram_col_num+num_cols_reqd+1].usage=true; 
						my_fpga[j][bram_col_num+num_cols_reqd+1].module_num =module_num ;
					}
				}
			}
		}

		else if(dsp_cnt>0 && bram_cnt==0)
		{
			int num_cols_reqd= clb_cnt/dsp_cnt;
			int clb_last_col=clb_cnt%dsp_cnt;
			bool bram_col_found= false;
			int col_count=0;
			int clb_placed=0;
			int clb_left=0;
			int y_min_module=0;
			int clb_alloted=0;
			bool right_edge_found=false;
			
			clb_allot_dsp_or_bram_g_t_0(clb_cnt, dsp_cnt, clb_alloted, rec_dsp, rec_bram, y_min_module, bram_cnt, right_edge_found, dsp_col_num, module_num );
			
		}
		
		else if(bram_cnt>0 && dsp_cnt==0)
		{
			
			int num_cols_reqd= clb_cnt/bram_cnt;
			int clb_last_col=clb_cnt%bram_cnt;
			bool dsp_col_found= false;
			int col_count=0;
			int clb_placed=0;
			int clb_left=0;
			int y_min_module=0;
			int clb_alloted=0;
			bool right_edge_found=false;
			
			cout<<endl<<"bram_col_num:  "<<bram_col_num<<endl;
			clb_allot_dsp_or_bram_g_t_0(clb_cnt, bram_cnt, clb_alloted, rec_bram, rec_dsp, y_min_module, dsp_cnt, right_edge_found, bram_col_num, module_num );
			
			
		}
		
		else if (dsp_cnt==0 && bram_cnt==0)
		{
			it=clb_col.begin();
			int clb_first_col=*it;
			it=dsp_col.begin();
			int dsp_first_col=*it;
			it=bram_col.begin();
			int bram_first_col=*it;
			int clb_before_dsp_bram=0;
			
			if(dsp_first_col<bram_first_col)
			{
				clb_before_dsp_bram=dsp_first_col-clb_first_col;
			}
			else if(dsp_first_col>bram_first_col)
			{
				clb_before_dsp_bram=bram_first_col-clb_first_col;
			}
			int num_rows_reqd= clb_cnt/clb_before_dsp_bram;
			int clb_last_row=clb_cnt%clb_before_dsp_bram;  
			
			for(signed int i=0; i<num_rows_reqd; i++)
				{
					for(unsigned int j=0; j<clb_before_dsp_bram; j++)
					{
						my_fpga[i][j].usage=true; 
						my_fpga[i][j].module_num =module_num ;
					}
				}	
			for(unsigned int j=0; j<clb_last_row; j++)
			{
				my_fpga[num_rows_reqd][j].usage=true; 
				my_fpga[num_rows_reqd][j].module_num =module_num ;
			}
			
		}
	}
	
	filled_reg SR=calculate_module_cordinate(ROWS, COLUMNS, 0, false);
}










  


































