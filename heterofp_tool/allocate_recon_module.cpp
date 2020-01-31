#include "allocate_recon_module.h"
#include"general.h"
#include "white_space.h"
#include "sim_annealing_moves.h"

	vector <filled_reg> filled_region_cords;
	int ROWS, COLUMN;
	
filled_reg calculate_module_cordinate(int ROWS, int COLUMS,  int module_num, bool replcmt, int module_2)
{
	
	vector<int> x_cor;
	vector<int> y_cor;
	vector<int>::const_iterator it2;  
	filled_reg static_reg;
	rec_reg static_mod1;
	
	int x_max, y_max, x_min, y_min, centroid[2];
	
	cout<<"\nCOLUMS: "<<COLUMS<<"  ROWS: "<<ROWS<<" module_num: "<<module_num<<endl;
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
	   
	static_mod1.X_TL=x_min; 
	static_mod1.Y_TL=y_min;
	static_mod1.X_BR=x_max;
	static_mod1.Y_BR=y_max;
	
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
		
	if(module_num>0 )
	{
		rec_region_list[module_num-1].centroid[0]=centroid[0];
		rec_region_list[module_num-1].centroid[1]=centroid[1];
	}
	
	else if(module_num==0)
	{
		rec_region_list[0].centroid[0]=centroid[0];
		rec_region_list[0].centroid[1]=centroid[1];
		rec_region_list[module_2-1].centroid[0]=centroid[0];
		rec_region_list[module_2-1].centroid[1]=centroid[1];
	}
	
	
	cout <<endl<< "module "<<module_num<<"  cordinates: (" << static_mod1.X_TL<<","<<static_mod1.Y_TL<<"),("<<static_mod1.X_BR<<","<<static_mod1.Y_BR<<")"<<"   centoid ("<<static_reg.centroid[1]<<
																","<<static_reg.centroid[0]<<")"<<endl;
	 
	outputfile <<endl<< "module "<<module_num<<"  cordinates: (" << static_mod1.X_TL<<","<<static_mod1.Y_TL<<"),("<<static_mod1.X_BR<<","<<static_mod1.Y_BR<<")"<<"   centoid ("<<static_reg.centroid[1]<<
																","<<static_reg.centroid[0]<<")"<<endl;
	
	/*cout<<endl<<" RR1 details clb: "<<rec_region_list[0].clb<<"  bram: "<<rec_region_list[0].bram<<" dsp: "<<rec_region_list[0].dsp<<" centroid: ("<<rec_region_list[0].centroid[1]<<","<<rec_region_list[0].centroid[0]<<")"<<endl;
	cout<<endl<<" RR2 details clb: "<<rec_region_list[1].clb<<"  bram: "<<rec_region_list[1].bram<<" dsp: "<<rec_region_list[1].dsp<<" centroid: ("<<rec_region_list[1].centroid[1]<<","<<rec_region_list[1].centroid[0]<<")"<<endl;
	cout<<endl<<" RR3 details clb: "<<rec_region_list[2].clb<<"  bram: "<<rec_region_list[2].bram<<" dsp: "<<rec_region_list[2].dsp<<" centroid: ("<<rec_region_list[2].centroid[1]<<","<<rec_region_list[2].centroid[0]<<")"<<endl;
	cout<<endl<<" RR4 details clb: "<<rec_region_list[3].clb<<"  bram: "<<rec_region_list[3].bram<<" dsp: "<<rec_region_list[3].dsp<<" centroid: ("<<rec_region_list[3].centroid[1]<<","<<rec_region_list[3].centroid[0]<<")"<<endl;*/
	return(static_reg);
	 
	
}


void allocate_recon_module(rec_reg RR1 , int module_num )
{
	int clb_cnt, bram_cnt, dsp_cnt, dsp_col_num, bram_col_num, clb_b_dsp_bram, y_min_module=0, row_num=0, first_ele, bram_colm_1, bram_colm_2, dsp_alloted=0, bram_alloted=0,dsp_fpga_row=0, bram_fpga_row=0;
	int	bram_col_used=0, num_bram_col=1;
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
		vector <int> bram_col_after_iob;
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
bram_find:  if(my_fpga[y_min_module][bram_colm_1].usage==true)
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
		dsp_fpga_row=y_min_module;
		cout<<endl<<" y_min_module: "<<y_min_module<<endl;
		for(unsigned int i=y_min_module; i<ROWS; i++)   
		{
			dsp_alloted=dsp_alloted+dsp_per_tile;
			dsp_fpga_row++;
			cout<<"dsp allotted: "<<dsp_alloted<<" i= "<<i<<"  j= "<<dsp_col_num<<" module num:  "<<module_num<<endl;
			my_fpga[i][dsp_col_num].usage=true;   
			my_fpga[i][dsp_col_num].module_num =module_num;   
			
			if(dsp_alloted>=dsp_cnt)
			{
				break;
			}
		
		}    
		
		cout<<endl<<" dsp alloted: "<<dsp_alloted;
		
	}
	else if (dsp_cnt==0)
	{
		dsp_col_num=0;
	}
	
	//////****************************************** BRAM ALLOCATION  FOR RECONFIGURABLE REGION*********************************////
	
	if(bram_cnt>0)
	{	
		bool bram_col_end=false;
		it=bram_col.begin(); 
		
		if (dsp_cnt>0)
		{
			
			
			cout << "\n [" << (*itm).first << ':' << (*itm).second << ']';
			cout << "\n [" << (*rit).first << ':' << (*rit).second << ']';
			cout<<endl;
			 
			 cout<<endl<<"first element: "<<first_ele<<endl;
			 
				bram_col_used++;
				bram_fpga_row=y_min_module;
				for(unsigned int i=y_min_module; i<ROWS; i++)
				{
					bram_alloted=bram_alloted+bram_per_tile;
					bram_fpga_row++;
					cout<<"bram allotted: "<<bram_alloted<<" i= "<<i<<"  j= "<<first_ele<<" module num:  "<<module_num<<"  dsp_fpga_row: "<<dsp_fpga_row<<"  bram_fpga_row: "<<bram_fpga_row<<endl;
					my_fpga[i][first_ele].usage=true;   
					my_fpga[i][first_ele].module_num =module_num ;  
					
					if(bram_fpga_row>= dsp_fpga_row && bram_col_used<=1)
					{
						bram_col_used++;
						cout<<endl<<"i: "<<i<<endl;
						bram_fpga_row=y_min_module;
						itm=dist_vec_map.begin();
						++itm;
						i=y_min_module-1;
						first_ele=itm->second;
						bram_col_num=first_ele;
					}
					
					else if(bram_fpga_row>= dsp_fpga_row && bram_col_used>1 && bram_alloted<bram_cnt)
					{
						bram_col_end=true;
						break;
					}
			
					if(bram_alloted>=bram_cnt) 
					{
						bram_col_end=false;
						cout<<endl<<" BRAM ALLOTED: "<<bram_alloted<<endl;
						break;
					}
				}
				
			
			unsigned int row_num=bram_fpga_row-1;
			
			cout<<endl<<" size of dist vec bram: "<<dist_vec_map.size()<<endl;
			if(bram_col_end==true)
			{
				int p=0;
		R3:		p=0;	
				row_num= bram_fpga_row;
				for(itm=dist_vec_map.begin();itm!=dist_vec_map.end();++itm)
				{
					p++;
					bram_col_num=itm->second;
					bram_alloted=bram_alloted+bram_per_tile;
					cout<<"bram allotted: "<<bram_alloted<<" i= "<<row_num<<"  j= "<<bram_col_num<<" module num:  "<<module_num<<"  dsp_fpga_row: "<<dsp_fpga_row<<"  bram_fpga_row: "<<bram_fpga_row<<endl;
					my_fpga[row_num][bram_col_num].usage=true;   
					my_fpga[row_num][bram_col_num].module_num =module_num ;  
					
					if(bram_alloted>=bram_cnt || p==2) 
					{
						break;
					}
				}
				
				if(bram_alloted<bram_cnt) 
				{
					bram_fpga_row++;
					goto R3;
				}
				
			}
			
			
			bram_col_num=first_ele;
		}
		
		else if(dsp_cnt==0)
		{
			int row_prev=0;
			int row_num=0; 
			int count=0;
			int iob_found=0, iob_col_cnt=0, second_ele=0;
		
			vector<int> :: iterator itt;
			for(unsigned int i=1; i<COLUMNS; i++)
			{
				if(my_fpga[0][i].rec_type==rec_iob)
				{
					iob_found=1;
					iob_col_cnt++;
				}
				
				if(iob_found==1 && my_fpga[0][i].rec_type==rec_bram && iob_col_cnt<2 && iob_col_cnt>0)
				{
					cout<<endl<<"bram col number: "<<i<<"  iob col cnt: "<<iob_col_cnt;
					bram_col_after_iob.push_back(i);
					bram_col_num=i;
				}
			}
			
			
			it= bram_col_after_iob.begin();
			first_ele=*it;
			it=(bram_col_after_iob.begin()+1);
			second_ele=*it;
			cout<<endl<<" size of bram after iob: "<<bram_col_after_iob.size()<<" first_ele: "<<first_ele<<"  second ele: "<<second_ele<<endl;
			for (it=bram_col_after_iob.begin(); it!=bram_col_after_iob.end(); ++it)
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
							it=(bram_col_after_iob.begin()+1);
							second_ele=*it;
						}
						
						else
						{
							row_num=filled_region.y_max+1;
							y_min_module=row_num;
							first_ele=*it;
							it=(bram_col_after_iob.begin()+1);
							second_ele=*it;
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
					first_ele=bram_col_after_iob[0];
					row_prev=row_prev;
					y_min_module=row_prev;
				  }
					
				else if(row_prev>row_num)
				  {
					cout<<endl<<"row_prev>row_num"<<endl;
					first_ele=*it;
					it=(bram_col_after_iob.begin()+1);
					second_ele=*it;
					row_prev=row_num;
					y_min_module=row_prev;
				  }
				}
		
			}
			
			bram_col_num=first_ele;
			
			cout<<endl<<"-----bram_col_num=first_ele:   "<<first_ele<<" y_min_module:  "<<y_min_module<<endl;    
			bram_fpga_row=y_min_module;
			for(unsigned int i=y_min_module; i<ROWS; i++)
			{
				if (bram_alloted>=bram_cnt)
				{
					break;
				}
				
				bram_alloted=bram_alloted+bram_per_tile;
				bram_fpga_row++;
				cout<<"bram allotted:  "<<bram_alloted<<"  first ele: "<<bram_alloted<<" i= "<<i<<"  j= "<<first_ele<<" module num:  "<<module_num<<"bram count: "<<bram_cnt<<endl;
				my_fpga[i][first_ele].usage=true;   
				my_fpga[i][first_ele].module_num =module_num ;

				if( bram_alloted<bram_cnt)
				{
					num_bram_col=2;
					bram_alloted=bram_alloted+bram_per_tile;
					cout<<"bram allotted  "<<bram_alloted<<" second ele: "<<bram_alloted<<" i= "<<i<<"  j= "<<second_ele<<" module num:  "<<module_num<<endl;
					my_fpga[i][second_ele].usage=true;   
					my_fpga[i][second_ele].module_num =module_num ; 
				}
				
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
			clb_cnt=RR1.clb;
			clb_alloted=0;
			bram_cnt=RR1.bram;
			
			clb_alloted=allocate_clb_both_bram_dsp_greater_then_0(RR1,dsp_col_num, bram_col_num, bram_col_used, y_min_module, dsp_fpga_row,  bram_fpga_row);
			
			cout<<endl<<" ------------------------- module number "<<module_num<< " --------------------------------cccccccccc llllllllllll bbbbbbbbbbbbb alloted: "  <<clb_alloted<<endl;
			filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false, module_num);
		}

		else if(dsp_cnt>0 && bram_cnt==0)
		{
			int num_cols_reqd= clb_cnt/dsp_cnt;
			int clb_last_col=clb_cnt%dsp_cnt;
			bool bram_col_found= false;
			
			int col_count=0; 
			int clb_placed=0;
			int clb_left=0;
			it=bram_col.begin();
			int ele=*it;
			cout<<endl<<"  clb count: "<<clb_cnt<<" clb alloted: "<<clb_alloted<<"  dsp col num: "<<dsp_col_num<<"y_min_module: "<<y_min_module<<" RR1.bram: "<<RR1.bram <<"  module num: " <<RR1.module_num<<" dsp fpga row: "<<dsp_fpga_row<<" bram fpga row: "<<bram_fpga_row<<endl;                
			clb_allot_dsp_or_bram_g_t_0(clb_cnt, 0, clb_alloted, rec_dsp, dsp_col_num, y_min_module, RR1.bram, false, RR1.module_num, dsp_fpga_row, bram_fpga_row);
			filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false, module_num);

			
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
			//clb_allot_dsp_or_bram_g_t_0(clb_cnt, bram_cnt, clb_alloted, rec_bram, rec_dsp, y_min_module, dsp_cnt, right_edge_found, bram_col_num, module_num );
			
			it=bram_col_after_iob.begin();
			first_ele=*it;
			
			it=bram_col_after_iob.begin()+1;
			int second_ele=*it;
			
			clb_alloted=allocate_clb_both_bram_dsp_greater_then_0(RR1, second_ele, first_ele, 1, y_min_module, bram_fpga_row,  bram_fpga_row);
			cout<<endl<<" CLB ALLOTED: "<<clb_alloted<<endl;
			filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false, module_num);
			
			
		}
		
		else if (dsp_cnt==0 && bram_cnt==0)
		{
			print_fpga(ROWS, COLUMNS);
			allocate_only_clb(RR1,  module_num);			
		}
	}
	
	else if (clb_cnt==0)
	{
		int clb_alloted=0;
		filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false, module_num);
	}
	RR_count++;
	
	
}


int allocate_clb_both_bram_dsp_greater_then_0(rec_reg RR1, int dsp_col_num, int bram_col_num, int bram_col_used, int y_min_module, int dsp_fpga_row, int bram_fpga_row)
{
	int clb_alloted=0, module_num=0;
	int clb_b_dsp_bram=abs(dsp_col_num-bram_col_num)-(bram_col_used-1);
	int clb_cnt=RR1.clb; 
	int bram_cnt=RR1.bram; 
	int dsp_cnt=RR1.dsp; 
	module_num=RR1.module_num;
	cout<<"\n dsp count: "<<dsp_cnt<<" bram cnt : "<<bram_cnt<<" clb cnt: "<<clb_cnt<<endl;
	cout<<"number of clbs between bram & dsp cols: "<<((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row)-y_min_module)*clb_per_tile)<<endl;
	cout<<" number of rows: "<<(max(bram_fpga_row, dsp_fpga_row)-y_min_module)<<endl;
	
	if (clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row)-y_min_module)*clb_per_tile))
	{   
		int source=0, dest=0;
		cout<<endl<<" (clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row))*clb_per_tile) "<<endl;
		
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
		
		cout<<endl<<" y_min_module: "<<y_min_module<<"  max(bram_fpga_row, dsp_fpga_row): "<<max(bram_fpga_row-1, dsp_fpga_row-1)<<endl;
		for(unsigned int i=source; i<dest; i++)
		{
			if(my_fpga[y_min_module][i].rec_type!=rec_clb)
			{
				continue;
			}
			for(unsigned int j=y_min_module; j<=max(bram_fpga_row-1, dsp_fpga_row-1); j++)
			{
				clb_alloted= clb_alloted+clb_per_tile;
				cout<<endl<<" clb allted row: "<<j<<" column: "<<i<<" clb alloted: "<<clb_alloted;
				my_fpga[j][i].usage=true;
				my_fpga[j][i].module_num =module_num ;
				if(clb_alloted>=clb_cnt)
				{
					break;
				}
			}
			
			if(clb_alloted>=clb_cnt)
			{
				break;
			}
		}
	}
	
	else if(clb_cnt>((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row)-y_min_module)*clb_per_tile))		
	{
		int source=0, dest=0;
		cout<<endl<<" clb_cnt>((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row)-y_min_module)*clb_per_tile)"<<endl;
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
			if(my_fpga[y_min_module][i].rec_type!=rec_clb)
			{
				continue;
			}
			for(unsigned int j=y_min_module; j<=max(bram_fpga_row-1, dsp_fpga_row-1); j++)
			{
				clb_alloted= clb_alloted+clb_per_tile;
				my_fpga[j][i].usage=true; 
				my_fpga[j][i].module_num =module_num ; 
				if(clb_alloted>=clb_cnt)
				{
					break;
				}
			}
			if(clb_alloted>=clb_cnt)
			{
				break;
			}
		}
	
		int clb_left= clb_cnt-((clb_b_dsp_bram-1)*(max(bram_cnt, dsp_cnt)));
		int num_cols_reqd= (clb_left)/max(bram_cnt, dsp_cnt);
		int clb_last_col= (clb_left)%(max(bram_cnt, dsp_cnt));
	
		cout<<" number of colums reqd: "<<num_cols_reqd<<"  dsp column no: "<<dsp_col_num<<" last colum cordinate:" <<(dsp_col_num+1+num_cols_reqd)<<endl;
	
	
		for(unsigned int n=max(dsp_col_num, bram_col_num)+1; n<COLUMNS; n++)
		{
			if(my_fpga[y_min_module][n].rec_type!=rec_clb)
			{
				break;
			}
			for(unsigned int j=y_min_module; j<=max(bram_fpga_row-1, dsp_fpga_row-1); j++)
			{
				if(my_fpga[j][n].rec_type!=rec_clb)
				{
					break;
				}
				if(clb_alloted>=clb_cnt)
				{
					break;
				}
				
				clb_alloted= clb_alloted+clb_per_tile;
				my_fpga[j][n].usage=true; 
				my_fpga[j][n].module_num =module_num ;
					
				
			}
				
			if(clb_alloted>=clb_cnt)
			{
				
				break;
			}
		}
		
		for(unsigned int n=min(dsp_col_num, bram_col_num)-1; n>0; n--)
		{
			if(my_fpga[y_min_module][n].rec_type!=rec_clb)
			{
				int clb_left=clb_cnt-clb_alloted;
				int y_cord= max(bram_fpga_row, dsp_fpga_row)+1;
				resource rec= rec_dsp;
				int col_num= n+1;
				extend_vertically(clb_cnt, clb_left, y_cord, col_num,  rec, clb_alloted, 0);
				break; 
			}
			for(unsigned int j=y_min_module; j<=max(bram_fpga_row-1, dsp_fpga_row-1); j++)
			{
				if(my_fpga[j][n].rec_type!=rec_clb)
				{
					break;
				}
				if(clb_alloted>=clb_cnt)
				{
					break;
				}
				
				clb_alloted= clb_alloted+clb_per_tile;
				my_fpga[j][n].usage=true; 
				my_fpga[j][n].module_num =module_num ;
			}
				
			if(clb_alloted>=clb_cnt)
			{
				break;
			}
		}
	}
	cout<<"clb count: "<<clb_cnt<<" clb_alloted: "<<clb_alloted<<endl;
		return(clb_alloted);
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


/////**************************white space based clb allocation******************************//////

void allocate_only_clb(rec_reg RR1 , int module_num)
{
	int clb_cnt, rec_wast_cost=0, clb_alloted=0, clb_diff=0, diff_centroid=0;
	extern vector <white_space> white_space_list ;
	vector <white_space> :: iterator it;
	multimap <int, white_space> map_ws;
	multimap <int, white_space> :: iterator it_m;
	white_space WS1;
	clb_cnt=RR1.clb;
	
	for (it=white_space_list.begin(); it!=white_space_list.end(); ++it)
	{
		WS1=*it;
		clb_diff=(WS1.clb_free*clb_per_tile)-clb_cnt;
		
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
				clb_alloted=clb_alloted+clb_per_tile;
				cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
				my_fpga[j][i].usage=true; 
				my_fpga[j][i].module_num =module_num ; 
			}
			
		}	
	}
	
	filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false, module_num);
	cout<<endl<<"allocate_only_clb "<<clb_alloted<<endl;
	white_space_list.clear();
	cout<<endl<<" white space list cleared"<<endl;
	print_fpga(ROWS, COLUMNS);
	calculate_white_space(ROWS, COLUMNS);
	
	cout<<endl<<" size of white space list:  "<<white_space_list.size()<<endl;
	//filled_reg SR=  calculate_module_cordinate(ROWS, COLUMNS, module_num, false);
}


void clb_allot_dsp_or_bram_g_t_0(int clb_cnt, int rec_cnt,int clb_alloted, resource rec_main, int dsp_col_num, int y_min_module, int bram_cnt, bool right_edge_found, int module_num, int dsp_fpga_row, int bram_fpga_row)
{
	int dsp_cnt= rec_cnt;
	
	bool bram_col_found= false;
	bool locked_clb_found=false;
			
	int col_count=0; 
	int clb_placed=0;
	int clb_left=0;
			
	cout<<endl<<" inside clb allocation"<<endl;
			for(unsigned int i=dsp_col_num+1; i<COLUMNS; i++)
			{
				col_count++;

				if(clb_alloted>=clb_cnt)
				{
					break;
				}
				
				if(my_fpga[y_min_module][i].module_num>-1)
				{
					
					cout<<endl<<"locked_clb_found=true"<<endl;
					locked_clb_found=true;
					break;
				}
				
				if(my_fpga[y_min_module][i].rec_type!=rec_clb)
				{
					cout<<endl<<"bram_col_found=true"<<endl;
					bram_col_found=true;
					break;
				}
				 
				for(unsigned int j=y_min_module; j<=max(bram_fpga_row-1, dsp_fpga_row-1); j++)
				{
					clb_alloted=clb_alloted+clb_per_tile;
					cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<" module num:  "<<module_num<<endl;
					my_fpga[j][i].usage=true; 
					my_fpga[j][i].module_num =module_num ; 
					
					if(clb_alloted>=clb_cnt)
					{
					break;
					}
				
				}	
				
				
			}
			
			
				
			for(signed int i=dsp_col_num-1; i> 0; i--)
			{
				if(clb_alloted>=clb_cnt)
				{
					break;
				}
					
				
				if (i<1)
				{ 
					int clb_left= clb_cnt-clb_alloted;
					int y_cord= y_min_module+dsp_cnt;
					int col_num= 0;
					resource rec= rec_main;
					extend_vertically(clb_cnt, clb_left, y_cord, col_num, rec, clb_alloted, module_num );
				}
				
				if(my_fpga[y_min_module][i].rec_type!=rec_clb)
				{
					continue;
				}
				
				if(my_fpga[y_min_module][i].module_num <0 && my_fpga[y_min_module][i].rec_type==rec_clb)
				{
					for(unsigned int j=y_min_module; j<y_min_module+(max(bram_fpga_row-1, dsp_fpga_row-1)); j++)
					{
						clb_alloted=clb_alloted+clb_per_tile;
						cout<<"clb allotted: "<<clb_alloted<<" i= "<<i<<"  j= "<<j<<endl;
						my_fpga[j][i].usage=true; 
						my_fpga[j][i].module_num =module_num ; 
						if(clb_alloted>=clb_cnt)
						{
							cout<<endl<<"clb allotted 124:  "<<clb_alloted<<endl;
							break;
						}
					}
				}
			}
				
		if(clb_alloted<clb_cnt)
		{
			extend_vertically(clb_cnt, clb_left, y_min_module, 1, rec_dsp, clb_alloted, module_num);
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
