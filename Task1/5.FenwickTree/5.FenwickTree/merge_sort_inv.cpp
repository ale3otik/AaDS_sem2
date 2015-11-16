#include <cassert>
#include <stdlib.h>
int * merge_inv(int* arr_1 , int N , int *count)
{
	int* arr[2];
	arr[0] = arr_1;
	assert(arr[1] = (int*)malloc(sizeof(int)* N));
	int length , k = 1 , i_first , i_sec , j;
	int first , second;
	for(length = 1; length < N; length *= 2)
	{
		k += 1;
		k = k % 2;
		for(first = 0; first < N; first += 2 * length)
		{
			second = first + length;
			i_first = first;
			i_sec = second;
			j = first;
			while(j < second + length && j < N)
			{
				if(i_first < first + length && i_sec < second + length && i_sec < N)
				{
					if(arr[k][i_sec] < arr[k][i_first])
					{
						(*count) += (first + length) - i_first;
						arr[(k + 1) % 2][j] = arr[k][i_sec];
						i_sec++;
						j++;
					}
					else
					{
						arr[(k + 1) % 2][j] = arr[k][i_first];
						i_first++;
						j++;
					}
				}
				else if(i_sec < second + length && i_sec < N)
				{
					arr[(k + 1) % 2][j] = arr[k][i_sec];
					i_sec++;
					j++;
				}
				else
				{
					//(*count)++;
					arr[(k + 1) % 2][j] = arr[k][i_first];
					i_first++;
					j++;
				}
			}
			//(*count)--;
		}
	}
	// if( k == 0) free(arr[k]);
	return arr[(k + 1) % 2];
}