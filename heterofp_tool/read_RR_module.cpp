#include "read_RR_module.h"
#include"general.h"
#include <string>

vector <rec_reg> rec_region_list;

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


void read_rec_module(string myfile_name)
{
	string line;
	int line_cnt=0;
	int mod_count=0, count=0;
	map < int, int> mymap;
	int num_clb, num_bram, num_dsp, num_modules;
	rec_reg RR1;
	//ifstream module_file;
	ifstream myfile1;
	ifstream myfile2;
	int clb_all_rr=0,bram_all_rr=0,dsp_all_rr=0, module_num=0;
    myfile1.open(myfile_name.c_str(), ios::out);
    myfile2.open(myfile_name.c_str(), ios::out);
	
	if (myfile1.is_open())
	{
		while ( getline (myfile1, line) )
		{
			std::istringstream iss(line); 
            int  num;
			
			line_cnt++;
			
			if(line_cnt==2)
			{
				iss >> num;
				num_modules=num;
				
			}
			
			if(line_cnt>2 && line_cnt<=(2+num_modules))
			{
				iss>>num_clb>>num_bram>>num_dsp;
				count++;
				std::ostringstream ss;
				ss << count;
				std::string rr_name = "RR " + ss.str();
				//std::cout << o.str() << std::endl;
				RR1.name=rr_name;
				RR1.clb=num_clb;
				RR1.bram=num_bram;
				RR1.dsp=num_dsp;
				RR1.module_num=count;
				clb_all_rr=clb_all_rr+RR1.clb;
				bram_all_rr=bram_all_rr+RR1.bram;
				dsp_all_rr=dsp_all_rr+RR1.dsp;
				rec_region_list.push_back(RR1);
			}
		}
		myfile1.close();
	}
	
	int line_cnt2=0;
	vector<int> :: iterator it;
	vector<int> :: iterator it2;
	map < int, int> ::iterator it_m;
	string nodeline;
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
							
							if(val1==val2)
							{
								continue;
							}
							
							it_m =rec_region_list[val1].map_wires.find(val2);
							if (it_m != rec_region_list[val1].map_wires.end())
							{
								int key= val2;
								int value=it_m->second;
								value++;
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
	
	cout<<"\n clb in RR: "<<clb_all_rr<<" bram all rr: "<<bram_all_rr<<" dsp all rr:  "<<dsp_all_rr<<endl;
	
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


