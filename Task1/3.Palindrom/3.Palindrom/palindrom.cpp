#include<iostream>
#include<string>
#include<vector>
#include <algorithm>

#include <cassert>

using std::string;
using std::vector;

string LargestPalindrom(string str)
{
	int size = str.size();
	vector<vector<int>> Dp(size, vector<int>(size,0));
	for(int i = 0, j = 0; i < size; ++i , ++j) // j -str, i-column
	{
		Dp[j][i] = 1;
		if(j != 0) Dp[j-1][i] = 0;
	}
	/*******MAKE TABLE**********/
	for(int i = size - 2; i >= 0; --i)
	{
		for(int j = i + 1; j < size; ++j)
		{
			if(str[i] == str[j]) Dp[j][i] = Dp[j - 1][i + 1]+2;
			else
			{
				Dp[j][i] = std::max(Dp[j - 1][i] , Dp[j][i + 1]);
			}
		}
	}
	/***********Get Palindrom*************/
	string answer;
	answer.resize((Dp[size - 1][0]));
	int i = 0,left = 0;
	int j = size - 1 , right = Dp[size - 1][0]-1;
	while(true)
	{
		if(Dp[j][i] == 0) break;
		if(i == j)
		{
			assert(left == right);
			answer[left] = str[j];
			break;
		}

		if(str[i] == str[j])
		{
			answer[left] = (answer[right] = str[i]);
			++left;
			--right;
			++i;
			--j;
		}
		else
		{
			if(Dp[j][i + 1] > Dp[j - 1][i])
			{
				++i;
				continue;
			}
			--j;

		}
	}

	
	return answer;;
}
int main()
{
	string str;
	std::cin >> str;
	string ans = LargestPalindrom(str);
	std::cout << ans;
	while(1);
}