#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono;

int i=0,ct=0;
pthread_mutex_t lock; 


void iittp_barrier(int rank,int np,int tid,int num_threads)
{
    if(rank==0 && tid==0)
    {
        MPI_Send(&i,1,MPI_INT,1,1,MPI_COMM_WORLD);
        MPI_Recv(&i,1,MPI_INT,np-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    }
    else if(tid==0)
    {
        MPI_Recv(&i,1,MPI_INT,rank-1,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
        MPI_Send(&i,1,MPI_INT,(rank+1)%np,1,MPI_COMM_WORLD);
    }
    if(tid==0)
    {
    MPI_Bcast(&i,1,MPI_INT,0,MPI_COMM_WORLD);
    }
    pthread_mutex_lock(&lock);
    ct++;
    pthread_mutex_unlock(&lock);
    
    while((ct%num_threads)!=0);
}


void thread_inc(int tid,int &x,int rank,int np,int num_threads)
{

    printf("%d:%d:%d\n",rank,tid,x);
    iittp_barrier(rank,np,tid,num_threads);
}

int main(int argc, char **argv)
{
    int rank,np,i;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    int Num_threads=2;
    int x=rank;
    std::vector<std::thread> threads;
    printf("rank %d:%d\n",rank,x);
    for(i=0;i<Num_threads;i++)
    {
        threads.push_back(std::thread(thread_inc,i,std::ref(x),rank,np,Num_threads));
    }
    for(auto &th:threads)
    {
        th.join();
    }
 
    printf("rank %d:%d\n",rank,x);
    MPI_Finalize();
}
