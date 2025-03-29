#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <omp.h>

struct array
{
	long long number;
	int numberType;
};

int deficient(int x)
{
	int sum = 1, i;

	if(x==0)
	{
		return 0;
	}
	if(x==1)
	{
		return 1;
	}
	for(i=2;i<x;i++)
	{
		if(x%i==0)
		{
			sum = sum + i;
		}
	}

	if(sum < x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int perfect(int x)
{
	int sum = 1, i;

	if(x==0)
	{
		return 1;
	}
	if(x==1)
	{
		return 0;
	}
	for(i=2;i<x;i++)
	{
		if(x%i==0)
		{
			sum = sum + i;
		}
	}

	if(sum == x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int abundant(int x)
{
	int sum = 1, i;

	if(x==0 || x==1)
	{
		return 0;
	}
	for(i=2;i<x;i++)
	{
		if(x%i==0)
		{
			sum = sum + i;
		}
	}
	if(sum > x)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int main(int argc, char* argv[])
{
	int n, N, i, j, perfect1 = 0, abundant1 = 0, deficient1 = 0, perfect2 = 0, abundant2 = 0, deficient2 = 0;
	long long i2;
	struct array* v1;
	struct array* v2;
	struct timeval start, end;
	if(argc != 3)
	{
		printf("Usage: n, number of threads");
		return 1;
	}
	n = atoi(argv[1]);
	N = atoi(argv[2]);
	
	if(n <= 0 || N <= 0)
	{
		printf("Usage: n > 0, threads > 0 (positive integers)");
	}

	v1 = (struct array*)calloc(n, sizeof(struct array));
	v2 = (struct array*)calloc(n, sizeof(struct array));
	/////////////////////////////// array generation sequential
	
	gettimeofday(&start, NULL);
	for(i2=0;i2<n;i2++)
	{
		v1[i2].number = i2;
	}
	gettimeofday(&end, NULL);

	long sz1s = end.tv_sec - start.tv_sec;
	long sz1m = ((sz1s * 1000000) + end.tv_usec) - (start.tv_usec);

	/// array generation parallel
	gettimeofday(&start, NULL);
	#pragma omp parallel for num_threads(N) default(none) shared(n,v2) private(i)
	for(i2=0;i2<n;i2++)
	{
		v2[i2].number = i2;
	}
	gettimeofday(&end, NULL);
	long p1s = end.tv_sec - start.tv_sec;
	long p1m = ((p1s * 1000000) + end.tv_usec) - (start.tv_usec);

	////////////////////////////// classification sequential
	
	gettimeofday(&start, NULL);
	for(i=0;i<n;i++)
	{
		if(deficient(v1[i].number) == 1)
		{
			v1[i].numberType = 1;
			deficient1 = deficient1 + 1;
		}
		else if(perfect(v1[i].number) == 1)
		{
			v1[i].numberType = 2;
			perfect1 = perfect1 + 1;
		}
		else if(abundant(v1[i].number) == 1)
		{
			v1[i].numberType = 3;
			abundant1 = abundant1 + 1;
		}
	}
	gettimeofday(&end, NULL);
	long sz2s = end.tv_sec - start.tv_sec;
	long sz2m = ((sz2s * 1000000) + end.tv_usec) - (start.tv_usec);

	/// classification parallel
	
	gettimeofday(&start, NULL);
	#pragma omp parallel for num_threads(N) default(none) shared(n,v2) private(i) reduction(+:deficient2,perfect2,abundant2)
	for(i=0;i<n;i++)
	{
		if(deficient(v2[i].number) == 1)
		{
			v2[i].numberType = 1;
			deficient2 = deficient2 + 1;
		}
		else if(perfect(v2[i].number) == 1)
		{
			v2[i].numberType = 2;
			perfect2 = perfect2 + 1;
		}
		else if(abundant(v2[i].number) == 1)
		{
			v2[i].numberType = 3;
			abundant2 = abundant2 + 1;
		}
	}
	gettimeofday(&end, NULL);
	long p2s = end.tv_sec - start.tv_sec;
	long p2m = ((p2s * 1000000) + end.tv_usec) - (start.tv_usec);

	////////////////////////////// sorting sequentially
	int p;
	struct array temp;
	gettimeofday(&start, NULL);
	for(p=0;p<n;p++)
	{
		if(p%2==0)
		{
			for(i=1;i<n;i=i+2)
			{
				if(v1[i-1].numberType > v1[i].numberType)
				{
					temp = v1[i-1];
					v1[i-1] = v1[i];
					v1[i] = temp;
				}
			}
		}
		else
		{
			for(i=1;i<n-1;i=i+2)
			{
				if(v1[i].numberType > v1[i+1].numberType)
				{
					temp = v1[i+1];
					v1[i+1] = v1[i];
					v1[i] = temp;
				}
			}
		}
	}
	gettimeofday(&end, NULL);
	long sz3s = end.tv_sec - start.tv_sec;
	long sz3m = ((sz3s * 1000000) + end.tv_usec) - (start.tv_usec);

	/// sorting in parallel
	gettimeofday(&start, NULL);
	for(p=0;p<n;p++)
	{
		if(p%2==0)
		{
			#pragma omp parallel for num_threads(N) default(none) shared(v2,n) private(i,temp)
			for(i=1;i<n;i=i+2)
			{
				if(v2[i-1].numberType > v2[i].numberType)
				{
					temp = v2[i-1];
					v2[i-1] = v2[i];
					v2[i] = temp;
				}
			}
		}
		else
		{
			#pragma omp parallel for num_threads(N) default(none) shared(v2,n) private(i,temp)
			for(i=1;i<n-1;i=i+2)
			{
				if(v2[i].numberType > v2[i+1].numberType)
				{
					temp = v2[i+1];
					v2[i+1] = v2[i];
					v2[i] = temp;
				}
			}
		}
	}
	gettimeofday(&end, NULL);
	long p3s = end.tv_sec - start.tv_sec;
	long p3m = ((p3s * 1000000) + end.tv_usec) - (start.tv_usec);

	/////////////////////////////
	printf("SEQUENTIAL RESULTS:\n");
	for(i=0;i<n;i++)
	{
		printf("number: %d; type: %d\n", v1[i].number, v1[i].numberType);
	}
	printf("deficient: %d, perfect: %d, abundant: %d\n", deficient1, perfect1, abundant1);

	printf("\nPARALLEL RESULTS:\n");
	for(i=0;i<n;i++)
	{
		printf("number: %d; type: %d\n", v2[i].number, v2[i].numberType);
	}
	printf("deficient: %d, perfect: %d, abundant: %d\n\n", deficient2, perfect2, abundant2);

	float speedup, efficiency;

	printf("Generation time sequential: %ld sec %ld microsec\n", sz1s, sz1m);
	printf("Generation time parallel: %ld sec %ld microsec\n", p1s, p1m);
	speedup = (float)sz1m / p1m;
	efficiency = (float)speedup / N;
	printf("Speedup in generation: %f\n", speedup);
	printf("Efficiency in generation: %f\n\n", efficiency);

	printf("Classification time sequential: %ld sec %ld microsec\n", sz2s, sz2m);
	printf("Classification time parallel: %ld sec %ld microsec\n", p2s, p2m);
	speedup = (float)sz2m / p2m;
	efficiency = (float)speedup / N;
	printf("Speedup in classification: %f\n", speedup);
	printf("Efficiency in classification: %f\n\n", efficiency);

	printf("Sorting time sequential: %ld sec %ld microsec\n", sz3s, sz3m);
	printf("Sorting time parallel: %ld sec %ld microsec\n", p3s, p3m);
	speedup = (float)sz3m / p3m;
	efficiency = (float)speedup / N;
	printf("Speedup in sorting: %f\n", speedup);
	printf("Efficiency in sorting: %f\n", efficiency);

	free(v1);
	free(v2);
	return 0;
}
