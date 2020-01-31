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
#include <algorithm>
#include <sstream>
#include <numeric>
#include <iterator>
#include <time.h>

using namespace std;

typedef struct
{
	int module_num;
	map < int, int> map_wires;
} rec_reg;  

	vector <rec_reg> rec_region_list;
	
	
int main()
{
	vector <string> netdata;
	vector <string> node_name;
	string line;
	string nodeline;
	int line_cnt=0, clb=0,bram=0,dsp=0, count=-1;
	int num_modules=0;
	ofstream outputfile;
	outputfile.open ("n300anew.txt");
	ifstream myfile1 ("n300anew.fpga");
	ifstream myfile2 ("n300anew.fpga");
	
	outputfile<<"### resource file ##################"<<endl;
	rec_reg RR1;
			
	if (myfile1.is_open())
	{
		while ( getline (myfile1,nodeline) )
		{
			std::istringstream iss(nodeline); 
            int  num;
			
			line_cnt++;
			
			if(line_cnt==2)
			{
				iss >> num;
				num_modules=num;
				outputfile<<"num_module "<<num_modules<<endl;
			}
			
			if(line_cnt>2 && line_cnt<=(2+num_modules))
			{
				iss>>clb>>bram>>dsp;
				count++;
				RR1.module_num=count;
				rec_region_list.push_back(RR1);
				outputfile<<"RR"<<count<<" "<<clb<<" "<<bram<<" "<<dsp<<endl;
			}
		}
		myfile1.close();
	}
	
	
	outputfile<<endl<<"##### wire connections ################"<<endl<<"x ";
	
	for (unsigned int i=0; i<num_modules; i++)
	{
		outputfile<<"RR"<<i+1<<" ";
	}
	
	int line_cnt2=0;
	vector<int> :: iterator it;
	vector<int> :: iterator it2;
	map < int, int> ::iterator it_m;

	ostream_iterator< int > output( cout, "  " );
	int val1=0, val2=0;
		
	cout<<endl<<"size of rec region list: "<<rec_region_list.size()<<endl;
	if (myfile2.is_open())
	{
		while ( getline (myfile2,nodeline) )
		{
			std::istringstream buf(nodeline); 
            int  num;
			
			line_cnt2++;
			//cout<<endl<<line_cnt2;
			if(line_cnt2>=num_modules+6)
			{
				
				vector<int> vec;
				vec.clear();
				copy(istream_iterator<int>(buf),istream_iterator<int>(), back_inserter(vec));
				
				//cout << " Vector vec contains: ";
				//copy( vec.begin(), vec.end(), output );
				//cout<<" size of vector: "<<vec.size()<<endl;
				
				if(vec.size()>1)
				{
					for (it=vec.begin(); it!=vec.end(); ++it)                                              
					{
						val1=*it;
						
						for (it2=vec.begin(); it2!=vec.end(); ++it2)
						{
							
							val2=*it2;
							
							cout<<endl<<" val1: "<<val1<<" val2: "<<val2;
							
							if(val1==val2)
							{
								continue;
							}
							/*typedef struct
							{
							int module_num;
							map < int, int> map_wires;
							} rec_r*/
							
							
							it_m =rec_region_list[val1].map_wires.find(val2);
							if (it_m != rec_region_list[val1].map_wires.end())
							{
								int key= val2;
								int value=it_m->second;
								value++;
								cout<<endl<<" value: "<<value;
								rec_region_list[val1].map_wires.erase (val2);
								rec_region_list[val1].map_wires.insert ( std::pair<int,int>(key,value) );
							}
							
							else
							{
								int key= val2;
								int value=1;
								rec_region_list[val1].map_wires.insert ( std::pair<int,int>(key,value) );
							}
						}
							
					}
				}
				
			}
			
		}
		myfile2.close();
	}
	outputfile<<endl;
	
	it_m =rec_region_list[1].map_wires.find(3);
	int value=it_m->second;
	
	cout<<endl<<" between 1 and 3: "<<value;
	
	it_m =rec_region_list[2].map_wires.find(7);
	value=it_m->second;
	
	cout<<endl<<" between 2 and 7: "<<value;
	
	cout<<endl;
	
	return 0;
}
























