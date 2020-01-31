#include "sim_annealing_moves.h"
#include"general.h"
#include "allocate_recon_module.h"
#include "sequence_pair.h"
#include "cost_calc.h"
#include "fpga.h"

vector <rec_reg> shuffled_region_list;
vector <rec_reg>  shuffle_rr_list(vector <rec_reg> rec_region_list)
{
	vector <rec_reg> RR_region_temp;
	RR_region_temp.clear();
	RR_region_temp=rec_region_list;
	
	std::random_shuffle ( RR_region_temp.begin(), RR_region_temp.end() );  
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
	filled_reg SR=calculate_module_cordinate(ROWS, COLUMNS, module_num, true, module_num);
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
	
	SR=calculate_module_cordinate(ROWS, COLUMNS, module_num, true, module_num);
	
	gen_seq_pair(filled_region_cords);
	length_pair=calculate_wirelength(rec_region_list, false);
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
