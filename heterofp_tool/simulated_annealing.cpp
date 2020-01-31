#include "simulated_annealing.h"
#include"general.h"
#include "fpga.h"
#include "allocate_recon_module.h"
#include "sim_annealing_moves.h"
#include "white_space.h"
#include "sequence_pair.h"
#include "cost_calc.h"


void simulated_annealing(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b, int curr_cost)
{
	
	float min_temp, next_temp, temp;
	int cost, min_clb_wasted=0, min_bram_wasted=0, min_dsp_wasted=0, min_clb_used=0, min_bram_used=0, min_dsp_used=0;
	float perc_clb_wasted, perc_bram_wasted, perc_dsp_wasted;
	cost_param cost_fac, cost_fac_min;
	//min_temp= 0.005* cost/(no_modules); 
	unsigned int max_iteration=12;
	int val=0;
	vector <rec_reg> :: iterator randIt; 
	min_cost=curr_cost;
	rec_reg RR2;
	
	for (unsigned iteration = 0; iteration < max_iteration; iteration++)
	{
		val=iteration%4;
		clear_fpga(ROWS, COLUMNS);
		print_fpga(ROWS, COLUMNS);
		cout<<endl<<" fpga printed iteration number: "<<iteration<<endl;
		filled_region_cords.clear();
		allocate_static_module(static_mod, 0);
		shuffled_region_list.clear();
		int size_list=sorted_region_list.size();
		//shuffled_region_list=shuffle_rr_list( rec_region_list);
		//cost_fac=allocate_list(shuffled_region_list);*/
		int v1= 0, v2= 0;
		switch (val) 
		 {
			 case 0:            
				shuffled_region_list=shuffle_rr_list( sorted_region_list);
				cost_fac=allocate_list(shuffled_region_list, false);
				cout<<endl<<"shuffled"<<endl;
				break;
			case 1:         
				randIt = sorted_region_list.begin();
				advance(randIt, rand() % sorted_region_list.size());
				RR2=*randIt;
				cout<<endl<<" rr1 module num "<<RR2.module_num<<endl;
				shuffled_region_list=remove_n_replace(sorted_region_list, RR2.module_num);
				cost_fac=allocate_list(shuffled_region_list, false);
				cout<<endl<<"removed and replaced"<<endl;
				break;
			case 2:   
				v1=rand()%size_list;
				v2=rand()%size_list;
				
				shuffled_region_list=swap_two_modules(sorted_region_list, v1, v2);
				cost_fac=allocate_list(shuffled_region_list, false);	
				cout<<endl<<"swaped"<<endl;
				break;
			case 3:     
				randIt = sorted_region_list.begin();
				advance(randIt, rand() % sorted_region_list.size());
				RR2=*randIt;
				cout<<endl<<" rr1 module num "<<RR2.module_num<<endl;
				shuffled_region_list=remove_n_replace(sorted_region_list, RR2.module_num);
				//shuffled_region_list=remove_n_replace(sorted_region_list, 6);
				cost_fac=allocate_list(shuffled_region_list, false);
				//cost_fac=shift_func(dir_left, 4, 5);	
				//shuffled_region_list=shuffle_rr_list( rec_region_list);
				//cost_fac=allocate_list(shuffled_region_list);					
				cout<<"Thank you for playing!\n";
				break;
			default:        
				cost_fac=allocate_list(sorted_region_list, false);
				break;
				
		}
		
		
		calculate_white_space(ROWS, COLUMNS);
		cost_fac=allocate_list(only_clb_list, true);
		current_cost=cost_fac.FINAL_COST;
		//int hpwl= calculate_hpwl("aptenew.fpga"); 
		
		if(current_cost<min_cost)
		{
			cost_fac_min=cost_fac;
			min_cost=cost_fac.FINAL_COST;
			seq_pair_min_a=seq_pair_a;
			seq_pair_min_b=seq_pair_b;
			
			cout<<endl;
			for (unsigned int j=0; j<ROWS;j++)
			{
				for(unsigned int i=0; i<COLUMNS; i++)
				{
					
					min_fpga[j][i].rec_type= my_fpga[j][i].rec_type;
					min_fpga[j][i].rec_name= my_fpga[j][i].rec_name;
					min_fpga[j][i].module_num= my_fpga[j][i].module_num;
					min_fpga[j][i].rec_reg_name= my_fpga[j][i].rec_reg_name;
					min_fpga[j][i].usage= my_fpga[j][i].usage;
				}
			}
		}
		
		
		wasted_rec_fie<<endl<<"\n iteration number: "<<iteration;
		outputfile <<endl<< "iteration number: "<<iteration<<"  wasted resource: " << cost_fac.rw_cost<<"  total area: "<<cost_fac.total_area<<"  wiighed WL: "<<10*cost_fac.weighted_WL<<"   FINAL COST: "<<cost_fac.FINAL_COST<<"  min cost:" <<min_cost<<endl;
		
		cout<<endl<<" iterartion: "<<iteration<<endl;
	}
	
	cout<<endl<<"min cost: "<<min_cost<<" filled region size: "<<filled_region_cords.size()<<endl;
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			my_fpga[j][i].rec_type= min_fpga[j][i].rec_type;
			my_fpga[j][i].rec_name= min_fpga[j][i].rec_name;
			my_fpga[j][i].module_num= min_fpga[j][i].module_num;
			my_fpga[j][i].rec_reg_name= min_fpga[j][i].rec_reg_name;
			my_fpga[j][i].usage= min_fpga[j][i].usage;
		}
	}

	int clb_wasted=0, dsp_wasted=0, bram_wasted=0, clb_used=0, bram_used=0, dsp_used=0;
	cout<<endl<<" ROWS: "<<ROWS<<"  cols: "<<COLUMNS<<endl; 
	for (unsigned int i=0; i<ROWS;i++)
	{
		for(unsigned int j=0; j<COLUMNS; j++)
		{
			if(my_fpga[i][j].module_num>-1)
			{
				if(my_fpga[i][j].usage==false )
				{
					if(my_fpga[i][j].rec_type==rec_clb )
					{
						clb_wasted=clb_wasted+clb_per_tile;
						
					}
					else if(my_fpga[i][j].rec_type==rec_bram )
					{
						cout<<endl<<" bram wasted: col: "<<j<<"  row: "<<i;
						bram_wasted=bram_wasted+bram_per_tile;
					}
					else if(my_fpga[i][j].rec_type==rec_dsp )
					{
						dsp_wasted=dsp_per_tile;
					}
				}
			
				else if(my_fpga[i][j].usage==true )
				{
					if(my_fpga[i][j].rec_type==rec_clb )
					{
						clb_used=clb_used+clb_per_tile;
						
						
					}
					else if(my_fpga[i][j].rec_type==rec_bram )
					{
						bram_used=bram_used+bram_per_tile;
						cout<<endl<<" bram used: "<<bram_used;
						
					}
					else if(my_fpga[i][j].rec_type==rec_dsp )
					{
						dsp_used=dsp_used+dsp_per_tile;
						cout<<endl<<"dsp used: "<<dsp_used; 
						
					}
				}
			}
		}
	}
		
	
	print_fpga(ROWS, COLUMNS);
	
	//calculate_white_space(ROWS, COLUMNS);
	//cost_fac=allocate_list(only_clb_list, true);
	
	gen_seq_pair(filled_region_cords);
	print_fpga(ROWS, COLUMNS);
	cout<<endl<<"  size od clb list:  "<<only_clb_list.size()<<endl;
	
	perc_clb_wasted=(clb_wasted*100)/(clb_wasted+clb_used);
	perc_bram_wasted=(bram_wasted*100)/(bram_wasted+bram_used);
	perc_dsp_wasted=(dsp_wasted*100)/(dsp_wasted+dsp_used);
	
	
	outputfile <<endl<< " max iteration number: "<<max_iteration<<"min  wasted resource: " << cost_fac_min.rw_cost<<"  min total area: "<<cost_fac_min.total_area<<" min  wighed WL: "<<10*cost_fac_min.weighted_WL<<"  MIN  FINAL COST: "<<cost_fac_min.FINAL_COST<<endl;
	outputfile<<" % clb wasted: "<<perc_clb_wasted<<"  % bram wasted: "<<perc_bram_wasted<<"  % dsp wasted: "<<perc_dsp_wasted<<endl;
	outputfile<<"  clb wasted: "<<clb_wasted<<"   bram wasted: "<<bram_wasted<<"   dsp wasted: "<<dsp_wasted<<endl;
	outputfile<<"  clb used: "<<clb_used<<"   bram used: "<<bram_used<<"   dsp used: "<<dsp_used<<endl;
	cout<<endl<<"min cost: "<<min_cost<<" filled region size: "<<filled_region_cords.size()<<endl;
	print_seq_pair(seq_pair_a, seq_pair_b);
	
}


float rand_01() 
{ 
	return float(rand()%10000)/10000; 
}

void simulated_annealing_main(vector <filled_reg> seq1_pair_a, vector <filled_reg> seq1_pair_b, int curr_cost)
{
	
	float min_temp, next_temp, temp;
	float T=100;
	int cost, min_clb_wasted=0, min_bram_wasted=0, min_dsp_wasted=0, min_clb_used=0, min_bram_used=0, min_dsp_used=0;
	float perc_clb_wasted, perc_bram_wasted, perc_dsp_wasted;
	cost_param cost_fac, cost_fac_min;
	
	unsigned int max_iteration=90;
	int val=0;
	vector <rec_reg> :: iterator randIt; 
	min_cost=curr_cost;
	rec_reg RR2;
	int count=0, uphill=0, reject=0, MT=0, d_cost=0, prev_cost=0;
	prev_cost= curr_cost;
	
	cout<<endl<<" temperature: "<<T<<" (float)reject/(float)MT: "<<(float)reject/(float)MT<<endl; 
	do 
	{
		cout<<endl<<" inside first while"<<endl;
        MT=uphill=reject=0;
		for (unsigned iteration = 0; iteration < max_iteration; iteration++)
		{
			val=iteration%4;
			clear_fpga(ROWS, COLUMNS);
			print_fpga(ROWS, COLUMNS);
			cout<<endl<<" fpga printed iteration number: "<<iteration<<endl;
			filled_region_cords.clear();
			allocate_static_module(static_mod, 0);
			shuffled_region_list.clear();
			int size_list=sorted_region_list.size();
			//shuffled_region_list=shuffle_rr_list( rec_region_list);
			//cost_fac=allocate_list(shuffled_region_list);*/
			int v1= 0, v2= 0;
			switch (val) 
			{
				case 0:            
					shuffled_region_list=shuffle_rr_list( sorted_region_list);
					cost_fac=allocate_list(shuffled_region_list, false);
					cout<<endl<<"shuffled"<<endl;
					break;
				case 1:         
					randIt = sorted_region_list.begin();
					advance(randIt, rand() % sorted_region_list.size());
					RR2=*randIt;
					cout<<endl<<" rr1 module num "<<RR2.module_num<<endl;
					shuffled_region_list=remove_n_replace(sorted_region_list, RR2.module_num);
					cost_fac=allocate_list(shuffled_region_list, false);
					cout<<endl<<"removed and replaced"<<endl;
					break;
				case 2:   
					v1=rand()%size_list;
					v2=rand()%size_list;
					
					shuffled_region_list=swap_two_modules(sorted_region_list, v1, v2);
					cost_fac=allocate_list(shuffled_region_list, false);	
					cout<<endl<<"swaped"<<endl;
					break;
				case 3:     
					randIt = sorted_region_list.begin();
					advance(randIt, rand() % sorted_region_list.size());
					RR2=*randIt;
					cout<<endl<<" rr1 module num "<<RR2.module_num<<endl;
					shuffled_region_list=remove_n_replace(sorted_region_list, RR2.module_num);
					//shuffled_region_list=remove_n_replace(sorted_region_list, 6);
					cost_fac=allocate_list(shuffled_region_list, false);
					//cost_fac=shift_func(dir_left, 4, 5);	
					//shuffled_region_list=shuffle_rr_list( rec_region_list);
					//cost_fac=allocate_list(shuffled_region_list);					
					cout<<"Thank you for playing!\n";
					break;
				default:        
					cost_fac=allocate_list(sorted_region_list, false);
					break;
					
			}
			
			
			calculate_white_space(ROWS, COLUMNS);
			cost_fac=allocate_list(only_clb_list, true);
			current_cost=cost_fac.FINAL_COST;
			
			MT++;
			d_cost=current_cost-prev_cost;
			float p = exp(-d_cost/T);
			
			if(d_cost<0 || rand_01() < p)
			{
				if (d_cost > 0)
				{
					uphill=uphill + 1;
				}					
				
				prev_cost=current_cost;
				
				if(current_cost<min_cost)
				{
					cost_fac_min=cost_fac;
					min_cost=cost_fac.FINAL_COST;
					seq_pair_min_a=seq_pair_a;
					seq_pair_min_b=seq_pair_b;
					
					cout<<endl;
					for (unsigned int j=0; j<ROWS;j++)
					{
						for(unsigned int i=0; i<COLUMNS; i++)
						{
							
							min_fpga[j][i].rec_type= my_fpga[j][i].rec_type;
							min_fpga[j][i].rec_name= my_fpga[j][i].rec_name;
							min_fpga[j][i].module_num= my_fpga[j][i].module_num;
							min_fpga[j][i].rec_reg_name= my_fpga[j][i].rec_reg_name;
							min_fpga[j][i].usage= my_fpga[j][i].usage;
						}
					}
				}
				
			}
			else
			{
				reject++;
			}
		
		
			wasted_rec_fie<<endl<<"\n iteration number: "<<iteration;
			outputfile <<endl<< "iteration number: "<<iteration<<"  wasted resource: " << cost_fac.rw_cost<<"  total area: "<<cost_fac.total_area<<"  wiighed WL: "<<10*cost_fac.weighted_WL<<"   FINAL COST: "<<cost_fac.FINAL_COST<<"  min cost:" <<min_cost<<endl;
			
			cout<<endl<<" iterartion: "<<iteration<<endl;
		}
		   T=0.9*T;
		 outputfile<<endl<<"temperature:  "<<T<<"(float)reject/(float)MT: "<<(float)reject/(float)MT<<endl;
	}	 while((float)reject/(float)MT<0.95 || T>60);
	
	
	cout<<endl<<"min cost: "<<min_cost<<" filled region size: "<<filled_region_cords.size()<<endl;
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			my_fpga[j][i].rec_type= min_fpga[j][i].rec_type;
			my_fpga[j][i].rec_name= min_fpga[j][i].rec_name;
			my_fpga[j][i].module_num= min_fpga[j][i].module_num;
			my_fpga[j][i].rec_reg_name= min_fpga[j][i].rec_reg_name;
			my_fpga[j][i].usage= min_fpga[j][i].usage;
		}
	}

	int clb_wasted=0, dsp_wasted=0, bram_wasted=0, clb_used=0, bram_used=0, dsp_used=0;
	cout<<endl<<" ROWS: "<<ROWS<<"  cols: "<<COLUMNS<<endl; 
	for (unsigned int i=0; i<ROWS;i++)
	{
		for(unsigned int j=0; j<COLUMNS; j++)
		{
			if(my_fpga[i][j].module_num>-1)
			{
				if(my_fpga[i][j].usage==false )
				{
					if(my_fpga[i][j].rec_type==rec_clb )
					{
						clb_wasted=clb_wasted+clb_per_tile;
						
					}
					else if(my_fpga[i][j].rec_type==rec_bram )
					{
						cout<<endl<<" bram wasted: col: "<<j<<"  row: "<<i;
						bram_wasted=bram_wasted+bram_per_tile;
					}
					else if(my_fpga[i][j].rec_type==rec_dsp )
					{
						dsp_wasted=dsp_per_tile;
					}
				}
			
				else if(my_fpga[i][j].usage==true )
				{
					if(my_fpga[i][j].rec_type==rec_clb )
					{
						clb_used=clb_used+clb_per_tile;
						
						
					}
					else if(my_fpga[i][j].rec_type==rec_bram )
					{
						bram_used=bram_used+bram_per_tile;
						cout<<endl<<" bram used: "<<bram_used;
						
					}
					else if(my_fpga[i][j].rec_type==rec_dsp )
					{
						dsp_used=dsp_used+dsp_per_tile;
						cout<<endl<<"dsp used: "<<dsp_used; 
						
					}
				}
			}
		}
	}
		
	
	print_fpga(ROWS, COLUMNS);
	
	//calculate_white_space(ROWS, COLUMNS);
	//cost_fac=allocate_list(only_clb_list, true);
	
	gen_seq_pair(filled_region_cords);
	print_fpga(ROWS, COLUMNS);
	cout<<endl<<"  size od clb list:  "<<only_clb_list.size()<<endl;
	
	perc_clb_wasted=(clb_wasted*100)/(clb_wasted+clb_used);
	perc_bram_wasted=(bram_wasted*100)/(bram_wasted+bram_used);
	perc_dsp_wasted=(dsp_wasted*100)/(dsp_wasted+dsp_used);
	
	
	outputfile <<endl<< " max iteration number: "<<max_iteration<<"min  wasted resource: " << cost_fac_min.rw_cost<<"  min total area: "<<cost_fac_min.total_area<<" min  wighed WL: "<<10*cost_fac_min.weighted_WL<<"  MIN  FINAL COST: "<<cost_fac_min.FINAL_COST<<endl;
	outputfile<<" % clb wasted: "<<perc_clb_wasted<<"  % bram wasted: "<<perc_bram_wasted<<"  % dsp wasted: "<<perc_dsp_wasted<<endl;
	outputfile<<"  clb wasted: "<<clb_wasted<<"   bram wasted: "<<bram_wasted<<"   dsp wasted: "<<dsp_wasted<<endl;
	outputfile<<"  clb used: "<<clb_used<<"   bram used: "<<bram_used<<"   dsp used: "<<dsp_used<<endl;
	cout<<endl<<"min cost: "<<min_cost<<" filled region size: "<<filled_region_cords.size()<<endl;
	print_seq_pair(seq_pair_a, seq_pair_b);
	
}


