#include <iostream>
#include <stdio.h>
#include <random>
#include <chrono>
#include <omp.h>

using namespace std;
using namespace std::chrono;
int main(int argc, char **argv)
{
    int x,y;
    int t1,iter=0,nthds;
    int n = atoi(argv[1]);
    bool flag=true;
    float t = 0.01;
    float d,t2;
    
    float* a0=(float*) malloc(n*n*sizeof(float));
    float* a1=(float*) malloc(n*n*sizeof(float));
    
    float lbound = 0;
    float ubound = 100;
    std::uniform_real_distribution<float> urd(lbound, ubound);
    std::default_random_engine re;
 
    for(x=1;x<n;x++)
    {
        for(y=0;y<n;y++)
        {
            a0[x*n+y]=0;
            a1[x*n+y]=0;
        }
    }
    for(x=1;x<=n;x++)
    {
        for(y=1;y<=n;y++)
        {
            a0[x*n+y] = urd(re);
        }
    }

    if(n<10)
        nthds=1;
    else if(n<100)
        nthds=3;
    else
        nthds=5;
    auto start = high_resolution_clock::now();
    omp_set_num_threads(nthds);
     cout<<"number of threads launched: "<<nthds<<endl;
    do
    {
        flag=false;
        {
        #pragma omp parallel for collapse(2)
        for(x=1;x<=n;x++)
        {
            for(y=1;y<=n;y++)
            {
                t1=x*n;
                d=5.0;
                t2 = a0[t1+y]+ a0[t1+y+1]+ a0[t1+y-1] + a0[t1+n+y] + a0[t1-n+y];
                t2=t2/d;
               if(t<=fabs(t2-a0[t1+y]))
                {
                    flag=true;
                }
                a1[t1+y]=t2;
            }
        }
        #pragma omp parallel for collapse(2)
        for(x=1;x<=n;x++)
        {
            for(y=1;y<=n;y++)
            {
                a0[x*n+y]=a1[x*n+y];
            }    
        }
        }
    iter++;
    }
    while(flag);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    cout <<"Time:"<< duration.count() <<endl;
    cout<<n<<endl;
    cout<<"number of iterations: "<<iter<<endl;
}
