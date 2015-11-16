#define _CRT_CSECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <assert.h>
#define STRAIGHT 1
#define REVERSE 2

#define IGNORE 0 // (diag + 0) when there  are same symb;
#define INSERT 1 // (down) insert symbol into First str;
#define DELETE 2 //(right) delete symbol from First str;
#define CHANGE 3 //(diag) replace symbol in First from Second str;

using std::list;
using std::vector;
using std::string;
using std::ifstream;

struct iter // to absolute index in 
{
	int begin;
	int end;
}; 

struct editOrder // structue describes the texts diffrences
{
	int Position;
	int type;
	char sym;
};

static int min_index(int a , int b , int c) // if c == min(a,b) func will return (3) c
{
	if(a < b)
	{
		if(a < c) return 1;
		else return 3;
	}
	if(b < c) return 2;
	return 3;
}

static int min(int a , int b , int c)
{
	if(a < b)
	{
		if(a < c) return a;
		else return c;
	}
	if(b < c) return b;
	return c;
}



static int FindMin(vector<int> & column1 , vector<int> & column2) // find minimum to split Second into two parts
{
	int  length = column1.end() - column1.begin();
	int min = 0;
	for(int i = 0; i < length; ++i)
	{
		if(column1[i] + column2[length-1-i] < column1[min] + column2[length-1-min]) min = i;
	}
	return min;
}

/*
*	This function can be used as basic algorithm O(M*N) memory or as final part of O(max(M,N)) algorithm.
*/
static list<editOrder> * FinalCountRange(string & First , string & Second , iter it_first , iter it_second)
{
	//count length
	int L1 = it_first.end - it_first.begin;
	int L2 = it_second.end - it_second.begin;

	//create counting table
	vector<vector<int>> Table(L2+1,vector<int>(L1+1));

	//initialize first column and string
	for(int i = 0; i < L2 + 1; ++i)
	{
		Table[i][0] = i;
	}
	for(int i = 0; i < L1 + 1; ++i)
	{
		Table[0][i] = i;
	}

	// count matrix
	int check; // to diag step
	for(int i = 1; i < L1 + 1; ++i)
	{
		for(int j = 1; j < L2 + 1; ++j)
		{
			if(First[i-1+ it_first.begin] == Second[j-1 + it_second.begin]) check = 0;
			else check = 1;
			Table[j][i] = min(Table[j][i - 1] + 1 , Table[j-1][i] + 1 , Table[j-1][i - 1] + check);
		}
	}

	// recreate reverse way and get edit order
	int i = L1;
	int j = L2;
	list<editOrder> * ANS = new list<editOrder>;
	assert(ANS);

	while(i>0 || j>0) 
	{
		if(i == 0)
		{
			ANS->push_front({it_first.begin + i -1, INSERT , Second[j - 1 + it_second.begin] }); 
			--j;
		}
		else if(j == 0)
		{
			ANS->push_front({ it_first.begin+i -1 , DELETE , First[i-1 + it_first.begin] });
			--i;
		}
		else
		{
			int min = min_index(Table[j-1][i] , Table[j][i-1] , Table[j - 1][i - 1]);
			switch(min)
			{
				case 1:
					ANS->push_front({ it_first.begin + i -1 , INSERT , Second[j - 1 + it_second.begin] });
					--j;
					break;
				case 2:
					ANS->push_front({ it_first.begin + i -1, DELETE , First[i-1 + it_first.begin] });
					--i;
					break;
				case 3:
					if(Table[j - 1][i - 1] != Table[j][i])
					{
						ANS->push_front({ it_first.begin + i -1 , CHANGE , Second[j - 1 + it_second.begin] });
					}
					--i;
					--j;
					break;

			}
		}
	}
	return ANS;
}

/*
*	Count column in right or reverse order
*/
static vector<int> * CountMatrix(string & First , string & Second , iter it_first , iter it_second,int param)
{
	//start initialize
	vector<int> * Table[2];
	int L1 = it_first.end - it_first.begin;
	int L2 = it_second.end - it_second.begin;
	assert(Table[0] = new vector<int>(L2+1));
	assert(Table[1] = new vector<int>(L2+1));
	for(int i = 0; i < L2 + 1; ++i)
	{
		(*Table[0])[i] = i;
	}

	int k = 1; // to count columns
	int j = 1; // to count currrent column index ( i = k mod 2)
	int K1  = 0, K2 = 0;

	if(param == REVERSE)
	{
		K1 = L1;
		K2 = L2;
	}

	while(k < L1+1)
	{
		(*Table[j])[0] = k;
		for(int i = 1; i < L2+1; ++i)
		{
			int check;
			if(param != REVERSE)
			{
				K1 = 2 * k-1;
				K2 = 2 * i-1;
			}
			if(First[K1 - k+ it_first.begin] == Second[K2 - i+ it_second.begin]) check = 0;
			else check = 1;
			(*Table[j])[i] = min((*Table[j])[i - 1] + 1 , (*Table[(j + 1) % 2])[i] + 1 , (*Table[(j + 1) % 2])[i - 1] + check);
		}
		++k;
		j = (j + 1) % 2;
	}
	j = (j + 1) % 2; // second column
	int f = (j + 1) % 2; // first column

	delete Table[f];

	return Table[j];

}

/*
*	the main fucnction of program
*	manager of another functions
*	used recursively
*/
static list<editOrder> * GetEditOrder(string & First , string & Second , iter it_first, iter it_second)
{
	if(it_first.end - it_first.begin> 1 && it_second.end - it_second.begin > 1)
	{
		int middleColumn = it_first.begin + (it_first.end - it_first.begin) / 2 - 1; //index of middle column
		vector<int> * column1 = CountMatrix(First , Second , { it_first.begin , middleColumn+1} , it_second,STRAIGHT); // left part
		vector<int> * column2 = CountMatrix(First , Second , { middleColumn+1, it_first.end } , it_second,REVERSE); //right part
		
		int index = FindMin(*column1 , *column2);
		delete column1;
		delete column2;

		list<editOrder> * left = GetEditOrder(First , Second , { it_first.begin , middleColumn+1 } , { it_second.begin , it_second.begin+index});
		list<editOrder> * right = GetEditOrder(First , Second , { middleColumn + 1 , it_first.end } , { it_second.begin+ index , it_second.end });
		
		left->insert(left->end() , right->begin() , right->end()); // stick to part of edit order
		
		delete right;
		return left;
	}
	else
	{
		list<editOrder> * Answer = FinalCountRange(First , Second , it_first , it_second);
		return Answer;
	}
}
