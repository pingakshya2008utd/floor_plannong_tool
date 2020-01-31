#include "priority_sort.h"
#include"general.h"


vector <rec_reg> sorted_region_list;
vector <rec_reg> only_clb_list;

void priority_sort(vector <rec_reg> rec_region_list)
 {
	vector <rec_reg> L1;
	vector <rec_reg> L2;
	vector <rec_reg> L3;
	vector <rec_reg> L4;
	vector <rec_reg> list;
	vector <rec_reg> ::iterator it;
	map <int, rec_reg> map_temp;
	map <int, rec_reg> :: reverse_iterator it_m;
	rec_reg RR1;
	int i=0;
	
	for (it=rec_region_list.begin(); it!=rec_region_list.end(); ++it)
	{
		RR1=*it;
		cout<<endl<<"RR "<<i+1<<"   CLB: "<<RR1.clb<<"  BRAM: "<<RR1.bram<<"  DSP:  "<<RR1.dsp<<"   module number: "<<RR1.module_num;
		if(RR1.dsp>0 && RR1.bram>0)
		{
			L1.push_back(RR1);
		}
		else if(RR1.dsp>0 && RR1.bram==0)
		{
			L2.push_back(RR1);
		}
		else if(RR1.dsp==0 && RR1.bram>0)
		{
			L3.push_back(RR1);
		}
		else if(RR1.dsp==0 && RR1.bram==0)
		{
			L4.push_back(RR1);
			only_clb_list.push_back(RR1);
		}
		i++;
	}
	
	cout<<endl;
	
	 sort_rr_list(L1,  rec_dsp );
	 sort_rr_list(L2, rec_dsp );
	 sort_rr_list(L3, rec_bram );
	 //sort_rr_list(L4, rec_clb );
	 
 }
 
 void sort_rr_list(vector <rec_reg> L1,  resource res_type )
 {
	vector <rec_reg> ::iterator it;
	multimap <int, rec_reg> map_temp;
	multimap <int, rec_reg> :: reverse_iterator it_m;
	
	 for (it=L1.begin(); it!=L1.end(); ++it)
	{
		int key;
		rec_reg value;
		value=*it;
		if(res_type==rec_dsp)
		{
			key=value.dsp;
		}
		else if(res_type==rec_bram)
		{
			key=value.bram;
		}
		
		else if(res_type==rec_clb)
		{
			key=value.clb;
		}
		
		map_temp.insert ( std::pair<int,rec_reg>(key,value) );
	}
	
	for (it_m=map_temp.rbegin(); it_m!=map_temp.rend(); ++it_m)
	{
		rec_reg RR2;
		RR2= it_m->second;
		sorted_region_list.push_back(RR2);
		//cout<<endl<<"dsp:= "<<it_m->first <<"  "<<RR2.name<<" RR module number: "<<RR2.module_num;
		cout<<endl<<"RR name  "<<RR2.name<<" RR module number: "<<RR2.module_num<<"   RR CLB: "<<RR2.clb<<"  RR BRAM: "<<RR2.bram<<"   RR DSP: "<<RR2.dsp;
		cout<<" sorted_region_list size: "<<sorted_region_list.size();
		
	}
	
	cout<<endl;
 }
  