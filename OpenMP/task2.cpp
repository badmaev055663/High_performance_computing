#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

// find maximum among minumums in row 
// m - rows, n - columns
int task2_omp(int** matrix, int m, int n)
{
	int max = 0;
	for (int i = 0; i < m; ++i)
	{
		int min_row_elem = matrix[i][0];
		max = min_row_elem;

        #pragma omp parallel for reduction(min:  min_row_elem)
		for (int j = 1; j < n; ++j)
		{
			min_row_elem = min_row_elem < matrix[i][j] ? min_row_elem : matrix[i][j];
		}
		max = min_row_elem > max ? min_row_elem : max;
	}
	return max;
}

//generate matrix [m, n]
int** generate_matrix(int m, int n)
{
	int** res = new int*[m];
	srand(time(NULL));
	for (int i = 0; i < m; ++i)
	{
		res[i] = new int[n];
		for (int j = 0; j < n; ++j)
		{
			res[i][j] = rand() % 100;
		}
	}
	return res;
}

int main()
{
	int m = 6, n = 8;
	int** matrix = generate_matrix(m, n);
	
	printf("max: %d\n", task2_omp(matrix, m, n));
	for (int i = 0; i < m; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	return 0;
}