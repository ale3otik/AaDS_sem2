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

void SetDictionary(vector<pair<string,int>> & dictionary, std::ifstream & read_stream)
{

	string buffer;
	int count;
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

}
int main()
{
	/*
	* Dictionary inicialize
	*/
	std::ifstream read_stream("dict.txt");
	
	vector<pair<string , int>> dictionary;
	SetDictionary(dictionary, read_stream);

	Suggester SevenKdict(dictionary);

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
    	vector<pair<string,int>> answer = SevenKdict.GetSuggests(request,suggests_size);

		/*
		* Print out
		*/
		for(size_t i = 0; i < answer.size(); ++i)
		{
			
			cout << answer[i].first << " |pr: " << answer[i].second << endl;
		}
		cout <<'\n' <<endl;
	}
	return 0;
}