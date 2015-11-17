#include<iostream>
#include<list>
#include<string>
#include<fstream>
#include<vector>

using namespace std;

void printList(list<string> l)
{
	list<string>::iterator i;
	for( i = l.begin(); i != l.end(); i++)
		cout << *i << endl;
}

int main()
{
	fstream f; // for file stream
	string temp_str; // to store the temp strings 
	list<string> kb; //knowledge base is a list of strings
	list<string> query; //list of queries
	// main_rhs = query;// for something
	int kb_size, q_size; // kb_size stores the KB size and q_size stores the numeber of queries

	f.open("input_1.txt", ios::in); //open the file
	// f >> temp_str;
	getline(f, temp_str);
	q_size = stoi(temp_str);

	//string temp;

	for(int i = 0 ; i < q_size; i++)
	{
		getline(f, temp_str);
		query.push_back(temp_str);
	}

	f >> temp_str;
	kb_size = stoi(temp_str);

	for (int i = 0; i < kb_size; i++)
	{
		getline(f, temp_str);
		kb.push_back(temp_str);
	}
	f.close();

	cout << "The KB:";
	printList(kb);
	cout << "\nThe Query list:\n";
	printList(query);
/*
	list<string>ans = bc_driver(kb, query); //driver function for backward chaining
	list<string>::iterator i;
	//cout << "ANSWER:";
	string answer = "TRUE";
	if (ans.empty())
		answer = "FALSE";
	else if (!ans.empty() && ans.size() == 1 && ans.front() == "failure")
		answer = "FALSE";
	f.open("output.txt", ios::out);
	f << answer;
	f.close();
	cout << "ANSWER:" << endl;
	for (i = ans.begin(); i != ans.end(); i++)
		cout << *i << endl;
		*/
	
}