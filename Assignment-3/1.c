#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    int rank,np;
    int length= atoi(argv[1]);
    double a[length];
    
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    
    if(rank==0)
    {
        double lbound = 0;
        double ubound = 10;
        std::uniform_real_distribution<double> urd(lbound, ubound);
        std::default_random_engine re;
        
        for (int i=0;i<length;i++)
        {
             a[i] = urd(re);
             cout<<a[i];
        } 
        cout<<"\n";
    }


    if(rank==0)   //master process
    {
        int i,pb;
        int length1=length/np;
        if(np>1)
        {
            for(i=1;i<np-1;i++)
            {
                pb=i*length1;
                MPI_Send(&length1,1,MPI_INT,i,0,MPI_COMM_WORLD);
                MPI_Send(&a[pb],length1,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
            }
            pb=i*length1;
            int left = length-pb;
            MPI_Send(&left,1,MPI_INT,i,0,MPI_COMM_WORLD);
            MPI_Send(&a[pb],left,MPI_DOUBLE,i,0,MPI_COMM_WORLD);
        }
        double sum=0;
        for(i=0;i<length1;i++)
        {
            sum+=a[i];
        }
        double part_sum;
        for(i=1;i<np;i++)
        {
            MPI_Recv(&part_sum,1,MPI_DOUBLE,MPI_ANY_SOURCE,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
            sum+=part_sum;
        }
        cout<<"Total sum: "<<sum<<endl;
    }
    else             // slave process
    {
        int length1,i;
        MPI_Recv(&length1,1,MPI_INT,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        double a1[length1];
        MPI_Recv(&a1,length1,MPI_DOUBLE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        double part_sum=0;
        for(i=0;i<length1;i++)
        {
            part_sum+=a1[i];
        }
        MPI_Send(&part_sum,1,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
    }
    MPI_Finalize();
    
}
