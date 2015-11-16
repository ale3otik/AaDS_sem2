#include <cassert>
#include "FenwickTree.h"

using std::vector;

FenwickTree::FenwickTree(vector<int> & data)
{
	tree.assign(data.size() , 0);
	for(long long i = 0; i < data.size(); ++i)
	{
		addValue(i , data[i]);
	}

}
FenwickTree::FenwickTree(size_t size,int value)
{
	tree.assign(size , 0);

}

FenwickTree::~FenwickTree(){}

void FenwickTree::addValue(long long index , int value)
{
	if(index < 0) return;
	while(index < tree.size())
	{
		tree[index] += value;
		index = get_next_index(index);
	}
}

int	FenwickTree::getSum(long long left , long long right)
{
	assert(!(left < 0 || right >= tree.size() || left>right));
	return(get_prefix_sum(right) - get_prefix_sum(left - 1));
}

long long FenwickTree::get_prev_index(long long cur)
{
	int ans = (cur & (cur + 1))-1;
	return ans;
}
long long  FenwickTree::get_next_index(long long cur)
{
	return cur | (cur + 1);
}
int FenwickTree::get_prefix_sum(long long cur)
{
	int answer = 0;
	while(cur >=0)
	{
		answer += tree[cur];
		cur = get_prev_index(cur);
	}
	return answer;
}


/*************************INVERSIONS*************************/
int get_inversions(vector<int> & array)
{
	int answer = 0;
	int delta = 0;
	int max = array[0];
	int min = array[0];
	for(int i = 0; i < array.size(); ++i)
	{
		if(max < array[i]) max = array[i];
		if(min > array[i]) min = array[i];
	}
	if(min < 0) delta = -min;
	FenwickTree tree(max + delta + 2 , 0);
	for(int i = 0; i < array.size(); ++i)
	{
		answer += tree.getSum(array[i] + delta + 1 , max + delta + 1);
		tree.addValue(array[i] + delta , 1);
	}
	return answer;
}