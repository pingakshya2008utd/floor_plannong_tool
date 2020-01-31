#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <vector>
#include <algorithm>


using namespace std;




int main(int argc,char *argv[])
{
    	string xint;



           vector <string> netdata;
           vector <string> node_name;
string line;
string nodeline;
    unsigned	int i=0;
    		FILE * ptr_net;
    		FILE * ptr_area;
    		FILE * ptr_netD;
    		FILE *fp;
    		string ab;
    	int n=0;
    	char * pch;
//list<string> L;

 ifstream myfile (argv[1]);
  if (myfile.is_open())
  {
    while ( getline (myfile,line) )
    {
        netdata.push_back(line);
    }
    myfile.close();
  }


  ifstream myfile1 (argv[2]);
  if (myfile1.is_open())
  {
    while ( getline (myfile1,nodeline) )
    {
     string mynode = nodeline;
     mynode = mynode. substr ( 0, mynode. find_first_of(' ') );
        node_name.push_back(mynode);
    }
    myfile1.close();
  }


 ofstream outputfile;
outputfile.open ("ibm01.txt");


vector< list<string> > L1;
list <string> net_info;

for (i=5;i<netdata.size();i++)
{
  std::size_t found = netdata[i].find('s');
  if (found!=std::string::npos)
    {

     if ( net_info. size() != 0 )
        {

            L1. push_back ( net_info );
            net_info. clear();
        }

     string mystr = netdata[i];
     mystr = mystr. substr ( 0, mystr. find_first_of(' ') );
     net_info. push_back ( mystr );

    }
    else
    {
     string mystr = netdata[i];
     mystr = mystr. substr ( 0, mystr. find_first_of(' ') );
     net_info. push_back ( mystr );
    }
    if ( i == (netdata.size()-1) )
    {
        L1. push_back ( net_info );
    }

}
    unsigned int j = 0;
    for ( j = 0; j < L1. size(); j++ )
    {
        list<string> val;
        val = L1[j];
        list<string>:: iterator it = val. begin();

        //cout << "size of val is " << val. size() << endl;
       cout << "Net " << j << " : ";
        outputfile<< "Net " << j << " : ";
        for ( ; it != val. end(); it++ )
        {
          cout << *it << " ";
          outputfile << *it << " ";
        }
       cout << endl;
       outputfile<< endl;
    }

cout <<endl;
outputfile <<endl;
outputfile <<endl;
cout <<endl;
   int cnt=0;
    unsigned int q = 0;
    unsigned int m1 = 0;

    for ( m1 = 0; m1 < node_name. size(); m1++ )
    {
        cout<<endl<< node_name[m1]<<": ";
        outputfile<<endl<< node_name[m1]<<": ";
    for ( q = 0; q < L1. size(); q++ )
    {
        list<string> val1;
        val1 = L1[q];
        list<string>:: iterator it1; // = val1. begin();

      //std::list<string>::iterator findIter = std::find(val1.begin(), val1.end(), node_name[m1] );
it1 = find (val1.begin(), val1.end(), node_name[m1]);
  if (it1 != val1.end())
   {
    cout<<"net"<<q<<"  " ;
                outputfile<<"net"<<q<<"  " ;

   }



        }
     // cout<<endl;

    }
    return 0;

}