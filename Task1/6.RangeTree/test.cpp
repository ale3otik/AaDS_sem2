#include <vector>
#include <iostream>
#include <fstream>

#include "RangeTree.cpp"

int main()
{
	using namespace std;
	vector<int> a;
	using namespace std;
	std::ifstream cin("in.txt");
	int b;
	while(true)
	{

		cin >> b;
		if(!cin) break;
		a.push_back(b);
	}
	RangeTree<int>L(&a);
	L.AddToRange(0,2,3);
	cout << " "<<int(0)<<" ";
	pair<int , int> ans = L.RangeMinimumQuery(0 , 4);
	cout << ans.first << endl;
	cout << ans.second << endl;
	//ss
	while(1);
	return 0;
}