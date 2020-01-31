#include "sequence_pair.h"
#include"general.h"

vector <filled_reg> seq_pair_a;
vector <filled_reg> seq_pair_b;
vector <filled_reg> seq_pair_min_a;
vector <filled_reg> seq_pair_min_b;
//********************************************************************************************************************************/
void gen_seq_pair(vector <filled_reg> filled_region_cords)
{
	seq_pair_a.clear();
	seq_pair_b.clear();
    vector <filled_reg>:: iterator it, it_2;
	int a_xL, b_xL, a_xR, b_xR, a_yU,b_yU, a_yD, b_yD, len_a, mod_cnt=0;
	filled_reg main_list_module,seq_pair_module, RR1;
	it=filled_region_cords.begin();
	RR1= *it;
	seq_pair_a.push_back(RR1);
	seq_pair_b.push_back(RR1);
	vector <filled_reg> seq_pair_temp;
	vector  <filled_reg> :: iterator it_sp;
	cout<<endl<<"0th module: RR"<<RR1.module_num<<endl;
	cout<<endl<<"size of seq pair:  "<<seq_pair_a.size()<<endl;
	for (it_2=filled_region_cords.begin()+1; it_2!=filled_region_cords.end(); ++it_2)
	{
		mod_cnt++;
		main_list_module=*it_2;
		a_xR=main_list_module.x_max;
		a_xL=main_list_module.x_min;
		a_yU=main_list_module.y_min;
		a_yD=main_list_module.y_max;
		len_a=seq_pair_a.size();
		seq_pair_temp=seq_pair_a;
		cout<<endl<<mod_cnt<<"th filled region module: RR"<<main_list_module.module_num<<endl;
		cout<<"-----------------------------------------------------------------------------------"<<endl;
		
		
		for (it_sp=seq_pair_temp.begin(); it_sp!=seq_pair_temp.end(); ++it_sp)
		{
			seq_pair_module=*it_sp;
			b_xR=seq_pair_module.x_max;
			b_xL=seq_pair_module.x_min;
			b_yU=seq_pair_module.y_min;
			b_yD=seq_pair_module.y_max;
			cout<<endl<<"seq pair mod num:  "<<seq_pair_module.module_num<<endl;
			if (a_xL<b_xR && b_yD<a_yU) 
			{
				cout<<"insert left"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_left, 0);
				cout<<" left done"<<endl;
			}
			else
			{
				cout<<"insert right"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_right,0);
				cout<<" right done"<<endl;
			}
			
			if (a_xL<b_xR && a_yD< b_yU )
			{
				cout<<"insert left"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_left, 1);
				cout<<" left done"<<endl;
			}
			else
			{
				cout<<"insert right"<<endl;
				insert_seq(main_list_module, seq_pair_module, dir_right,1);
				cout<<" right done"<<endl;
			}
		}
		
		cout<<endl<<"size of seq pair after module "<<mod_cnt<<"= "<<seq_pair_a.size()<<endl;
	}

	
}

void insert_seq(filled_reg main_mod, filled_reg seq_mod, direction dir, int seq_pair_num)
{
	vector <filled_reg> :: iterator it_MM, it_SP;
	int pos_mm=-1, pos_sp=-1;
	
	cout<<" main module postion in sp: "<<pos_mm<<endl;
	cout<<" seq pair module postion in sp: "<<pos_sp<<endl;
	//cout<<endl<<"size of seq pair after module "<<seq_pair_a.size()<<endl;
	
	if(seq_pair_num==0)
	{
		pos_sp= find_pos(seq_pair_a, seq_mod);
		pos_mm= find_pos(seq_pair_a, main_mod);
		if(pos_mm==-1 && dir==dir_left)
		{
			seq_pair_a.insert( seq_pair_a.begin()+pos_sp, main_mod );
		}
	
		else if(pos_mm==-1 && dir==dir_right)
		{
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp+1, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm<pos_sp)
		{
			seq_pair_a=seq_pair_a;
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm>pos_sp)
		{
			seq_pair_a.erase (seq_pair_a.begin()+pos_mm);
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_right && pos_mm>pos_sp)
		{
			seq_pair_a=seq_pair_a;
		}

		else if(pos_mm!=-1 && dir==dir_right && pos_mm<pos_sp)
		{
			seq_pair_a.erase (seq_pair_a.begin()+pos_mm);
			cout<<endl<<"size of seq pair AFTER delete:  "<<seq_pair_a.size()<<endl;
			pos_sp= find_pos(seq_pair_a, seq_mod);
			seq_pair_a.insert( seq_pair_a.begin() + pos_sp+1, main_mod );
		}
	}
	else if(seq_pair_num==1)
	{
		pos_sp= find_pos(seq_pair_b, seq_mod);
		pos_mm= find_pos(seq_pair_b, main_mod);
		if(pos_mm==-1 && dir==dir_left)
		{
			seq_pair_b.insert( seq_pair_b.begin()+pos_sp, main_mod );
		}
	
		else if(pos_mm==-1 && dir==dir_right)
		{
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp+1, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm<pos_sp)
		{
			seq_pair_b=seq_pair_b;
		}
	
		else if(pos_mm!=-1 && dir==dir_left && pos_mm>pos_sp)
		{
			seq_pair_b.erase (seq_pair_b.begin()+pos_mm);
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp, main_mod );
		}
	
		else if(pos_mm!=-1 && dir==dir_right && pos_mm>pos_sp)
		{
			seq_pair_b=seq_pair_b;
		}

		else if(pos_mm!=-1 && dir==dir_right && pos_mm<pos_sp)
		{
			seq_pair_b.erase (seq_pair_b.begin()+pos_mm);
			pos_sp= find_pos(seq_pair_b, seq_mod);
			seq_pair_b.insert( seq_pair_b.begin() + pos_sp+1, main_mod );
		}
	}
	
}

int find_pos(vector <filled_reg> seq_pair, filled_reg A)
{
	int pos=-1, cnt=0;
	vector <filled_reg> :: iterator it;
	filled_reg RR1;
	
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

void print_seq_pair(vector <filled_reg> seq_pair_a, vector <filled_reg> seq_pair_b)
{
	vector <filled_reg> ::reverse_iterator it_sp;
	filled_reg RR1;
	cout<<endl<<"<";
	for (it_sp=seq_pair_a.rbegin(); it_sp!=seq_pair_a.rend(); ++it_sp)
	{
		RR1=*it_sp;
		cout<<"RR"<<RR1.module_num<<",";
	}
	
	cout<<"> , <";
	
	for (it_sp=seq_pair_b.rbegin(); it_sp!=seq_pair_b.rend(); ++it_sp)
	{
		RR1=*it_sp;
		cout<<"RR"<<RR1.module_num<<",";
	}
	cout<<">"<<endl;
}
