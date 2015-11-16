#include "RangeTree.hpp"
using std::pair;
using std::vector;
using std::string;

class Suggester
{
public:
	vector<pair<string,int>> GetSuggests(string request, int quantity);
	Suggester(vector<pair<string , int>> & dictionary);
	~Suggester();
private:
	RangeTree <int> * Tree;
	vector<string> words;
	vector<int> frequency;

	vector<long long> GetSuggestsQuery(pair<long long , long long> & range_in_dictionary , int sug_size);
	pair<long long , long long> findRange(string  & request);

	void SortDictionary(vector<pair<string , int>> & dictionary);
	void MakeTree();
};