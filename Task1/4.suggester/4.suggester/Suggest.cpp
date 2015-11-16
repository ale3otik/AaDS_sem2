#include <vector>
#include <cassert>
#include <algorithm> 
#include <string>
#include <queue> 
#include <functional>

#include "Suggest.h"
//#include "RangeTree.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::priority_queue;
using std::pair;
using std::make_pair;
#define EMPTY_WORD_CODE_ -117
/*********************************************************************************************/

/********************************************************************************************/
class ranges
{
public:
	pair<long long , long long> edges;
	int maximum;
	size_t index;
	ranges(pair<long long,long long> get_edges, int get_maximum,size_t get_index)
	{
		maximum = get_maximum;
		index = get_index;
		edges = get_edges;
	}

	~ranges() {}
};
bool operator <(ranges const  & A , ranges const & B)
{
	return A.maximum < B.maximum;
}

bool compare_strings_upper(const string & req , const string & str) //comparator to upper_bound
{
	{
		int i;
		for(i = 0; i < req.size() && i < str.size();++i)
		{
			if(req[i] < str[i]) return true;
			if(req[i] > str[i]) return false;
		}
		if(i >= req.size()) return true;
		else return false;
	}
}

bool compare_strings_lower(const string & str, const string & req) // comparator to lower_bound
{
	{
		int i;
		for(i = 0; i < req.size() && i < str.size(); ++i)
		{
			if(req[i] > str[i]) return true;
			if(req[i] < str[i]) return false;
		}
	    return true;
	}
}

pair<long long , long long> Suggester::findRange(string  & request)
{
	vector<string>::iterator left_it = std::upper_bound(words.begin() , words.end() , request,compare_strings_upper);
	vector<string>::iterator right_it = std::lower_bound(words.begin() , words.end() , request , compare_strings_lower);
	long long left = left_it - words.begin();
	long long right = right_it- words.begin();
	--right;
	
	return make_pair(left , right);
}


vector<long long> Suggester::GetSuggestsQuery(pair<long long , long long> & range_in_dictionary,int sug_size)
{
	vector<long long> answer;
	if(range_in_dictionary.first > range_in_dictionary.second)
	{
		answer.push_back(-1);
		return answer;
	}

	priority_queue<ranges> queue;
	
	pair<int,size_t> get_max = Tree->RangeMinimumQuery(range_in_dictionary.first , range_in_dictionary.second);
	ranges range_to_push(range_in_dictionary , get_max.first , get_max.second);
	queue.push(range_to_push);
	while(sug_size > 0)
	{
		if(queue.empty()) break;

		ranges get_top = queue.top();
		queue.pop();

		answer.push_back(get_top.index);

		pair<long long , long long> new_parts[2];
	    new_parts[0] = make_pair(get_top.edges.first , get_top.index - 1);
	    new_parts[1] = make_pair(get_top.index + 1 , get_top.edges.second);

		for(int j = 0; j < 2; ++j)
		{
			if(new_parts[j].second >= new_parts[j].first)
			{
				get_max = Tree->RangeMinimumQuery(new_parts[j].first , new_parts[j].second);
				queue.push(ranges(new_parts[j] , get_max.first , get_max.second));
			}
		}
		--sug_size;
	}
	return answer;
}


vector<pair<string,int>> Suggester::GetSuggests(string request,int quantity)
{
	vector<pair<string,int>> answer;
	
	pair<long long , long long> range_in_dictionary = findRange(request); // find the range of suggested words

	vector<long long> answer_indexes = GetSuggestsQuery(range_in_dictionary,quantity);
	answer.reserve(answer_indexes.size());
	for(int i = 0; i < answer_indexes.size(); ++i)
	{
		if(answer_indexes[i] < 0)
		{
			answer.push_back(make_pair("No suggests",-1));
			break;
		}
		answer.push_back(make_pair(words[answer_indexes[i]] , frequency[answer_indexes[i]]));
	}
	return answer;
}


bool comparator(const int & a , const int & b) //for the tree (RMQ)
{
	if(a >= b) return true;
	return false;
}
bool dict_comparator(const pair<string , int> dt1 , const pair<string , int> & dt2)
{
	if(dt1.first < dt2.first) return true;
	return false;
}

void Suggester::SortDictionary(vector<pair<string , int>> & dictionary)
{

	sort(dictionary.begin() , dictionary.end() , dict_comparator);

	for(int i = 0; i < dictionary.size(); ++i)
	{
		words.push_back(dictionary[i].first);
		frequency.push_back(dictionary[i].second);
	}
}

void Suggester::MakeTree()
{
	Tree = new RangeTree<int>(&frequency , comparator);
}

Suggester::Suggester(vector<pair<string , int>> & dictionary)
{
	SortDictionary(dictionary);
	MakeTree();
}
Suggester::~Suggester() 
{
	delete Tree;
}