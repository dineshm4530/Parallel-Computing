#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<omp.h>
#include<random>

using namespace std;
int main(int argc,char** argv)
{
    int i,j,k;
    double sum=0;
    int m=atoi(argv[1]);
    int n=atoi(argv[2]);
    int p=atoi(argv[3]);
    double a[m][n];
    double b[n][p];
    double c[m][p];
    
    double lbound = 0;
    double ubound = 10;
    std::uniform_real_distribution<double> urd(lbound, ubound);
    std::default_random_engine re;
    std::uniform_real_distribution<double> urd1(lbound, ubound);
    std::default_random_engine re1;
    
    
    for(i=0;i<m;i++)
    {
        for(j=0;j<n;j++)
        {
        a[i][j]=urd(re);
        cout<<a[i][j];
        }
        cout<<"\n";
    }
    cout<<"\n";
    for(i=0;i<n;i++)
    {
        for(j=0;j<p;j++)
        {
        b[i][j]=urd1(re1);
        cout<<b[i][j];
        }
        cout<<"\n";
    }
    cout<<"\n";
    #pragma omp parallel for collapse(2) private(k)
        for(i=0;i<m;i++)
        {
            for(j=0;j<p;j++)
            {
                c[i][j]=0;
                for(k=0;k<n;k++)
                {
                    c[i][j]+=(a[i][k]*b[k][j]);
                }
            }
        }
    printf("Multiply two matrices:\n");
    for(i=0;i<m;i++)
        {
            for(j=0;j<n;j++)
            {
                cout<<c[i][j];
            }
            cout<<"\n";;
        }
}
