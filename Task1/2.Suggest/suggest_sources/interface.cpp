#include <string>
#include <iostream>
#include <algorithm> 
#include <fstream>
#include <vector>

#include "Suggest.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::pair;

void SortDictionary()
{

}

bool comparator(const int & a,const int & b) //for the tree (RMQ)
{
	if(a >= b) return true;
	return false;
}
bool dict_comparator(const pair<string,int> dt1,const pair<string,int> & dt2)
{
	if(dt1.first < dt2.first) return true;
	return false;
}
void SetDictionary(vector<string> * words , vector<int>* frequency , std::ifstream & read_stream)
{

	string buffer;
	int count;
	vector<pair<string , int>> dictionary;
	while(true)
	{
		read_stream>>count;
		getline(read_stream , buffer);
		if(!read_stream) break;
		if(buffer[0] == ' ')
		{
			for(int i = 1; i < buffer.size(); ++i)
			{
				buffer[i - 1] = buffer[i];
			}
			buffer.resize(buffer.size() - 1);
		}
	    dictionary.push_back(make_pair(buffer,count));
	}

	sort(dictionary.begin(),dictionary.end(),dict_comparator);
	for(int i = 0; i < dictionary.size(); ++i)
	{
		words->push_back(dictionary[i].first);
		frequency->push_back(dictionary[i].second);
	}
}
int main()
{
	/*
	* Dictionary inicialize
	*/
	std::ifstream read_stream("dict.txt");
	vector<string> * words = new vector<string>;
	vector<int> * frequency = new vector < int >;
	SetDictionary(words , frequency , read_stream);//including sorting

	RangeTree<int> Tree = RangeTree<int>(frequency,comparator); // create tree of indexes for RMQ with reverse order

	cout << "Chose a quantity of suggested words : ";
	int suggests_size;
	cin >> suggests_size;

	while(true) // main body
	{
		/*
		* wait User request
		*/
		string request;
		cout << "enter word : ";
		cin>> request;

		/*
		*GetSuggests
		*/
    	vector<long long> * answer = GetSuggests(words,Tree, request,suggests_size);

		/*
		* Print out
		*/
		for(size_t i = 0; i < answer->size(); ++i)
		{
			if((*answer)[i] < 0)
			{
				cout << "No suggests"<<endl;
				break;
			}
			cout << (*words)[(*answer)[i]] << " |pr: " << (*frequency)[(*answer)[i]] << endl;
		}
		cout <<'\n' <<endl;
		delete answer;
	}
	delete frequency;
	delete words;
	return 0;
}