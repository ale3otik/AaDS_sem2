#ifndef __FENWICK_TREE_H__
	#define __FENWICK_TREE_H__
#include<vector>
class FenwickTree
{
public:
	FenwickTree(std:: vector<int> & data);
	FenwickTree::FenwickTree(size_t size , int value);
	~FenwickTree();
	void addValue(long long index , int value);
	int getSum(long long left , long long right);
private:
	std::vector<int> tree;

	long long  get_next_index(long long cur);
	long long  get_prev_index(long long cur);
	int get_prefix_sum(long long cur);
};
/******************INVERSIONS******************/
int get_inversions(std::vector<int> & array);

#endif