// MPI_lab_3.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdlib.h>
#include <malloc.h>
#include "mpi.h"
#include <stdio.h>
#include <limits>

double genRandDouble(int N)
{
	return ((double)(rand())/RAND_MAX)*N;
}

int genRandInt(int N)
{
	return (int)(((double)(rand())/RAND_MAX)*N);
}

int _tmain(int argc, char* argv[])
{
	int size, rank;

	int const N= 10;
	int arr[N]= {7,4,6,2,8,1,10,3,9,5};

	int valuesPerProcess= 0;

	//min
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double startExecutionTime= MPI_Wtime();

	//first part division array on parts between processors BEGIN

	valuesPerProcess= N / size;

	int firstIndex= valuesPerProcess*rank;
	int lastIndex= 0;
	
	if(rank!= size-1)
	{
		lastIndex = (valuesPerProcess*rank)+valuesPerProcess;
	}
	else
	{
		lastIndex= N;
	}

	int minValue= arr[lastIndex-1];	

	for(int i= lastIndex-1; i>= firstIndex; i--)
	{
		if(arr[i]< minValue)
		{
			minValue= arr[i];
		}
	}

	int maxValue= arr[firstIndex];	

	for(int i= firstIndex; i< lastIndex; i++)
	{
		if(arr[i]> maxValue)
		{
			maxValue= arr[i];
		}
	}

	printf("\n--------------------\nprocessor #%i: \n", rank);

	for(int i= lastIndex-1; i>= firstIndex; i--)
	{
		printf("[%i] ", arr[i]);
	}

	printf("\nmin value: %i\n", minValue);
	printf("\nmax value: %i\n--------------------\n", maxValue);

	//first part division array on parts between processors END

	printf("\nprocessor[%i] compare part:\n", rank);

	int recvValueMin= minValue;
	int recvValueMax= maxValue;

	int chetNechetSortTrigger= 1;

	for(int i= 0; i< size; i++)
	{
		printf("\niteration #%i: \n", i);

		printf("\nminValue: %i\n", minValue);
		printf("\nmaxValue: %i\n", maxValue);

		if(rank == 0 || rank % 2 == 0)
		{
			//printf("chet");
			if(chetNechetSortTrigger == 1)
			{
				if(rank+1< size)
				{
					MPI_Recv(&recvValueMin, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
					printf("\nrecvMinValue: %i\n", recvValueMin);

					MPI_Recv(&recvValueMax, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
					printf("\nrecvMaxValue: %i\n", recvValueMax);
				}
			}
			else if(chetNechetSortTrigger == -1)
			{
				if(rank-1>= 0)
				{
					//printf("nechet");
					printf("\nsendMinValue: %i\n", minValue);
					MPI_Send(&minValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

					printf("\nsendMaxValue: %i\n", maxValue);
					MPI_Send(&maxValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					//break;
				}
			}
			else
			{
				printf("/nsomething go wrong with \"chetNechetSortTrigger\"/n");
			}
		}
		else
		{
			if(chetNechetSortTrigger == 1)
			{
				if(rank-1>= 0)
				{
					//printf("nechet");
					printf("\nsendMinValue: %i\n", minValue);
					MPI_Send(&minValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);

					printf("\nsendMaxValue: %i\n", maxValue);
					MPI_Send(&maxValue, 1, MPI_INT, rank-1, 0, MPI_COMM_WORLD);
					//break;
				}
			}
			else if(chetNechetSortTrigger == -1)
			{
				if(rank+1< size)
				{
					MPI_Recv(&recvValueMin, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
					printf("\nrecvMinValue: %i\n", recvValueMin);

					MPI_Recv(&recvValueMax, 1, MPI_INT, rank+1, 0, MPI_COMM_WORLD, &status);
					printf("\nrecvMaxValue: %i\n", recvValueMax);
				}
			}
			else
			{
				printf("/nsomething go wrong with \"chetNechetSortTrigger\"/n");
			}
		}

		if(recvValueMin< minValue)
		{
			minValue= recvValueMin;
		}

		if(recvValueMax> maxValue)
		{
			maxValue= recvValueMax;
		}

		chetNechetSortTrigger*= -1;

	}

	if(rank == 0)
	{
		printf("\nFinal min value: %i\n",  minValue);
		printf("\nFinal max value: %i\n",  maxValue);

		double executionTime= MPI_Wtime()-startExecutionTime;
		printf("\nExecution time: %f\n", executionTime);
	}

	MPI_Finalize();


	MPI_Finalize();


	//system("pause");
	return 0;
}

