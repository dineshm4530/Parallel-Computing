#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    int rank, np;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int length = atoi(argv[1]);
    int length1 = length / np;
    int pb;
    if (rank == np - 1)
    {
        pb = length % np;
        length1 += pb;
    }
    int a[length1];
    int i = 0;
    while (i < length)
    {
        if(rank==np-1)
            a[i] = rank * (length1-(pb)) + i;
        else
            a[i]=rank*length+i;
        i++;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    

    int local_sum = 0;
    for (i=0;i<length1;i++)
        local_sum+=a[i];
    int sum=0;
    MPI_Reduce(&local_sum,&sum,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0)
    {
        cout<<"Total sum: "<<sum<<endl;
    }
    MPI_Finalize();
    return 0;
    
}
