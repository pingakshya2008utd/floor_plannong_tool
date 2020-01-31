#include "fpga.h"
#include"general.h"

//int ROWS, COLUMN;
vector <clb> clb_vec;
vector <bram> bram_vec;
vector <dsp> dsp_vec;
vector <iob> iob_vec;
vector <int> clb_col;
vector <int> dsp_col;
vector <int> bram_col;
vector <int> iob_col;

//fpga my_fpga[20][64], temp_fpga[20][64], min_fpga[20][64];
//fpga my_fpga[16][64], temp_fpga[16][64], min_fpga[16][64];
fpga my_fpga[40][64], temp_fpga[40][64], min_fpga[40][64];
int num_modules, total_clb, total_bram, total_dsp, total_iob, clb_per_tile, bram_per_tile, dsp_per_tile, iob_per_tile;
long int current_cost;

void print_fpga(int ROWS, int COLUMS)
{
	cout<<endl;
	ofstream fpga_print_file;
	fpga_print_file.open ("fpga_print_file.txt");
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			if (my_fpga[j][i].rec_type==rec_clb)
			{
				//cout<<" clb"<<i<<","<<j;
				fpga_print_file<<" clb"<<i<<","<<j;
				//fpga_print_file<<" clb ";
				
			}
			else if (my_fpga[j][i].rec_type==rec_bram)
			{
				//cout<<"bram"<<i<<","<<j;
				//fpga_print_file<<" bram ";
				fpga_print_file<<"bram"<<i<<","<<j;
			}
			else if (my_fpga[j][i].rec_type==rec_dsp)
			{
				//cout<<"dsp"<<i<<","<<j;
				fpga_print_file<<"dsp"<<i<<","<<j;
				//fpga_print_file<<" dsp ";
			
			}
			else if (my_fpga[j][i].rec_type==rec_iob)
			{
				//cout<<"iob"<<i<<","<<j;
				fpga_print_file<<"  "<<i<<","<<j;
				//fpga_print_file<<"    ";
			
			}
			
			if (my_fpga[j][i].usage==true)
			{
				//cout<<" "<<my_fpga[j][i].module_num<<"L|";
				fpga_print_file<<" "<<my_fpga[j][i].module_num<<"L|";
			}
				
			else if (my_fpga[j][i].usage==false)
			{
				//cout<<"F|";
				fpga_print_file<<" "<<my_fpga[j][i].module_num<<"|";
			}	
			
			if (my_fpga[j][i].module_num>=0)
			{
				//cout<<" "<<my_fpga[j][i].module_num<<"L|";
				cout<<" "<<(my_fpga[j][i].module_num)/10<<(my_fpga[j][i].module_num)%10<<"L|";
				//fpga_print_file<<" "<<my_fpga[j][i].module_num<<"L|";
			}
				
			else if (my_fpga[j][i].module_num<0)
			{
				cout<<"   F|";
				//fpga_print_file<<" "<<my_fpga[j][i].module_num<<"|";
			}
		}
		
		cout<<endl;
		fpga_print_file<<endl;
	}
	fpga_print_file.close();
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
	total_iob=0;
	if (myfile1.is_open())
	{	
	while ( getline (myfile1,line) )
        {
			std::istringstream iss(line); 
			string res, clbnameis;
            int  num;
			
			line_cnt++;
		if (line_cnt>6)
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
				
				else if (res=="iob")
				{
					
					for (unsigned int i=0;i<num; i++)
					{
						iob io[ROWS];
						iob_col.push_back(col_count);
						for (unsigned int j=0; j<ROWS; j++)
						{
						io[j].row=j;
						io[j].usage=false;
						io[j].column=col_count;
						my_fpga[j][col_count].rec_type=rec_iob;
						my_fpga[j][col_count].rec_name="iob";
						my_fpga[j][col_count].module_num=-1;
						iob_vec.push_back(io[j]);
						total_iob++;
						}
						col_count++;
						
					}
				}
			}
			
		}
	}
		
		myfile1.close();
		cout<<endl<<"total clb: "<<total_clb*clb_per_tile<<"  total bram: "<<total_bram*bram_per_tile<<"   total dsp: "<<total_dsp*dsp_per_tile<<"   total iob: "<<total_iob*iob_per_tile<<endl; 
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
				//cout<<"rows  "<<ROWS<<endl;
				
				
			}
			
			else if (line_cnt==2)
			{
				iss >> res >> num;
				COLUMN=num;
				
				//cout<<"Columns   "<<COLUMN<<"\n"; 
			}
			else if (line_cnt>2 && line_cnt<7)
			{
				iss >> res >> num;
				if(res=="clb_per_tile")
				{
					clb_per_tile=num;
				}
				else if(res=="bram_per_tile")
				{
					bram_per_tile=num;
				}
				else if(res=="dsp_per_tile")
				{
					dsp_per_tile=num;
				}
				else if(res=="iob_per_tile")
				{
					iob_per_tile=num;
				}
				
				//cout<<"Columns   "<<COLUMN<<"\n"; 
			}
		}
	}
	
	myfile.close();
	cout<<"\n\nclb per tile: "<<clb_per_tile<<"\nbram per tile: "<<bram_per_tile<<"\ndsp per tile: "<<dsp_per_tile<<"\niob per tile: "<<iob_per_tile<<endl;
}


void clear_fpga(int ROWS, int COLUMS)
{
	
	for (unsigned int j=0; j<ROWS;j++)
	{
		for(unsigned int i=0; i<COLUMNS; i++)
		{
			temp_fpga[j][i]=my_fpga[j][i];
			/*if(my_fpga[j][i].module_num<0)
			{
				my_fpga[j][i].usage=false;
				my_fpga[j][i].module_num=-1;
			}
			else if(my_fpga[j][i].module_num==0)
			{
				my_fpga[j][i].usage=false;
				my_fpga[j][i].module_num=-1;
			}*/
			
			my_fpga[j][i].usage=false;
			my_fpga[j][i].module_num=-1;
			
		}
	}
}

 
