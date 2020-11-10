#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

int main()
{
	const long n = 2000;
	long** a, ** b;
	long long** c;

	a = new long* [n];
	b = new long* [n];
	for (int i = 0; i < n; i++)
	{
		a[i] = new long[n];
		b[i] = new long[n];
		for (long j = 0; j < n; j++)
		{
			a[i][j] = i + j;
			b[i][j] = i - j;
		}
	}

	c = new long long* [n];
	for (long i = 0; i < n; i++)
	{
		c[i] = new long long[n];
	}

	unsigned int start_time = clock();
#pragma omp parallel shared(c) num_threads(8)
	{
#pragma omp for collapse(2) schedule(static)
		for (long i = 0; i < n; i++)
		{
			for (long j = 0; j < n; j++)
			{
				c[i][j] = 0;
				for (long k = 0; k < n; k++)
					c[i][j] += (long long)(a[i][k] * b[k][j]);
			}
		}
	}
	unsigned int end_time = clock();
	unsigned int search_time = end_time - start_time;
	cout << "Parallel " << search_time << endl;


	start_time = clock();
	{
		for (long i = 0; i < n; i++)
		{
			for (long j = 0; j < n; j++)
			{
				c[i][j] = 0;
				for (long k = 0; k < n; k++)
					c[i][j] += (long long)(a[i][k] * b[k][j]);
			}
		}
	}
	end_time = clock();
	search_time = end_time - start_time;
	cout << "No parallel " << search_time << endl;
}

