#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
string trim(string s)
{
	size_t first = s.find_first_not_of(" \t");
	size_t last = s.find_last_not_of(" \t");
	auto strRange = last - first + 1;

    return s.substr(first, strRange);	
}

bool firstUpper(string& word) 
{ 
	return word.size() && isupper(word[0]); 
}

int main()
{
	list< unordered_map<string,string> > res;

	unordered_map<string,string> un_map1 = { { "one" , "1" } , { "two" , "2" } , { "three" , "3" } };
	unordered_map<string,string> un_map2 = { { "four" , "4" } , { "five" , "5" } , { "six" , "6" } };

	unordered_map<string,string> un_map3 = { { "seven" , "7" } , { "eight" , "8" } , { "nine" , "9" } };

	res.push_back(un_map1);
	res.push_back(un_map2);
	res.push_back(un_map3);
	string temp2, temp1;






	std::string input = "a => ~ghi(g,x,y)";
	size_t delim = input.find('=');

	string temp = input.substr(delim+3);
	cout << "conseq: " << temp << endl;

	std::istringstream ss(input);
	std::string token;
	input = temp.substr(1);
	cout << input << endl;
	size_t d = input.find('(');
	size_t e = input.find(')');
	string name = input.substr(0,d);
	cout << name << endl;
	input = input.substr(d);
	cout << "updated input " << input << endl; 
	d = input.find('(');
	string args = input.substr(1 , input.length() - 2);
	cout << "args: " << args <<endl;
	std::istringstream s(args);
	while(std::getline(s, token, ','))
	{
		cout << token << endl;
	}

	list<string> xc;// = { "tom" , "brad" , "x" };
	list<string> zx;// = { "tom" , "brad" , "x" };
	xc.push_back("tom");
	xc.push_back("brad");
	xc.push_back("x");

	zx.push_back("t");
	zx.push_back("brad");
	zx.push_back("x");
	cout << "sizze:" << zx.size() << endl;

	cout << "calling isUpper: " << firstUpper(zx.front()) << endl;

	if(xc == zx) 
		cout << "equal\n"; 
	else
		cout << "nope\n";
	unordered_map<string,int> mapr; 
	mapr = { { "Mars", 3000}, { "Saturn", 60000}, { "Jupiter", 70000 } };

	mapr.insert({"hello", 88});
	mapr.insert({"ok", 4});
int y = 9;
string tes = "maata";
mapr.insert({tes,y});
	for (auto& x: mapr)
	    cout << x.first << ": " << x.second << std::endl;

	cout <<"fadsf"<< mapr.at("tommy") << endl;

	while(std::getline(ss, token, '^'))
	{
	    std::cout <<"m" <<token << " "<< trim(token) << '\n';
	}

	cout << "############################################\n";
	unordered_map<string,string> test;
		for (auto& x: test)
	    cout << "->>>>>>>" << x.first << ": " << x.second << std::endl; 
	
	return 0;
}