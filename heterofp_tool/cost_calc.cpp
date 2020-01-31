#include "cost_calc.h"


//vector <filled_reg> filled_region_cords;
pair <long int,long int>  calculate_wirelength(vector <rec_reg> rec_region_list, bool clb_exist)
{
	vector <rec_reg> :: iterator it_1;
	vector <rec_reg> :: iterator it_2;
	pair <long int,long int> length_pair ;
	int x_dist, y_dist, man_dist, num_wires, weigthed_manh_dist;
	long int total_length=0, weig_tot_len=0;
	rec_reg RR1, RR2;

	for (it_1=rec_region_list.begin()+1; it_1!=rec_region_list.end(); ++it_1)
	{
		RR1=*it_1;
		
		if(clb_exist==false)
		{
			if(RR1.dsp==0 && RR1.bram==0)
			{
				cout<<endl<<" RR1 module number   pqrst: "<<RR1.module_num<<"  module name: "<<RR1.name<<endl;
				continue;
			}
		}
		for (it_2=rec_region_list.begin(); it_2!=rec_region_list.end()-1; ++it_2)
		{
			RR2=*it_2;
			
			if(clb_exist==false)
			{
				if(RR2.dsp==0 && RR2.bram==0)
				{
				cout<<endl<<" RR2 module number abcde: "<<RR1.module_num<<"  module name: "<<RR1.name<<endl;
				continue;
				}
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




int calculate_wasted_res(vector <filled_reg> filled_region_cords)
{
	vector <filled_reg>:: iterator it;
	int clb_cost=1, bram_cost=9, dsp_cost=19, clb_wasted=0, bram_wasted=0, dsp_wasted=0, rw_cost=0, cnt=0;
	clb_cost=total_clb/total_clb;
	bram_cost=total_clb/total_bram;
	dsp_cost=total_clb/total_dsp;
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

cost_param allocate_list(vector <rec_reg> module_list, bool clb_exist)
{
	cost_param cost_fac;
	int  rw_cost=0, total_area=0, i=0, final_cost;
	//vector <rec_reg> rec_region_list;
	rec_reg RR1;
	cost_fac.rw_cost= 0;
	cost_fac.total_area=0;
	cost_fac.total_WL=0;
	cost_fac.weighted_WL=0;
	cost_fac.cnt=0;
	cost_fac.FINAL_COST=0;
	vector <rec_reg> ::iterator it;
	vector <rec_reg> ::reverse_iterator it_rv;
	pair <long int,long int> length_pair;
	length_pair.first=0;
	length_pair.second=0;
	for (it=module_list.begin(); it!=module_list.end(); ++it)
    {
		i++;
		RR1=*it;
		cout<<endl<<" -------------------------------------------------------------------------------------------module number: "<<RR1.module_num<<endl;
		allocate_recon_module(RR1, RR1.module_num);
		
	}
	

	
	//it_rv=module_list.rbegin();
	//RR1=*it_rv;
	//allocate_recon_module(RR1,  my_fpga, RR1.module_num);

	gen_seq_pair(filled_region_cords);
	length_pair=calculate_wirelength(rec_region_list, clb_exist);
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
	outputfile<<endl<<" WL w: "<<length_pair.second<<endl;
	print_fpga(ROWS, COLUMNS); 
	
	
	cout<<"done"<<endl;
	return(cost_fac);
	
}
