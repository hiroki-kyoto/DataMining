#include<iostream>
#include<cmath>
using namespace std;

#define  innode 3 
#define  hidenode 10
#define  outnode 1 
#define  trainsample 8
class BpNet
{
public:
    void train(double p[trainsample][innode ],double t[trainsample][outnode]);
    double p[trainsample][innode];    
    double t[trainsample][outnode];    

    double *recognize(double *p);

    void writetrain(); 
    void readtrain(); 

    BpNet();
    virtual ~BpNet();

public:
    void init();
    double w[innode][hidenode];
    double w1[hidenode][outnode];
    double b1[hidenode];
    double b2[outnode];

    double rate_w; 
    double rate_w1;
    double rate_b1;
    double rate_b2;

    double e;
    double error;
    double result[outnode];
};

BpNet::BpNet()
{
    error=1.0;
    e=0.0;

    rate_w=0.9;  
    rate_w1=0.9; 
    rate_b1=0.9; 
    rate_b2=0.9;
}

BpNet::~BpNet()
{

}

void winit(double w[],int n) 
{
  for(int i=0;i<n;i++)
    w[i]=(2.0*(double)rand()/RAND_MAX)-1;
}

void BpNet::init()
{
    winit((double*)w,innode*hidenode);
    winit((double*)w1,hidenode*outnode);
    winit(b1,hidenode);
    winit(b2,outnode);
}

void BpNet::train(double p[trainsample][innode],double t[trainsample][outnode])
{
    double pp[hidenode];
    double qq[outnode];
    double yd[outnode];

    double x[innode]; 
    double x1[hidenode];
    double x2[outnode];
    double o1[hidenode];
    double o2[hidenode];

    for(int isamp=0;isamp<trainsample;isamp++)
    {
        for(int i=0;i<innode;i++)
            x[i]=p[isamp][i];
        for(int i=0;i<outnode;i++)
            yd[i]=t[isamp][i];

        for(int j=0;j<hidenode;j++)
        {
            o1[j]=0.0;
            for(int i=0;i<innode;i++)
                o1[j]=o1[j]+w[i][j]*x[i];
            x1[j]=1.0/(1+exp(-o1[j]-b1[j]));
        }

        for(int k=0;k<outnode;k++)
        {
            o2[k]=0.0;
            for(int j=0;j<hidenode;j++)
                o2[k]=o2[k]+w1[j][k]*x1[j]; 
            x2[k]=1.0/(1.0+exp(-o2[k]-b2[k])); 
        
        }

        for(int k=0;k<outnode;k++)
        {
            qq[k]=(yd[k]-x2[k])*x2[k]*(1-x2[k]); 
            for(int j=0;j<hidenode;j++)
                w1[j][k]+=rate_w1*qq[k]*x1[j]; 
        }

        for(int j=0;j<hidenode;j++)
        {
            pp[j]=0.0;
            for(int k=0;k<outnode;k++)
                pp[j]=pp[j]+qq[k]*w1[j][k];
            pp[j]=pp[j]*x1[j]*(1-x1[j]);

            for(int i=0;i<innode;i++)
                w[i][j]+=rate_w*pp[j]*x[i];
        }

        for(int k=0;k<outnode;k++)
        {
            e+=fabs(yd[k]-x2[k])*fabs(yd[k]-x2[k]);
        }
        error=e/2.0;

        for(int k=0;k<outnode;k++)
            b2[k]=b2[k]+rate_b2*qq[k]; 
        for(int j=0;j<hidenode;j++)
            b1[j]=b1[j]+rate_b1*pp[j];
    }
}

double *BpNet::recognize(double *p)
{
    double x[innode]; 
    double x1[hidenode]; 
    double x2[outnode]; 
    double o1[hidenode];
    double o2[hidenode]; 

    for(int i=0;i<innode;i++)
        x[i]=p[i];

    for(int j=0;j<hidenode;j++)
    {
        o1[j]=0.0;
        for(int i=0;i<innode;i++)
            o1[j]=o1[j]+w[i][j]*x[i];
        x1[j]=1.0/(1.0+exp(-o1[j]-b1[j])); 
      
    }

    for(int k=0;k<outnode;k++)
    {
        o2[k]=0.0;
        for(int j=0;j<hidenode;j++)
            o2[k]=o2[k]+w1[j][k]*x1[j];
        x2[k]=1.0/(1.0+exp(-o2[k]-b2[k]));
      
    }

    for(int k=0;k<outnode;k++)
    {
        result[k]=x2[k];
    }
    return result;
}

void BpNet::writetrain()
{
    FILE *stream0;
    FILE *stream1;
    FILE *stream2;
    FILE *stream3;
    int i,j;
   
    if(( stream0 = fopen("w.txt", "w+" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    for(i=0;i<innode;i++)
    {
        for(j=0;j<hidenode;j++)
        {
            fprintf(stream0, "%f\n", w[i][j]);
        }
    }
    fclose(stream0);

    if(( stream1 = fopen("w1.txt", "w+" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    for(i=0;i<hidenode;i++)
    {
        for(j=0;j<outnode;j++)
        {
            fprintf(stream1, "%f\n",w1[i][j]);
        }
    }
    fclose(stream1);

    if(( stream2 = fopen("b1.txt", "w+" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    for(i=0;i<hidenode;i++)
        fprintf(stream2, "%f\n",b1[i]);
    fclose(stream2);
	
    if(( stream3 = fopen("b2.txt", "w+" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    for(i=0;i<outnode;i++)
        fprintf(stream3, "%f\n",b2[i]);
    fclose(stream3);

}

void BpNet::readtrain()
{
    FILE *stream0;
    FILE *stream1;
    FILE *stream2;
    FILE *stream3;
    int i,j;

    if(( stream0 = fopen("w.txt", "r" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    float  wx[innode][hidenode];
    for(i=0;i<innode;i++)
    {
        for(j=0;j<hidenode;j++)
        {
            fscanf(stream0, "%f", &wx[i][j]);
            w[i][j]=wx[i][j];
        }
    }
    fclose(stream0);

    if(( stream1 = fopen("w1.txt", "r" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    float  wx1[hidenode][outnode];
    for(i=0;i<hidenode;i++)
    {
        for(j=0;j<outnode;j++)
        {
            fscanf(stream1, "%f", &wx1[i][j]);
            w1[i][j]=wx1[i][j];
        }
    }
    fclose(stream1);

    if(( stream2 = fopen("b1.txt", "r" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    float xb1[hidenode];
    for(i=0;i<hidenode;i++)
    {
        fscanf(stream2, "%f",&xb1[i]);
        b1[i]=xb1[i];
    }
    fclose(stream2);

    if(( stream3 = fopen("b2.txt", "r" ))==NULL)
    {
        cout<<"create file failure!\n";
        exit(1);
    }
    float xb2[outnode];
    for(i=0;i<outnode;i++)
    {
        fscanf(stream3, "%f",&xb2[i]);
        b2[i]=xb2[i];
    }
    fclose(stream3);
}

double X[trainsample][innode]= {
    {0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}
    };
	
double Y[trainsample][outnode]={
    {0},{0.1429},{0.2857},{0.4286},{0.5714},{0.7143},{0.8571},{1.0000}
    };

int main()
{
    BpNet bp;
    bp.init();
    int times=0;
    while(bp.error>0.0001)
    {
        bp.e=0.0;
        times++;
        bp.train(X,Y);
        cout<<"Times="<<times<<" error="<<bp.error<<endl;
    }
    cout<<"trainning complete..."<<endl;
    double m[innode]={1,1,1};
    double *r=bp.recognize(m);
    for(int i=0;i<outnode;++i)
       cout<<bp.result[i]<<" ";
    double cha[trainsample][outnode];
    double mi=100;
    double index;
    for(int i=0;i<trainsample;i++)
    {
        for(int j=0;j<outnode;j++)
        {
            
            cha[i][j]=(double)(fabs(Y[i][j]-bp.result[j]));
            if(cha[i][j]<mi)
            {
                mi=cha[i][j];
                index=i;
            }
        }
    }
    for(int i=0;i<innode;++i)
       cout<<m[i];
    cout<<" is "<<index<<endl;
    cout<<endl;
    return 0;
}
