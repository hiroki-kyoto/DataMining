#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#define USER_N 1
#define USER_TOTAL 182881
#define BRAND_N 300
#define MONTH_N 4
#define TYPE_N 4
#define NUL "0"
#define THRESHOLD 0.5

/** some supposement:
* 4/15-5/15 Named as Month 1
* 5/15-6/15 Named as Month 2
* 6/15-7/15 Named as Month 3
* 7/15-8/15 Named as Month 4
**/

using namespace std;

/** function statment begin **/

string MapMonth(int m, int d);

int FindInArray(string arr[],int length,string str);

int ReadCSV(const char* csv, string user[USER_N],
                  string brand[USER_N][MONTH_N][BRAND_N],
                  int type[USER_N][MONTH_N][BRAND_N][TYPE_N],
                  int& un,int bn[USER_N][MONTH_N]);

int WritePrediction(const char* filename, string user[USER_N], 
                          string prediction[USER_N][BRAND_N]);

int PredictBrands(string prediction[BRAND_N],string brand[MONTH_N][BRAND_N],
                         int type[MONTH_N][BRAND_N][TYPE_N]);

/** function statment end **/

int main(int argc, char *argv[])
{
    string user[USER_N];
    string brand[USER_N][MONTH_N][BRAND_N];
    int    type[USER_N][MONTH_N][BRAND_N][TYPE_N];
    
    static int un;
    static int bn[USER_N][MONTH_N];
    
    string prediction[USER_N][BRAND_N];
    
    /** initial variables **/
    {
        int i=0,ii=0,iii=0,iv=0;
        for(;i<USER_N;i++)
        {
            user[i]=NUL;
            for(;ii<MONTH_N;ii++)
            {
                for(;iii<BRAND_N;iii++)
                {
                    brand[i][ii][iii]=NUL;
                    for(;iv<TYPE_N;iv++)
                    {
                        type[i][ii][iii][iv]=0;
                    }   
                }
            }
        }
    }
    
    /** read the csv data set **/
    ReadCSV("s1.csv",user,brand,type,un,bn);
    
    /** apply ANN to predict brands **/
    PredictBrands(prediction[0],brand[0],type[0]);
     
    /** output prediction entries into file **/
    WritePrediction("p1.txt",user,prediction);
    
    return EXIT_SUCCESS;
}


/** implementation of stated functions **/

string MapMonth(int m, int d)
{
    if(m==4)
    {
        return "1";
    }
    if(m==8)
    {
        return "4";
    }
    int base=m%4;
    if(d<15)
    {
        stringstream ss;
        ss<<base;
        return ss.str();
    } else {
        stringstream ss;
        ss<<base+1;
        return ss.str();
    }
}

int FindInArray(string arr[],int length,string str)
{
    int find=-1;
    int i=length;
    while(i-->0)
    {
        if(str.equals(arr[i]))
        {
            find=i;
            break;
        }
    }
    return find; 
}

string Trim(string& str)
{
    str.erase(0,str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}


int ReadCSV(const char* csv, string user[USER_N],
                  string brand[USER_N][MONTH_N][BRAND_N],
                  int type[USER_N][MONTH_N][BRAND_N][TYPE_N],
                  int& un,int bn[USER_N][MONTH_N])
{
    int success=1;
    
    ifstream fin(csv);
	
	string line;	
	while (getline(fin, line)) {
		
		istringstream sin(line);	
		vector<string> fields;	
		string field;
		while (getline(sin, field, ',')) {
			fields.push_back(field);	
		}

		string uid = Trim(fields[0]);	
		string bid = Trim(fields[1]);	
		string tid = Trim(fields[2]);	
		string mid = Trim(fields[3]);
		
		// get maped month number		
		{
            int i=mid.find_first_of("月");
            int m=atoi(mid.substr(0,i));
            int d=atoi(mid.substr(i,length(mid)-1));
            mid=MapMonth(m,d);
        }
		
		// to kill time, maybe...
		cout<<uid<<"\t"<<bid<<"\t"<<tid<<"\t"<<mid<<endl; 
		
		// pre-process data
		int m=atoi(mid)-1;
		int t=atoi(tid);
		int index=FindInArray(user,un,uid);
		
		if(index>-1)
        {
            int bindex=FindInArray(brand[index][m],bn[index][m],bid);
            if(bindex>-1)
            {
                type[index][m][bindex][t]+=1;
            } else {
                bn[index][m]+=1;
                bindex=bn[index][m]-1;
                brand[index][m][bindex]=bid;
                type[index][m][bindex][t]+=1;
            }
            
            
        } else {
            un++;
            index=un-1;
            user[index]=uid;
            bn[index][m]=1;
            brand[index][m][0]=bid;
            type[index][m][0][t]+=1;
        }
		
	}
    
    return success;
}

int WritePrediction(const char* filename, string user[USER_N], 
                          string prediction[USER_N][BRAND_N])
{
    int success=1;
    
    return success;
}

int PredictBrands(string prediction[BRAND_N],string brand[MONTH_N][BRAND_N],
                         int type[MONTH_N][BRAND_N][TYPE_N])
{
    int success=1;
    
    return success;
}                        


