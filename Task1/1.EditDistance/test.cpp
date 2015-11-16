#include "EditDistance.cpp"
#include <iostream>
#include <random>
#include <algorithm>
using namespace std;
#define TESTS_QUANTITY 20
static void GetString(ifstream & stream_in , string & targetStr) // get both texts from files 
{
	string BufStr;
	while(stream_in)
	{
		if(getline(stream_in , BufStr))
		{
			if(targetStr != "")
			{
				targetStr.append("\n");
			}
			targetStr.append(BufStr);
		}
	}
}
bool comparator(editOrder & first , editOrder & second)
{
	if(first.Position <= second.Position)
	{
		return true;
	}
	return false;
}
void generate_random(string & cur,ofstream & target )
{
	//cout << "\n ! \n";
	static default_random_engine generator;
	static uniform_int_distribution<int> random_quantity(30 , 100); // quantity of changes
	int quantity = random_quantity(generator);
	vector<editOrder> changes(quantity);
	static uniform_int_distribution<int> random_position(0 , cur.size());
	static uniform_int_distribution<int> random_type(1 , 3);
	static uniform_int_distribution<int> random_sym('A' , 'z');
	for(int i = 0; i < quantity; ++i)
	{
		changes[i] = { random_position(generator) , random_type(generator) , random_sym(generator) };
	}
	sort(changes.begin() , changes.end() , comparator);
	unsigned int k = 0;
	for(unsigned int i = 0; i < cur.size();)
	{
		if(k < changes.size() && i == changes[k].Position)
		{
			switch(changes[k].type)
			{
				case CHANGE:
					target << changes[k].sym;
					++i;
					++k;
					break;
				case INSERT:
					target << changes[k].sym;
					++k;
					break;
				case DELETE:
					++i;
					++k;
					break;
				default:
					cout << "random error" << endl;
					exit(1);
			}
		}
		else
		{
			target << cur[i];
			++i;
		}
	}
}

int main()
{
	for(int j = 0; j < TESTS_QUANTITY; ++j)
	{
		string  First , Second;
		ifstream first_in("lotr1.txt");
		assert(first_in);
		GetString(first_in , First); // get first text

		string text2 = "lotr2_*";
		text2[6] = j + 'a';
		ofstream second_out(text2);
		assert(second_out);
		generate_random(First , second_out);
		second_out.close();

		ifstream second_in(text2);
		assert(second_in);
		GetString(second_in , Second);//get second text
		second_in.close();
		first_in.close();

		text2 += ".edit"; // in this file out edit
		ofstream cout(text2);

		iter it_first;
		iter it_second;
		it_first.begin = 0;
		it_first.end = First.end() - First.begin();
		it_second.begin = 0;
		it_second.end = Second.end() - Second.begin();

		list<editOrder> * Answer;
		Answer = GetEditOrder(First , Second , it_first , it_second);


		cout << "ALL" << endl;
		int size = Answer->size();
		cout << '\n'<<"advanced alg: " << size << '\n' << endl;
		for(int i = 0; i != size; ++i)
		{
			editOrder buf = Answer->front();
			cout << buf.Position << ' ' << buf.type << ' ' << buf.sym << '\n';
			Answer->pop_front();
		}
		cout << endl;
			/*..........................*/
		
		Answer = FinalCountRange(First , Second , it_first , it_second);
		size = Answer->size();
		cout << '\n' <<"basic alg: "<< size << '\n' << endl;
		for(int i = 0; i != size; ++i)
		{
			editOrder buf = Answer->front();
			cout << buf.Position << ' ' << buf.type << ' ' << buf.sym << '\n';
			Answer->pop_front();
		}
		delete Answer;
	}
	return 0;
}