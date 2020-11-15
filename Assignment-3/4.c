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
    int a[1000];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int length = atoi(argv[1]);
    int flag = atoi(argv[2]);
    int length1, ad,pb;
    if (flag == 1)
    {
        length1=length/np;
        if (rank == np - 1)
        {
            pb = length % np;
            length1 += pb;
        }
        int i = 0;
        while (i < length1)
        {
            if (rank == np - 1)
                a[i] = rank * (length1 - pb) + i;
            else
                a[i] = rank * length1 + i;
            i++;
        }
    }
    else if(flag==2)
    {
        length1=length/np;
        if(rank < length % np && length % np != 0)
            length1++;
        int i=0;
        while(i*np+rank < length)
        {
            a[i]=i*np+rank;
            i++;
        }
    }
    else
    {
        ad=2;
        int pb1 = ad*np;
        length1=length/pb1;
        
        
    }
    MPI_Barrier(MPI_COMM_WORLD);
    

    int local_sum = 0,i;
    for (i = 0; i < length1; i++)
        local_sum += a[i];
    int sum = 0;
    MPI_Reduce(&local_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        cout<<"Total sum: "<<sum<<endl;
    }
    MPI_Finalize();
    
}
