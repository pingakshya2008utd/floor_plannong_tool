#include "white_space.h"
#include"general.h"

vector <white_space> white_space_list ;

void calculate_white_space(int ROWS, int COLUMS)
{

	int x_min_init, x_max_init, x_min_curr, x_max_curr, curr_module_num=-1, col_min, col_max, j, x_max_present, x_max_past, curr_module_min=1, cnt;
	white_space ws1;
//	extern vector <filled_reg> filled_region_cords;
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
		cout<<endl<<"fpga "<<j<<","<<i<<"  "<<my_fpga[j][i].module_num<<endl;
			if(i>=COLUMS)
			{
				if(row_cnt==0)
				{
					x_max_past=i-1;
					cout<<"\n i> column row_cnt===0 && row J= "<<j<<" rown count: "<<row_cnt;
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
					cout<<"\ni<column row_cnt===0 && row J= "<<j<<" rown count: "<<row_cnt;
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
				cout<<endl<<" row= "<<j<<" column i= "<<i<< " RR max past:  "<<x_max_past<<"  RR max present: "<<x_max_present<<" module number: "<<my_fpga[j][i].module_num;
				
				if(row_cnt==0)
				{
					if(cnt==0)
					{
						x_max_past=i;
						row_cnt++;
						col_min=RR1.x_max+1;
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
					}
					
					else
					{
						x_max_past=i-1;
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
					}
					goto L6;
				}
				
				else if(row_cnt>0 && x_max_present==x_max_past )
				{
					j=j+1;
					col_min=RR1.x_max+1;
					cout<<endl<< "RR max past:  "<<x_max_past<<"  RR max present: "<<x_max_present;
					if(cnt==0)
					{
						x_max_past=i;
					}
					else if(cnt>0)
					{
						x_max_past=i-1;
					}
					
					cout<<"\n  row_cnt>0 && x_max_present==x_max_past row j= "<<j<<"  RR1.ymax:  "<<RR1.y_max<<" curr_module_num:  "<<curr_module_num<<endl;
					
					if(j>ROWS)
					{
						cout<<endl<<" j>ROWS module num: "<<RR1.module_num<<endl;
						break;
					}
					
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
						cout<<"\n break curr_module_num: "<<curr_module_num<<endl;
						break;
					}
					goto L6;
				}
				
				else if(x_max_present!=x_max_past)
				{
					j=j+0;
					x_max_past=i-1;
					cout<<endl<<"  -----------count: "<<cnt<<"  row_cnt: "<<row_cnt<<" curr: "<<curr_module_num<<endl;
					for(unsigned int p=col_min; p<x_max_past+1; p++)
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
					
					row_cnt=0;
					goto L6;
				}
			}
		}
		
	
	
	}
	
	curr_module_min=curr_module_num;
	cout<<"\n curr modulef min: "<<curr_module_min<<endl;
	cout<<"bye bye white space  "<<endl;
	
	calculate_white_space_cordinate(ROWS, COLUMNS,curr_module_min);
}


void calculate_white_space_cordinate(int ROWS, int COLUMS,  int curr_module_min)
{
	filled_reg FR1;
	white_space WS1;
	vector <white_space>:: iterator it;
	
	cout<<endl<<" in calculate_white_space_cordinate  curr_module_min: "<<curr_module_min<<" size of white space list: "<<white_space_list.size()<<endl;
	for(int module_num=curr_module_min; module_num<-1; module_num++)
	{
		
		cout<<endl<<"module num:  "<<module_num;
		
		
		WS1=calculate_ws_cordinate(ROWS, COLUMNS, module_num);
		//FR1=calculate_module_cordinate(ROWS, COLUMNS, module_num, false);
		
		white_space_list.push_back(WS1);
	}
	
	cout<<endl<<" size of white space list: "<<white_space_list.size()<<endl;
	for (it=white_space_list.begin(); it!=white_space_list.end(); ++it)
	{
		WS1=*it;
		cout<<endl<<"white space module: "<<WS1.module_num<<" ("<<WS1.x_min<<","<<WS1.y_min<<"),("<<WS1.x_max<<","<<WS1.y_max<<")"<< " clb free: "<<WS1.clb_free<<"  bram free:  "<<WS1.bram_free<<" dsp free:  "<<WS1.dsp_free;
	}
	
	cout<<endl;
}

white_space calculate_ws_cordinate(int ROWS, int COLUMS,  int module_num)
{
	vector<int> x_cor;
	vector<int> y_cor;
	vector<int>::const_iterator it2;  
	white_space static_mod2;
	int x_max, y_max, x_min, y_min;
	
	cout<<endl<<" white space module number: "<<module_num<<endl;
	
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
	
	cout<<" white space module number 2: "<<module_num<<" size of x_cor: "<<x_cor.size()<<" size of y_cor: "<<y_cor.size()<<endl;
	
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
	
	   
	static_mod2.x_min=x_min; 
	static_mod2.y_min=y_min;
	static_mod2.x_max=x_max;
	static_mod2.y_max=y_max;
	static_mod2.module_num=module_num;
	static_mod2.clb_free=clb_count;
	static_mod2.bram_free=bram_count;
	static_mod2.dsp_free=dsp_count;
	cout<<"white space returned"<<endl;
	return(static_mod2);
}
