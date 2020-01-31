#include "allocate_recon_module.h"





void allocate_static_module(rec_reg RR1, int module_num)
{
	int clb_cnt, bram_cnt, dsp_cnt, dsp_col_num, bram_col_num, clb_b_dsp_bram, dsp_alloted=0, bram_alloted=0, clb_alloted=0, bram_fpga_row=0, dsp_fpga_row=0, bram_col_used=0, iob_found=0;
	clb_cnt= RR1.clb;
	bram_cnt= RR1.bram;
	dsp_cnt=RR1.dsp;
	vector <int> :: iterator it;
	map <int, int> dist_vec_map;
	map <int, int>:: iterator itm;
	cout<<endl<<"clb cnt "<<clb_cnt;
	cout<<endl<<"bram cnt "<<bram_cnt;
	cout<<endl<<"dsp cnt "<<dsp_cnt<<endl;
	cout<<"static module number: "<<RR1.module_num<<endl;
	
	
	if(dsp_cnt>0)
	{
		for (it=dsp_col.begin(); it!=dsp_col.end(); ++it)
		{
			for(unsigned int i=0; i<dsp_cnt; i++)
			{
				dsp_alloted=dsp_alloted+dsp_per_tile;
				dsp_fpga_row=i;
				my_fpga[i][*it].usage=true;   
				my_fpga[i][*it].module_num =module_num ;	
				
				if(dsp_alloted>=dsp_cnt)
				{
					break;
				}
			}
			if(dsp_alloted>=dsp_cnt)
			{
				dsp_col_num=*it;
				break;
			}
			
		}

		cout<<endl<<" DSP ALLOTED: "<<dsp_alloted<<endl;
	}
	else if (dsp_cnt==0)
	{
		dsp_col_num=0;
	}

    
	if(bram_cnt>0)
	{
		if (dsp_cnt>0)
		{
			bool bram_col_end=false;
			
			for (it=bram_col.begin(); it!=bram_col.end(); ++it)
			{
				int dist=abs(dsp_col_num-*it);
				dist_vec_map.insert ( std::pair<int,int>(dist,*it) );
			}
			int first_ele=dist_vec_map.begin()->second;
			bram_col_used++;
			
			for(unsigned int i=0; i<ROWS; i++)
			{
				bram_alloted=bram_alloted+bram_per_tile;
				bram_fpga_row=i;
				my_fpga[i][first_ele].usage=true;   
				my_fpga[i][first_ele].module_num =module_num ;  
				cout<<endl<<"bram_fpga_row: "<<bram_fpga_row<<"  dsp_fpga_row: "<<dsp_fpga_row<<"  bram alloted: "<<bram_alloted;
				
				if(bram_fpga_row>= dsp_fpga_row && bram_col_used<=1)
				{
					bram_col_used++;
					cout<<endl<<"i: "<<i<<endl;
					bram_fpga_row=0;
					itm=dist_vec_map.begin();
					++itm;
					i=-1;
					first_ele=itm->second;
					bram_col_num=first_ele;
				}
				
				else if(bram_fpga_row>= dsp_fpga_row && bram_col_used>1 && bram_alloted<bram_cnt )
				{
					cout<<endl<<" BRAM ALLOTED bram_fpga_row: "<<bram_alloted<<endl;
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
			
			unsigned int row_num=bram_fpga_row+1;
			
			if(bram_col_end==true)
			{
			
		R3:		row_num= bram_fpga_row;
				for(itm=dist_vec_map.begin();itm!=dist_vec_map.end();++itm)
				{
					bram_col_num=itm->second;
					bram_alloted=bram_alloted+bram_per_tile;
					bram_fpga_row++;
					my_fpga[row_num][bram_col_num].usage=true;   
					my_fpga[row_num][bram_col_num].module_num =module_num ;  
					
					if(bram_alloted>=bram_cnt) 
					{
						break;
					}
				}
				
				if(bram_alloted<bram_cnt) 
				{
					goto R3;
				}
				
			}
			
			cout<<endl<<" BRAM ALLOTED: "<<bram_alloted<<endl;
			bram_col_num=first_ele;
			
		}
		else if(dsp_cnt==0)
		{
			for(unsigned int i=1; i<COLUMNS; i++)
			{
				if(my_fpga[0][i].rec_type==rec_iob)
				{
					iob_found=1;
				}
				
				if(iob_found==1 && my_fpga[0][i].rec_type==rec_bram )
				{
					bram_col_num=i;
					break;
				}
			}
		
			for(unsigned int i=0; i<ROWS; i++)
			{
				bram_alloted=bram_alloted+bram_per_tile;
				bram_fpga_row++;
				my_fpga[i][bram_col_num].usage=true;   
				my_fpga[i][bram_col_num].module_num =module_num ;   
				if(bram_alloted>bram_cnt)
				{
					break;
				}
			}
			bram_col_num=bram_col_num;
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
			clb_b_dsp_bram=abs(dsp_col_num-bram_col_num)-(bram_col_used-1);
			cout<<"\n dsp count: "<<dsp_cnt<<" bram cnt : "<<bram_cnt<<endl;
			cout<<"number of clbs between bram & dsp cols: "<<((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row))*clb_per_tile)<<endl;
	
			if (clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row))*clb_per_tile))
			{   
		
				cout<<endl<<" clb_cnt<=((clb_b_dsp_bram-1)*(max(bram_fpga_row, dsp_fpga_row))*clb_per_tile))"<<endl;
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
					if(my_fpga[0][i].rec_type!=rec_clb)
					{
						continue;
					}
					for(unsigned int j=0; j<<=max(bram_fpga_row, dsp_fpga_row); j++)
					{
						clb_alloted= clb_alloted+clb_per_tile;
						my_fpga[j][i].usage=true;
						my_fpga[j][i].module_num =module_num ;
						if(clb_alloted>=clb_cnt)
						{
							break;
						}
					}
				}
			}
		
			else if(clb_cnt>((clb_b_dsp_bram-1)*max(bram_fpga_row, dsp_fpga_row)*clb_per_tile))		
			{
				int source=0, dest=0;
				cout<<endl<<" clb_cnt>((clb_b_dsp_bram-1)*max(bram_fpga_row, dsp_fpga_row)*clb_per_tile) "<<endl;
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
					if(my_fpga[0][i].rec_type!=rec_clb)
					{
						continue;
					}
					for(unsigned int j=0; j<=max(bram_fpga_row, dsp_fpga_row); j++)
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
					if(my_fpga[0][n].rec_type!=rec_clb)
					{
						break;
					}
					for(unsigned int j=0; j<=max(bram_fpga_row, dsp_fpga_row); j++)
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
					if(my_fpga[0][n].rec_type!=rec_clb)
					{
						int clb_left=clb_cnt-clb_alloted;
						int y_cord= max(bram_fpga_row, dsp_fpga_row)+1;
						resource rec= rec_dsp;
						int col_num= n+1;
						extend_vertically(clb_cnt, clb_left, y_cord, col_num,  rec, clb_alloted, 0);
						break;
					}
					for(unsigned int j=0; j<=max(bram_fpga_row, dsp_fpga_row); j++)
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
		}

		/*else if(dsp_cnt>0 && bram_cnt==0)
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
			
		}*/
	}
	cout<<endl<<" clb alloted: "<<clb_alloted<<endl;
	filled_reg SR=calculate_module_cordinate(ROWS, COLUMNS, 0, false, RR1.module_num);
}
