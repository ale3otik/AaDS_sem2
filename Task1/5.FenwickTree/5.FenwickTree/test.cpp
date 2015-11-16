#include <iostream>
#include <vector>
#include <random>
#include <fstream>
using std::vector;

#include "FenwickTree.h"
#include"merge_sort_inv.h"

std::default_random_engine generator;

std::vector<int> generate_random_permutation(size_t size)
{
	std::vector<int> answer;

	for(size_t i = 0; i < size; ++i)
		answer.push_back(i);

	for(size_t i = 0; i < size; ++i)
	{
		std::uniform_int_distribution<size_t> random_index(0 , size - 1);
		size_t j = random_index(generator);
		std::swap(answer[i] , answer[j]);
	}

	return answer;
}

int main()
{
	int length;
	std::cin >> length;
/*	my_array.reserve(length);
	for(int i = 0; i < length; ++i)
	{
		int buff;
		std::cin >> buff;
		my_array.push_back(buff);
	}
	*/
	std::ofstream cout("inversions_tests.txt");
	for(int counter = 0; counter < 500; ++counter)
	{
		vector<int> my_array = generate_random_permutation(length);
		int answer1 = get_inversions(my_array); //by Fenwick

		/* to merge*/
		int * c_array_to_merge = new int[my_array.size()];
		for(int i = 0; i < my_array.size(); ++i) c_array_to_merge[i] = my_array[i];
		int answer2 = 0;
		merge_inv(c_array_to_merge , my_array.size() , &answer2);
		/**********/

		cout << "fenw: " << answer1 << " merge: " << answer2 << std::endl;
		delete c_array_to_merge;
	}
	//while(1);
	return 0;
}