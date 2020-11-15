#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<omp.h>
#include<random>

using namespace std;
int main(int argc,char** argv)
{
    int i;
    int length=atoi(argv[1]);
    int a[length];
    int sum=0;
    
    double lbound = 0;
    double ubound = 10;
    std::uniform_real_distribution<double> urd(lbound, ubound);
    std::default_random_engine re;
    
    for(i=0;i<length;i++)
    {
        a[i]=urd(re);
        cout<<a[i];
    }
    cout<<"\n";
    #pragma omp parallel for reduction(+: sum)
        for(i=0;i<length;i++)
        {
            sum+=a[i];
        }
    cout<<"Total sum: "<<sum<<endl;    

}
