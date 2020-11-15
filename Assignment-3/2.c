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
    double a[length];
    for (int m=0;m<np;m++)
    {
        if (rank == m)
        {
            printf("Rank:%d\n",rank);
            
            double lbound = 0;
            double ubound = 10;
            std::uniform_real_distribution<double> urd(lbound, ubound);
            std::default_random_engine re;
            
            for (int i = 0; i < length; i++)
            {
                a[i] = urd(re);
                cout<<a[i];
            }
            cout<<"\n";
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
  
    if (rank == 0)
    {
        MPI_Send(&a[0],1,MPI_INT,((rank+1) % np), 0 ,MPI_COMM_WORLD);
   	MPI_Recv(&a[0],1,MPI_INT,np-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else
    {
        MPI_Recv(&a[0],1,MPI_INT,(rank-1) , 0 , MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  	MPI_Send(&a[0],1,MPI_INT,((rank+1) % np) , 0 ,MPI_COMM_WORLD);
    }
    
    for (int j = 0; j < np; j++)
    {
        if (rank == j)
        {
            printf("Rank:%d\n",rank);
            for (int i = 0; i < length; i++)
            {
                cout<<a[i];
            }
            cout<<"\n";
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }    
    MPI_Finalize();
  

}
