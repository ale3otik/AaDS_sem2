#ifndef __RANGE_TREE_ALG__
#define __RANGE_TREE_ALG__

#include <vector>
#include <cassert>
#include <iostream>
#include <algorithm> 

using std::cout;
using std::endl;
using std::vector;

/*
*
*	Helps functions and structs
*
*/

template<class data_type>
static bool default_comparator(const data_type & first , const data_type & second)
{
	if(first < second) return true;
	return false;
}


template<class data_type>
class tree_node
{
public:
	tree_node()
	{
		elem = data_type();
		mod = data_type();
		fictitious = true;
		index = 0;
	}
	~tree_node() { ; }
	data_type  elem;
	data_type  mod; // exist modification in this node
	bool fictitious; //  fictitious or not
	size_t index; // index of min elem

};

/**
*
*	Class RangeTree
*
**/

template<typename data_type>
class RangeTree
{
public:
	RangeTree(vector<data_type>*);
	RangeTree(vector<data_type>* , bool(*comp) (const data_type& , const data_type&));
	~RangeTree();

	std::pair<data_type , size_t>  RangeMinimumQuery(size_t left , size_t right);
	void AddToRange(size_t left , size_t right , data_type value);

private:

	vector<data_type> * data; // source aray
	vector<tree_node<data_type>> * tree; // tree aray
	size_t size_of_last_layer; // source array + inf elements
	size_t tree_real_index; // first index in tree point to (*data)[0]
	size_t real_size;
	
	bool(*comparator)(const data_type& , const data_type&); // comparator 
	void SetDefaultComparator();

	tree_node<data_type> minimum(tree_node<data_type> & first , tree_node<data_type> & second); //special minimum with nullptr catch
	tree_node<data_type> get_min(size_t first , size_t second); //special min associated with tree

	void CreateTree(vector<data_type> * Array); // make tree
	tree_node<data_type> TreeQuery(size_t left , size_t right , size_t cur , size_t left_tree_edge , size_t right_tree_edge); // used by RMQ

	void AddToTree(size_t left , size_t right , size_t cur , size_t left_tree_edge , size_t right_tree_edge , data_type value);
	void push(size_t cur);// used by AddToRange
	/************helps functions**********************/
	size_t find_next_bin_number(size_t size)
	{
		if(size == 0) return 0;
		size_t answer = size;
		answer = 1;
		while(answer < size)
		{
			answer = answer << 1;
		}

		return answer;
	}

	size_t get_right(size_t cur)
	{
		return cur * 2 + 2;
	}

	size_t get_left(size_t cur)
	{
		return cur * 2 + 1;
	}

	size_t get_parent(size_t cur)
	{
		if(cur == 0) return 0;
		return (cur - 1) / 2;
	}

};
/*********************Inicialize*********************/
template<class data_type>
RangeTree<data_type>::~RangeTree()
{
	delete tree;
}

template<class data_type>
void RangeTree<data_type>::CreateTree(vector<data_type> * Array)
{
	data = Array;
    real_size = data->size();
	if(real_size == 0)
	{
		cout << "array is empty"<<endl;
		exit(1);
	}
	size_of_last_layer = find_next_bin_number(real_size);
	assert(tree = new vector<tree_node<data_type>>(size_of_last_layer * 2 - 1 , tree_node<data_type>()));
	tree_real_index = tree->size() - size_of_last_layer;

	for(size_t i = tree_real_index; i < tree->size(); ++i)
	{
		if(!(i - tree_real_index< real_size))
		{
			break;
		}
		(*tree)[i].elem = (*data)[i - tree_real_index];
		(*tree)[i].index = i - tree_real_index;
		(*tree)[i].fictitious = false;
	}
	for(long long i = get_parent(tree->size() - 1); i >= 0; --i)
	{
		(*tree)[i]= get_min(get_right(i) , get_left(i)); 
	}
}

template<class data_type> 
RangeTree<data_type>::RangeTree(vector<data_type> * Array)
{
	SetDefaultComparator();
	CreateTree(Array);
}

template<class data_type>
RangeTree<data_type>::RangeTree(vector<data_type> * Array , bool(*comp) (const data_type& , const data_type&))
{
	comparator = comp;
	CreateTree(Array);
}


template<class data_type> 
void RangeTree<data_type>::SetDefaultComparator()
{
	comparator = default_comparator;
}


/******************AddToRange********************************/

template<class data_type> 
void RangeTree<data_type>::AddToRange(size_t left , size_t right , data_type value)
{
	if(left > right || left < 0 || right >= real_size)
	{
		cout << "Error: AddToRange:wrong index" << endl;
		exit(1);
	}
	AddToTree(left , right , 0 , 0 , size_of_last_layer - 1,value);
}

template<class data_type> 
void RangeTree<data_type>::AddToTree(size_t left , size_t right , size_t cur , size_t left_tree_edge , size_t right_tree_edge , data_type value)
{
	if(left == left_tree_edge && right == right_tree_edge)
	{
		if(cur >= tree_real_index)
		{
			(*tree)[cur].elem += value;
		}
		else
		{

			(*tree)[cur].mod += value;
		}
		return;
	}

	size_t middle = (right_tree_edge + 1 + left_tree_edge) / 2;
	push(cur);
	if(left< middle)
	{
		AddToTree(left , std::min(right , middle - 1) , get_left(cur) , left_tree_edge , middle - 1 , value);
	}
	if(right >= middle)
	{
		AddToTree(std::max(left , middle) , right , get_right(cur) , middle , right_tree_edge,value);
	}
	(*tree)[cur] = get_min(get_right(cur) , get_left(cur));
}

template<class data_type>
void RangeTree<data_type>::push(size_t cur)
{
	if(get_left(cur) < tree->size())
	{
		(*tree)[get_left(cur)].mod += (*tree)[cur].mod;
		(*tree)[get_right(cur)].mod += (*tree)[cur].mod;
	}
	(*tree)[cur].elem += (*tree)[cur].mod;
	(*tree)[cur].mod = data_type();
}
/*************************RMQ**********************************/
template<class data_type>
std::pair<data_type,size_t>  RangeTree<data_type>::RangeMinimumQuery(size_t left , size_t right)
{
	if(left > right || left < 0 || right >= real_size)
	{
		cout << "Error: RMQ:wrong index" << endl;
		exit(1);
	}
	tree_node<data_type> ans = TreeQuery(left , right , 0 , 0 , size_of_last_layer - 1);
	return std::make_pair(ans.elem + ans.mod,ans.index);
}

template<class data_type>
tree_node<data_type> RangeTree<data_type>::minimum(tree_node<data_type> & first , tree_node<data_type> & second)
{
	if(first.fictitious == true) return second;
	if(second.fictitious == true) return first;
	if(comparator(first.elem , second.elem))return first;
	return second;
}

template<class data_type>
tree_node<data_type> RangeTree<data_type>::get_min(size_t first , size_t second)
{
	tree_node<data_type> ans;
	if(first >= tree->size())
	{
		cout << "\nError: get_min\n" << endl;
		exit(1);
	}

	if((*tree)[first].fictitious == true)
	{
		ans = (*tree)[second];
		ans.mod = data_type();
		return ans;
	}
	if((*tree)[second].fictitious == true)
	{
		ans = (*tree)[first];
		ans.mod = data_type();
		return ans;
	}

	if(comparator((*tree)[first].elem + (*tree)[first].mod , (*tree)[second].elem + (*tree)[second].mod))
	{
		ans = (*tree)[first];
	}
	else
	{
		ans = (*tree)[second];
	}
	ans.elem += ans.mod;
	ans.mod = data_type(); // to stop copying of mod buffer to high flatsaa
	return ans;
}

template<class data_type> 
tree_node<data_type> RangeTree<data_type>::TreeQuery(size_t left , size_t right , size_t cur , size_t left_tree_edge , size_t right_tree_edge)
{

	push(cur); // update node

	if(left == left_tree_edge && right == right_tree_edge)
	{
		return (*tree)[cur];
	}

	tree_node<data_type> result; 
	size_t middle = (right_tree_edge + 1 + left_tree_edge) / 2;

	if(left< middle)
	{
		result = minimum(result , TreeQuery(left , std::min(right , middle - 1) , get_left(cur) , left_tree_edge , middle - 1));
	}
	if(right >= middle)
	{
		result = minimum(result , TreeQuery(std::max(left , middle) , right , get_right(cur) , middle , right_tree_edge));
	}
	return result;
}
#endif