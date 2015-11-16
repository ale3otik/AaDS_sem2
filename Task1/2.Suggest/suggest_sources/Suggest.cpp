#include <vector>
#include <cassert>
#include <algorithm> 
#include <string>
#include <queue> 
#include <functional>

#include <string.h>

#include "RangeTree.cpp"

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::priority_queue;
using std::pair;
using std::make_pair;

#define EMPTY_WORD_CODE_ -117

int get_past_word(string & word)
{
	if(word.size() <= 0)
	{
		cout << "Error: EMPTY word"<<endl;
		return EMPTY_WORD_CODE_;
	}
	--word[word.size() - 1];
	return 0;
}
int get_next_word(string & word)
{
	if(word.size() <= 0)
	{
		cout << "Error: EMPTY word" << endl;
		return EMPTY_WORD_CODE_;
	}
	++word[word.size() - 1];
	return 0;
}

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

pair<long long , long long> findRange(vector<string> * dictionary , string  & request)
{
	vector<string>::iterator left_it = std::upper_bound(dictionary->begin() , dictionary->end() , request,compare_strings_upper);
	vector<string>::iterator right_it = std::lower_bound(dictionary->begin() , dictionary->end() , request , compare_strings_lower);
	long long left = left_it - dictionary->begin();
	long long right = right_it- dictionary->begin();
	--right;
	
	return make_pair(left , right);
}


vector<long long> * GetSuggestsQuery(vector<string> * dictionary , RangeTree<int> & tree_of_frequency , pair<long long , long long> range_in_dictionary,int sug_size)
{
	vector<long long> * answer = new vector < long long >;
	if(range_in_dictionary.first > range_in_dictionary.second)
	{
		answer->push_back(-1);
		return answer;
	}

	priority_queue<ranges> queue;
	
	pair<int,size_t> get_max = tree_of_frequency.RangeMinimumQuery(range_in_dictionary.first , range_in_dictionary.second);
	ranges range_to_push(range_in_dictionary , get_max.first , get_max.second);
	queue.push(range_to_push);
	while(sug_size > 0)
	{
		if(queue.empty()) break;

		ranges get_top = queue.top();
		queue.pop();

		answer->push_back(get_top.index);

		pair<long long , long long> new_parts[2];
	    new_parts[0] = make_pair(get_top.edges.first , get_top.index - 1);
	    new_parts[1] = make_pair(get_top.index + 1 , get_top.edges.second);

		for(int j = 0; j < 2; ++j)
		{
			if(new_parts[j].second >= new_parts[j].first)
			{
				get_max = tree_of_frequency.RangeMinimumQuery(new_parts[j].first , new_parts[j].second);
				queue.push(ranges(new_parts[j] , get_max.first , get_max.second));
			}
		}
		--sug_size;
	}
	return answer;
}


vector<long long> * GetSuggests(vector<string> * dictionary , RangeTree<int> & tree_of_frequency , string  & request,int suggests_size)
{
	pair<long long , long long> range_in_dictionary = findRange(dictionary , request); // find the range of suggested words

	vector<long long> * answer = GetSuggestsQuery(dictionary , tree_of_frequency,range_in_dictionary,suggests_size);

	return answer;
}
