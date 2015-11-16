
/*Alexey Zotov*/

#ifndef __TravellingSalesmanProblem__DisjointSetUnion__
#define __TravellingSalesmanProblem__DisjointSetUnion__

#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <random>

template<typename type>
class DisjointSetUnion
{
public:
    DisjointSetUnion():random_parent(0,1){};
    DisjointSetUnion(type value);
    ~DisjointSetUnion(){};
    type Find(type value);
    void Union(type first, type second);
    bool MakeSet(type value); //true if succes, false if already exists
private:
    std::map <type, type> parents;
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> random_parent;
};

template<typename type>
DisjointSetUnion<type>::DisjointSetUnion(type value):
random_parent(0,1)
{
    parents[value] = value;
}

template<typename type>
type DisjointSetUnion<type>::Find(type value)
{
    typename std::map<type, type>::iterator it;
    std::vector< typename std::map<type,type>::iterator> nodes_to_update;
    for(it = parents.find(value); value != it->second; it = parents.find(value))
    {
        nodes_to_update.push_back(it);
        value = it->second;
    }
    
    for(int i = 0 ; i < nodes_to_update.size(); ++i){
        nodes_to_update[i]->second = it->second;
    }
    
    return it->second;
}

template <typename type>
bool DisjointSetUnion<type>::MakeSet(type value)
{
    if(parents.find(value) != parents.end()) return false;
    parents[value] = value;
    return true;
}

template<typename type>
void DisjointSetUnion<type>::Union(type first , type second)
{
    type left = Find(first);
    type right = Find(second);
    if(random_parent(generator)){
        parents[left] = right;
    }
    else{
        parents[right] = left;
    }
}

#endif /* defined(__TravellingSalesmanProblem__DisjointSetUnion__) */
