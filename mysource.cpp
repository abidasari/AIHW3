#include<iostream>
#include<list>
#include<string>
#include<fstream>
#include<vector>

using namespace std;
string main_rhs;
list<string>kb;
bool compound(string);
string get_args(string);
string stringify(list<string> theta)
{
	string res = "";
	if (theta.empty())
		return res;
	list<string>::iterator i;
	int j = 0;
	for (i = theta.begin(); j < theta.size() - 1; j++, i++)
		res += *i + ",";
	res += *i;
	return res;

}

bool occur_check(string x, string y)
{
	//cout << "inside occur check" << endl;
	if (!compound(x) && !compound(y))
		return false;
	//cout << "one of them is compound" << endl;
	string x1;  string y1;
	if (compound(x))
		x1 = get_args(x);
	if (compound(y))
		y1 = get_args(y);
	string delimiter = ",";
	int pos;
	if (compound(x))
	{
		pos = x1.find(delimiter);
		if (pos == -1)//x has a single argument
		{
			if (x1 == y)
				return true;
			else
				return false;
		}
		while (x1.length() > 0)
		{
			pos = x1.find(delimiter);
			string temp = x1.substr(0, pos);
			if (temp == y)
				return true;
			x1.erase(0, pos + 1);
		}
	}
	if (compound(y))
	{
		pos = y1.find(delimiter);
		if (pos == -1)//y has a single argument
		{
			if (y1 == x)
				return true;
			return false;

		}
		while (y1.length() > 0)
		{
			pos = y1.find(delimiter);
			string temp = y1.substr(0, pos);
			if (temp == x)
				return true;
			y1.erase(0, pos + 1);
		}
	}

	return false;
}
string does_theta_contain(string theta, string x)
{
	if (theta.empty())
		return "fail";
	string delimiter = "|";
	
		int pos = theta.find(delimiter);
		if (pos != -1)
		{
			string first = theta.substr(0, pos);
			if (first == x)
			{
				theta.erase(0, pos + delimiter.length());
				return theta;
			}

		}

	

	return "fail";
}
bool compound(string x)
{
	string delimiter = "(";
	int pos = x.find(delimiter);
	if (pos != -1)
		return true;
	return false;


}
string get_args(string x)
{
	string delimiter1 = "(";
	string delimiter2 = ")";
	int pos1 = x.find(delimiter1);
	int pos2 = x.find(delimiter2);
	x.erase(0, pos1 + 1);//erase (pos1+1) characters from 0th position
	string result = x.substr(0, x.length() - 1);//substring of x from 0 to length-1
	return result;

}

string get_op(string x)
{
	string delimiter = "(";
	int pos = x.find(delimiter);
	string result = x.substr(0, pos);//substring of x viz pos characters from 0
	return result;
}

bool is_list(string x)
{
	string delimiter = ",";
	int pos = x.find(delimiter);
	if (pos != -1)
		return true;
	return false;


}
string get_first(string x)
{
	string delimiter = ",";
	int pos = x.find(delimiter);
	string first = x.substr(0, pos);
	return first;


}
string get_rest(string x)
{
	string delimiter = ",";
	int pos = x.find(delimiter);
	x.erase(0, pos + 1);
	return x;

}

string unify(string, string, string);
string unify_var(string var, string x, string theta)
{
	cout << "unify_var(var=" << var << ",x=" << x << ",theta=" << theta << ")" << endl;

	if (does_theta_contain(theta, var) != "fail")
		return unify(does_theta_contain(theta, var), x, theta);
	else if (does_theta_contain(theta, x) != "fail")
		return unify(var, does_theta_contain(theta, x), theta);
	else if (occur_check(var, x))
	{
		//	cout << "occur check true" << endl;
		return "failure";
	}
	else
	{
		
		return (var + "|" + x);
	}
}

string unify(string x, string y, string theta)
{
	cout << "unify(x=" << x << ",y=" << y << ",theta=" << theta << ")" << endl;
	
		if (theta == "failure")
			return "failure";
		else if (x == y)
			return theta;
		else if (x == "x")
			return unify_var(x, y, theta);
		else if (y == "x")
			return unify_var(y, x, theta);
		else if (compound(x) && compound(y))
			return unify(get_args(x), get_args(y), unify(get_op(x), get_op(y), theta));
		else if (is_list(x) && is_list(y))
			return unify(get_rest(x), get_rest(y), unify(get_first(x), get_first(y), theta));
		else
		{
			
			return "failure";
		}
	
	
}
bool is_implication(string x)
{
	string delimiter = "=>";
	int pos = x.find(delimiter);
	if (pos != -1)
		return true;
	return false;
}
string get_rhs(string x)
{
	string delimiter = "=>";
	int pos = x.find(delimiter);
	if (pos == -1)
		return x;
	x.erase(0, pos + 2);
	return x;
}
string get_lhs(string x)
{
	string delimiter = "=>";
	int pos = x.find(delimiter);
	if (pos == -1)
		return "";
	string res = x.substr(0, pos);
	return res;
}
string get_first_lhs(string x)
{
	string delimiter = "&";
	int pos = x.find(delimiter);
	if (pos == -1)
		return x;
	string first = x.substr(0, pos);
	return first;


}
string get_rest_lhs(string x)
{
	string delimiter = "&";
	string empty;
	int pos = x.find(delimiter);
	if (pos == -1)
		return empty;
	x.erase(0, pos + 1);
	return x;

}

string sub(string sub, string x)
{
	//cout << "inside sub" << endl;
	string delimiter1 = ",x,";
	string delimiter2 = "(x,";
	string delimiter3 = ",x)";
	string delimiter4 = "(x)";
	string result;
	string delimiter = "|";
	int pos = sub.find(delimiter);
	if (pos == -1)
		return x;
	string var = sub.substr(0, pos);
	sub.erase(0, pos + 1);
	string val = sub;
	while (x.length() > 0)
	{
		int pos1 = x.find(delimiter1);
		if (pos1 != -1)
		{
			x.replace(pos1+1, 1, val);
			result.append(x.substr(0, pos1 + 1+val.length()+1));
			x.erase(0, pos1 +1+ val.length()+1);

		}
		int pos2 = x.find(delimiter2);
		if (pos2!=-1)
		{
			x.replace(pos2 + 1, 1, val);
			result.append(x.substr(0, pos2 + 1 + val.length() + 1));
			x.erase(0, pos2 + 1 + val.length() + 1);
		}
		int pos3 = x.find(delimiter3);
		if (pos3 != -1)
		{
			x.replace(pos3 + 1, 1, val);
			result.append(x.substr(0, pos3 + 1 + val.length() + 1));
			x.erase(0, pos3 + 1 + val.length() + 1);
		}
		int pos4 = x.find(delimiter4);
		if (pos4 != -1)
		{
			x.replace(pos4 + 1, 1, val);
			result.append(x.substr(0, pos4 + 1 + val.length() + 1));
			x.erase(0, pos4 + 1 + val.length() + 1);
		}
		else
		{
			
			result.append(x);
			x.erase();
		}
	}

	return result;

}
string sub_driver(list<string>theta, string x)
{

	string res = x;
	list<string>::iterator i;
	for (i = theta.begin(); i != theta.end(); i++)
	{
		res = sub(*i, x);

	}
	return res;
}
int count_args(string x)
{
	string delimiter = ",";
	int count = 0;
	if (x.find(delimiter) == -1)
		return 1;
	while (x.find(delimiter) != -1)
	{
		count++;
		int pos = x.find(delimiter);
		x.replace(pos, 1, ";");
	}
	return count + 1;
}
vector<string>vectorify(string args)
{
	string delimiter = ",";
	int pos;
	vector<string>res;
	if (args.find(delimiter) == -1)
	{
		vector<string>loner;
		loner.push_back(args);
		return loner;
	}
	while (args.find(delimiter) != -1)
	{
		pos = args.find(delimiter);
		string temp = args.substr(0, pos);
		res.push_back(temp);
		args.erase(0, pos + 1);
	}
	res.push_back(args);
	return res;
}
list<string>fetch_rules(list<string> kb, string goal)
{
	list<string>::iterator i;
	list<string>res;
	
	for (i = kb.begin(); i != kb.end(); i++)
	{
		string temp = get_rhs(*i);
		if (get_op(goal) != get_op(temp))
			continue;
		cout << *i << endl;
		cout << "op stage passed" << endl;
		string goal_args = get_args(goal);
		string temp_args = get_args(temp);
		int goal_args_count = count_args(goal_args);
		int temp_args_count = count_args(temp_args);
		if (goal_args_count != temp_args_count)
			continue;
		cout << *i << endl;
		cout << "count args stage passed" << endl;
		int chk = 0;
		vector<string>goal_params=vectorify(goal_args);
		vector<string>temp_params=vectorify(temp_args);
		for (int i = 0; i < goal_params.size(); i++)
		{
			if (goal_params[i] == "x" || temp_params[i] == "x")
				continue;
			else
			{
				if (goal_params[i] != temp_params[i])
				{
					cout << "making chk 1" << endl;
					chk = 1;
					break;
				}
			}

		}
		if (chk == 0)
		{ 
			cout << "pushing " << *i << endl;
			res.push_back(*i);
		}
			

	}
	return res;

}
list<string>bc_and(list<string>kb, string query, string theta);
list<string>bc_or(list<string> kb, string query,  string theta)//
{
	cout << "bc_or(kb," << query << "," << theta <<")"<< endl;
	list<string>sub_kb;
	sub_kb = fetch_rules(kb, query);
	list<string>::iterator i;
	list<string>res;
	for (i = sub_kb.begin(); i != sub_kb.end(); i++)
	{
		//implement standardizing here
		string lhs = get_lhs(*i);
		string rhs = get_rhs(*i);
		list<string>and_result = bc_and(kb, lhs, unify(rhs, query, theta));
		res.merge(and_result);
	}
	return res;
}
list<string>bc_and(list<string>kb, string query, string theta)
{
	cout << "bc_and(kb," << query << "," << theta <<")"<< endl;
	list<string>res;
	if (theta == "failure")
	{
		list<string>fail;
		fail.push_back("failure");
		return fail;

	}
	else if (query.length() == 0)
	{
		list<string>self;
		self.push_back(theta);
		return self;

	}
	else
	{
		string first_lhs = get_first_lhs(query);
		string rest_lhs = get_rest_lhs(query);
		list<string>or_result = bc_or(kb, sub(theta, first_lhs), theta);
		list<string>::iterator i;
		for (i = or_result.begin(); i != or_result.end(); i++)
		{
			list<string>and_result = bc_and(kb, rest_lhs, *i);
			res.merge(and_result);
			
		}
	}
	return res;
}
list<string>bc_driver(list<string>kb, string query) //just a kick-off function
{
	string theta;
	return bc_or(kb, query, theta); // not sure whaa theta is for
}
int main()
{
	fstream f;
	list<string> kb; //knowledge base is a list of strings
	string query;
	main_rhs = query;
	int n;

	f.open("input.txt", ios::in);
	f >> query;
	f >> n;
	string temp;
	for (int i = 0; i < n; i++)
	{
		f >> temp;
		kb.push_back(temp);
	}
	f.close();

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
}