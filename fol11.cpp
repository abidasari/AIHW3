#include <iostream>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sys/types.h>

using namespace std;

string trim(string s)//trims leading and trailing spaces
{
	size_t first = s.find_first_not_of(" \t");
	size_t last = s.find_last_not_of(" \t");
	auto strRange = last - first + 1;
    return s.substr(first, strRange);	
}

class term
{
public:
	int type; //if negation 0, else 1
	string name;
	list<string> args;
	term(string s);
	void merge(string s);
};

void term::merge(string s) // just a hack.
{
	if(s != "NULL")
	{
		if(s.find("~") != string::npos)
		{
			type = 0; //negation gets 0
			//getting rid of the "~"
			s = s.substr(1); // stores the name(args) only. no ~
		}
		else
			type = 1; //no negation gets 1
		size_t delimiter1 = s.find('(');
		name = s.substr(0,delimiter1); //this contains the name 
		s = s.substr(delimiter1); // this contains the (.,.,.)
				// cout << "this . "  << s <<endl;

		s = s.substr(1,s.length()-2); //removed the ()
		istringstream ss(s);
		string token;

		while(getline(ss, token, ','))
		{
		    // std::cout << trim(token) << '\n';
		    // term t(trim(token));
		    args.push_back(token);
		}
	}
}
term::term(string s)
{
	if(s != "NULL")
	{
		if(s.find("~") != string::npos)
		{
			type = 0; //negation gets 0
			//getting rif of the "~"
			s = s.substr(1); // stores the name only. no ~
		}
		else
			type = 1; //no negation gets 1
		size_t delimiter1 = s.find('(');
		name = s.substr(0,delimiter1); //this contains the name 
		s = s.substr(delimiter1); // this contains the (.,.,.)
		// cout << "this "  << s <<endl;
		s = s.substr(1,s.length()-2); //removed the ()
		istringstream ss(s);
		string token;

		while(getline(ss, token, ','))
		{
		    // std::cout << trim(token) << '\n';
		    // term t(trim(token));
		    args.push_back(token);
		}
	}
}

class sentence
{
public:
	string type; // fact or compound
	list<term> predicates; // maybe a conj of literals, may or may not contain a variable. If fact; this is NULL. 
	term consequent; // only a single literal. May or may not contain a variable.
	sentence(string s);
};

sentence::sentence(string s) : consequent("NULL")
{
	if (s.find("=>") != string::npos)
	    type = "compound";
    else
    	type = "fact";
    if(type == "fact")
    {
    	consequent.merge(s);
    	term t(s);
    	// predicates.push_back(t);
    }
    else
    {
    	string delimiter1 = "=>";
	    size_t pos = s.find('=');
	    string temp = s.substr(pos+3);// this now stores the conseq inscluding the ~
	    consequent.merge(temp);
	    //deal with antecedent now. ante. could be single term or ^ of terms
	    string antecedent = trim(s.substr(0,pos-1));
	    // std::string input = "\t   \t   abc(y,n) ^ def(5,t)     ^ ghi(g,g)   ";
		
		istringstream ss(antecedent);
		string token;

		while(getline(ss, token, '^'))
		{
		    // std::cout << trim(token) << '\n';
		    term t(trim(token));
		    predicates.push_back(t);
		}
    }
}


////////prints begin

void printList(list<string> l)
{
	list<string>::iterator i;
	for( i = l.begin(); i != l.end(); ++i)
		cout << *i << ",";
}

string pList(list<string> l)
{
	string output = "";
	list<string>::iterator i;
	for( i = l.begin(); i != l.end(); ++i)
		output = output + *i + ",";
	return output;
}

void printTerm(term t)
{
	cout << t.type << "_" ;
	cout << t.name;
	cout << "[";
	printList(t.args);
	cout << "]";
}

string printMapInline(unordered_map<string,string> m)
{
	string output;
	string temp = "";

	for (auto& x: m)
	{
		temp = temp + x.first + ": " + x.second + " | ";
	}
	return temp;
}

void printMap(unordered_map<string,string> m)
{
	for (auto& x: m)
	    cout << x.first << ": " << x.second << endl;
}


string pTerm(term t)
{
	string output;
	string args = pList(t.args); 
	output = to_string(t.type) + "_" + t.name + "[ " + args + "]" ;
	return output;
}

string printListTerms(list<term> t)
{
	string output = "< ";
	list<term>::iterator it;

	for (it = t.begin(); it!=t.end();++it)
	{
		output.append(pTerm(*it));
		output.append(" ^ ");
	}
	output.append(" >");
	return output;
}


void printKB(list<sentence> kb)
{
	// cout << "got here\n";
	list<sentence>::iterator it1;
	int i =0 ;
	for(it1 = kb.begin(); it1!=kb.end();i++, ++it1)
	{
		cout << i+1 <<  ") Type: " << it1->type << " ";
		if(it1->type == "fact") // no predicates
			printTerm(it1->consequent);
		else//print predicates and then consquent
		{
			list<term>::iterator it2;
			for (it2=it1->predicates.begin(); it2!=it1->predicates.end();it2++)
			{
				printTerm(*it2);
			}
			cout << " => ";
			printTerm(it1->consequent);
		}
		cout << endl;
	}
}

//Prints end

unordered_map<string,string> bc_or(list<sentence> kb, term query, unordered_map<string,string> theta);
unordered_map<string,string> bc_and(list<sentence> kb, list<term> queries, unordered_map<string,string> theta);
unordered_map<string,string> bc_ask(list<sentence> kb, term query);
list<sentence> fetch_rules(list<sentence> kb, term query);
unordered_map<string,string> unify(list<string> args, list<string> q_args, unordered_map<string,string> theta);
unordered_map<string,string> unify_var(list<string> args, list<string> q_args, unordered_map<string,string> theta);
term subst(unordered_map<string,string> theta, term t);
void standardizeKB(list<sentence> &kb);


bool firstUpper(string& word) //checks if the first letter of the string is Uppercase
{ 
	return word.size() && isupper(word[0]); 
}


void standardizeTerm(term& t , int i)
{
	list<string>::iterator it;
	for(it=t.args.begin();it!=t.args.end();++it)
	{
		if(firstUpper(*it))
			continue;
		*it = *it + to_string(i+1);
	}

}

void standardizeKB(list<sentence>& kb)
{
	list<sentence> res;
	list<sentence>::iterator it1;
	int i = 0; 
	for(it1 = kb.begin(); it1 != kb.end(); ++i, ++it1)
	{
		// cout << i+1 <<  ") Type: " << it1->type << " ";
		list<term>::iterator it;
		for(it = it1->predicates.begin(); it!=it1->predicates.end(); ++it)
		{
			standardizeTerm(*it, i);
		}
		standardizeTerm(it1->consequent, i);
	}
}


term subst(unordered_map<string,string> theta, term t)
{
	string thetaout = printMapInline(theta);
	string termin = pTerm(t);
	cout << "subst( " << thetaout << " , " << termin << " )" << endl;
	term res("NULL");
	res.type = t.type;
	res.name = t.name;
	// res.args = { };
	list<string> args_out;
	list<string>:: iterator it;
	for(it=t.args.begin(); it!=t.args.end(); ++it)
	{
		unordered_map<string,string>::const_iterator got = theta.find(*it);
		if(got != theta.end()) //binding found
		{
			args_out.push_back(got->second);
		}
		else //binding not found
		{
			args_out.push_back(*it);
		}
	}
	res.args = args_out;
	return res;
}


unordered_map<string,string> unify_var(list<string> args, list<string> q_args, unordered_map<string,string> theta)
{
	string plist = pList(args); 
	string qlist = pList(q_args);
	string thetal = printMapInline(theta);
	cout << "unify_var( " << plist << " , " <<  qlist << " , " << thetal << " )"<< endl;
	string var = args.front();
	string var2 = q_args.front();
	unordered_map<string,string>::iterator it1 = theta.find(var);
	unordered_map<string,string>::iterator it2 = theta.find(var2);
	if(it1 != theta.end())
	{
		list<string> res;
		res.push_back(theta.at(var));
		return unify(res,q_args,theta);
	}
	else if( it2 != theta.end())
	{
		list<string> res;
		res.push_back(theta.at(var2));
		return unify(args,res,theta);
	}
	//occurs check???
	else
	{
		theta.insert({var, var2});
		return theta;
	}
}


unordered_map<string,string> unify(list<string> args, list<string> q_args, unordered_map<string,string> theta)
{
	string plist = pList(args);
	string qlist = pList(q_args);
	string thetal = printMapInline(theta);
	cout << "unify( " << plist << " , " << qlist << " , " << thetal << " )" << endl;


	unordered_map<string,string>::iterator it = theta.find("failure");
	if(it != theta.end())
	{
		//return faliure
		unordered_map<string, string> res = { { "failure" , "failure" } };
		return res;
	}
	else if(args == q_args)
		return theta;
	
	else if(args.size() == 1 && !firstUpper(args.front()))
		return unify_var(args, q_args, theta);
	
	else if(q_args.size() == 1 && !firstUpper(q_args.front()))
		return unify_var(q_args, args, theta);
	
	else if(args.size() != 1 && q_args.size() != 1)
	{
		list<string> args_first, q_args_first; 
		string temp = args.front();
		args_first.push_back(temp);
		args.pop_front();
		temp = q_args.front();
		q_args_first.push_back(temp);
		q_args.pop_front();
		return unify(args, q_args, unify(args_first, q_args_first, theta));
	}
	else
	{
		//return faliure
		unordered_map<string, string> res = { { "failure" , "failure" } };
		return res; 
	}

}

list<sentence> fetch_rules(list<sentence> kb, term query)
{
	cout << "fetch_rules ( KB , " << pTerm(query) << " )"<<endl; 
	list<sentence>::iterator it;
	list<sentence> res;
	for(it=kb.begin();it!=kb.end();++it)
	{
		string name = query.name;
		if(it->consequent.name != name)
			continue;
		// cout << "name matches!\n";
		term t = it->consequent;
		if(t.args.size() != query.args.size())
			continue;
		// cout << "signatrue matches!!\n";
		if(t.type != query.type)
			continue;
		res.push_back(*it);
	}
	return res;
}


unordered_map<string,string> bc_or(list<sentence> kb, term query, unordered_map<string,string> theta)
{
		getchar();

	string qlist = pTerm(query);
	string thetal = printMapInline(theta);
	cout << "bc_or ( KB , " << qlist << " , " << thetal << " )\n";
	list<sentence> sub_kb = fetch_rules(kb, query);
	printKB(sub_kb);
	list<sentence>::iterator it;
	unordered_map<string,string> res;
	// unordered_map<string,string> result;
	for(it = sub_kb.begin(); it != sub_kb.end(); ++it)
	{
		list<term> lhs = it->predicates;
		term rhs = it->consequent;
		list<string> rhs_args = rhs.args;
		list<string> query_args = query.args;
		if(rhs.name != query.name)
			continue;
		unordered_map<string,string> and_res = bc_and(kb, lhs, unify(rhs_args, query_args, theta));
		
		for (auto& x: and_res)
		    cout << x.first << ": " << x.second << std::endl;
		// getchar();
		cout << "---------------------$----------------------\n";
		// int inp;
		// cin >> inp;


		res.insert(and_res.begin(), and_res.end()); // maybe it should be the other way around
		// result.merge(and_res); // dunno what to do exactly here.
	}
	return res;
}



unordered_map<string,string> bc_and(list<sentence> kb, list<term> predcs, unordered_map<string,string> theta)
{
	// string qlist = pTerm(query);
	string thetal = printMapInline(theta);
	cout << "bc_and ( KB , " << printListTerms(predcs) << " , "<< thetal << " )\n";

	unordered_map<string,string> result;
	unordered_map<string,string>::const_iterator got = theta.find ("failure");
	if( got != theta.end()) // failure found in the map!!
	{
		unordered_map<string,string> res = { { "failure" , "failure" } };
		// res.push_back("failure");
		return res;
	}
	else if (predcs.empty())
	{
		// list<string> res;
		// res.push_back(theta);
		return theta;
	}
	else
	{
		term first = predcs.front();
		predcs.pop_front();
		list<term> rest = predcs;

		unordered_map<string,string> or_result = bc_or(kb, subst(theta, first), theta);

		// unordered_map<string,string>::iterator it;
		// for(it = or_result.begin(); it!=or_result.end(); ++it)
		// {
		// 	unordered_map<string,string> _theta;
		// 	_theta.insert(*it);
		// 	unordered_map<string,string>and_res = bc_and(kb, rest, _theta);
		// 	// result.merge(andresult);
		// 	result.insert(and_res.begin(), and_res.end());
		// }
		unordered_map<string,string>and_res = bc_and(kb, rest, or_result);
		// result.merge(andresult);
		result.insert(and_res.begin(), and_res.end());
	}
	return result;
}

unordered_map<string,string> bc_ask(list<sentence> kb, term query) // not yet sure what it should return
{
	unordered_map<string, string> theta;
	return bc_or(kb, query, theta);
}


int main()
{
	fstream f; // for file stream
	string temp_str; // to store the temp strings 
	list<string> kb; //knowledge base is a list of strings
	list<string> query; //list of queries
	list<sentence> KB;
	list<term> Q; // the parsed KB.
	// main_rhs = query;// for something
	int kb_size, q_size; // kb_size stores the KB size and q_size stores the numeber of queries

	f.open("input.txt", ios::in); //open the file
	// f >> temp_str;
	getline(f, temp_str);
	q_size = stoi(temp_str);

	//string temp;

	for(int i = 0 ; i < q_size; i++)
	{
		getline(f, temp_str);
		query.push_back(temp_str);
	}

	getline(f,temp_str);
	kb_size = stoi(temp_str);

	for (int i = 0; i < kb_size; i++)
	{
		getline(f, temp_str);
		kb.push_back(temp_str);
	}
	f.close();

	// cout << "The KB:\n";
	// printList(kb);
	// cout << "\nThe Query list:\n";
	// printList(query);
	list<string>::iterator it;
	int i =0;
	for(it = kb.begin(); it!=kb.end(); i++ ,it++) // parsing and creating the new KB
	{
		// cout << i << ") the sentence: " << *it << endl;
		sentence s(*it);
		KB.push_back(s);
	}
	list<string>::iterator it1;
	for(it1 = query.begin(); it1!=query.end(); it1++)
	{
		term t(*it1);
		Q.push_back(t);
	}
	//printing KB
	// cout << endl;
	standardizeKB(KB);
	printKB(KB);
	cout << "------------------------\n";
	cout << printListTerms(Q) << endl;
	cout << "------------------------\n";


	// unordered_map<string,string> bindings = { {"x12","Bob"} };
	// list<string> a = {{"x1"}};
	// list<string> b = {{"Bob"}};
	// printMap(unify(a,b,bindings));




	list<term>::iterator it2;
	list<string> ans;
	getchar();
	for(it2 = Q.begin(); it2!=Q.end(); it2++)
	{
		string output = "true"; 
		unordered_map<string,string> res =  bc_ask(KB, *it2);
		if (res.empty())
			output = "false";

		unordered_map<string,string>::iterator got = res.find("failure");
		if ( got != res.end() && !res.empty() && res.size()==1)
		    output = "false";
	    
		// cout << "Final result: " << output << endl;
		ans.push_back(output);
	}
	cout << endl;
	printList(ans);
	cout << endl;

	// printKB(KB);
	return 0;
		
}