#include "calculate_hpwl.h"

int calculate_hpwl(string myfile_name) 
{
	
	rec_reg RR1;
	vector <int> x_cord;
	vector <int> y_cord;
	vector <int> :: iterator it2;
	int x_max=0, x_min=0, y_max=0, y_min=0, hpwl_this_graph=0, hpwl_total=0, line_cnt2=0, clb_bet_rows=0;
	int	num_modules=0;
	ifstream myfile2;
	myfile2.open(myfile_name.c_str(), ios::out);
	string nodeline;
	cout<<endl<<" in hpwl calculation block num_modules "<<num_modules<<endl;
	ostream_iterator< int > output( cout, "  " );
	
	if(	myfile2.is_open())
	{
		while ( getline (myfile2,nodeline) )
		{
			std::istringstream buf(nodeline); 
			std::istringstream iss(nodeline); 
			int  num;
			
			line_cnt2++;
			cout<<endl<<line_cnt2<<" num_modules: "<<num_modules;
			
			if(line_cnt2==2)
			{
				iss >> num;
				num_modules=num;
				
			}
			
			if(line_cnt2>=num_modules+6)
			{
				
				vector<int> vec;
				vector<int> :: iterator it;
				vec.clear();
				copy(istream_iterator<int>(buf),istream_iterator<int>(), back_inserter(vec));
				
				if(vec.size()>1)
				{
					for (it=vec.begin(); it!=vec.end(); ++it)                                              
					{
						int val1=*it;
						cout<<"  "<<val1;
						RR1=rec_region_list[val1];
						x_cord.push_back(RR1.centroid[1]);
						y_cord.push_back(RR1.centroid[0]);
					}
						
						it2 = max_element(x_cord.begin(), x_cord.end());
						x_max=*it2;  
						it2 = min_element(x_cord.begin(), x_cord.end());
						x_min=*it2;
						it2 = max_element(y_cord.begin(), y_cord.end());
						y_max=*it2;
						it2 = min_element(y_cord.begin(), y_cord.end());
						y_min=*it2;
						
						//hpwl_this_graph=abs(x_max-x_min)+abs(y_max-y_min);
						clb_bet_rows=(abs(y_max-y_min)-1)*clb_per_tile+clb_per_tile;
						hpwl_this_graph=abs(x_max-x_min)+clb_bet_rows;
						cout<<"  hpwl this graph: "<<hpwl_this_graph;
				}
				
				x_cord.clear();
				y_cord.clear();
			}
			hpwl_total=hpwl_total+hpwl_this_graph;
			
		}
		cout<<endl;
		myfile2.close();
	}
	
	cout<<endl<<" clb per tile: "<<dsp_per_tile<<endl;
	return(hpwl_total);
}
	