#include <iostream>   // std::cout
#include <string>  
#include <sstream>   // std::string, std::to_string

using namespace std;

int main ()
{
 
	std::ostringstream ss;
    long num = 123456;
    ss << num;
   // std::cout << ss.str() << std::endl;
	std::string pi = "pi is " + ss.str();
	
	std::cout << pi << '\n';
	
	return 0;
}