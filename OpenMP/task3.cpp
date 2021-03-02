#include <omp.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>


static int num_threads = 8;

/* matrix-vector multimplication by rows using omp
   matrix size - [m, n]
   vector size - [n]
   
*/
int* matrix_vector_omp(int** matrix, int* vector, int m, int n)
{
	int* res = new int [m];
	double t1 = omp_get_wtime();
    #pragma omp parallel for
	for (int i = 0; i < m; ++i)
	{
		int sum = 0;
		for (int j = 0; j < n; ++j)
		{
			sum += matrix[i][j] * vector[j];
		}
		res[i] = sum;
	}
	double t2 = omp_get_wtime();
	printf("time (omp): %.2lf ms\n", (t2 - t1) * 1000.0);
	return res;
}


// block multiplication
void block_vec_mult(int** matrix, const int* vector, int* res, int m, int n, int block_index)
{
	int block_row_size = m / num_threads * 2;
	int block_col_size = n / 2;
	int block_row_start = block_index / 2;
	int block_col_start = block_index % 2;

	for (int i = block_row_start * block_row_size; i < (block_row_start + 1) * block_row_size; ++i)
	{
		int sum = 0;

		for (int j = block_col_start * block_col_size; j < (block_col_start + 1) * block_col_size; ++j)
		{
			sum += matrix[i][j] * vector[j];
		}
		#pragma omp atomic
		res[i] += sum;
	}
}

// block multiplication
// assume n, m are even
int* matrix_vector_omp_block(int** matrix, int* vector, int m, int n)
{
	
	int* res = new int[m];
	for (int i = 0; i < m; ++i)
	{
		res[i] = 0;
	}
	double t1 = omp_get_wtime();
    #pragma omp parallel 
	{
		block_vec_mult(matrix, vector, res, m, n, omp_get_thread_num());
	}
	double t2 = omp_get_wtime();
	printf("time (omp block): %.2lf ms\n", (t2 - t1) * 1000.0);
	return res;
}


/* matrix-vector multimplication
   matrix size - [m, n]
   vector size - [n]
*/
int* matrix_vector(int** matrix, const int* vector, int m, int n)
{
	int* res = new int[m];
	double t1 = omp_get_wtime();
	for (int i = 0; i < m; ++i)
	{
		int sum = 0;
		for (int j = 0; j < n; ++j)
		{
			sum += matrix[i][j] * vector[j];
		}
		res[i] = sum;
	}
	double t2 = omp_get_wtime();
	printf("time: %.2lf ms\n", (t2 - t1) * 1000.0);
	return res;
}



//generate matrix [m, n]
int** generate_matrix(int m, int n)
{
	int** res = new int* [m];
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

int* generate_vector(int n)
{
	int* res = new int[n];
	srand(time(NULL));
	for (int i = 0; i < n; ++i)
	{
		res[i] = rand() % 100;
	}
	return res;
}

bool check(const int* vec1, const int* vec2, int n)
{
	for (int i = 0; i < n; ++i)
	{
		if (vec1[i] != vec2[i])
		{
			return false;
		}
	}
	return true;
}


int main()
{
	int m = 1000, n = 2000;
	int** matrix = generate_matrix(m, n);
	int* vec = generate_vector(n);

	omp_set_num_threads(num_threads);
	int* res_omp = matrix_vector_omp(matrix, vec, m, n);
	int* res_block = matrix_vector_omp_block(matrix, vec, m, n);
	int* res = matrix_vector(matrix, vec, m, n);



	if (check(res, res_omp, m) == false)
	{
		printf("results are different!!!!!!\n");
	}
	if (check(res_block, res, m) == false)
	{
		printf("block result is false!!!!!!\n");
	}


	for (int i = 0; i < m; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
	delete[] vec;
	delete[] res;
	delete[] res_omp;
	delete[] res_block;
	return 0;
}