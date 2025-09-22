/* 

 Write a program to simulate Memory placement strategies-best fit, first fit,
next fit and worst fit. 
*/

#include<bits/stdc++.h>
using namespace std;

class Memory
{
    public:
    int n;
    vector<int>arr;
    int lpos;

    Memory(int x)
    {
        n=x; // the size of the memory
        arr.assign(x,0);
        lpos = -1;
    }

    void makearr(vector<int>& a)
    {
        for(int i=0 ;i<n; i++)
        {
            arr[i]=a[i];
        }
    }

    void first_fit(int mem)
    {
        for(int i=0; i<n; i++)
        {
            if(arr[i] >= mem)
            {
                arr[i]=-1;
                lpos =i;
                cout<<"Memory Allocated successfully using First_fit "<<mem<<"GB"<<"Index: "<<i<<"\n";
                
                return;
            }
        }

        cout<<"WE didint find the requred memeory:";
    }

    void best_fit(int mem)
    {
        //find the smallest requyired for mem size
        int idx= -1;
        for(int i=0; i<n; i++)
        {
            if(arr[i]>=mem)
            {
                if( idx !=-1)
                {
                    if(arr[idx] > arr[i])
                    {
                        idx = i;
                    }
                }
                else
                {
                    idx = i;
                }
            }
        }

        if(idx==-1)
        {
            cout<<"We couldnt find the required memory:";
            return ;
        }

        arr[idx]=-1;
        lpos= idx;
        cout<<"Memory is allocated using the best "<<mem<<" GB "<<"index : "<<idx<<"\n";
    }

    void next_fit(int mem)
    {
        int idx =-1;
        for(int i= lpos+1; i<n; i++)
        {
            if(arr[i]>= mem)
            {
                arr[i]=-1;
             
                cout<<"Memory is allocated using the next_fit "<<mem<<" GB "<<"Index :"<<idx<<"\n";
                return;
            }

        }

        cout<<"We couldnt find the required memory: "<<"\n";
        return;
    }

    void worst_fit(int mem)
    {
        //find the largest required for mem size
        int idx= -1;
        for(int i=0; i<n; i++)
        {
            if(arr[i]>=mem)
            {
                if( idx !=-1)
                {
                    if(arr[idx] < arr[i])
                    {
                        idx = i;
                    }
                }
                else
                {
                    idx = i;
                }
            }
        }

        if(idx==-1)
        {
            cout<<"We couldnt find the required memory:";
            return ;
        }

        arr[idx]=-1;
        lpos= idx;
        cout<<"Memory is allocated using the worst fit: "<<mem<<" GB "<<"index : "<<idx<<"\n";
    }

};
int main()
{

    int n;
    cout<<"Enter the NO of the partitions of the Memory: ";
    cin>>n;

    Memory mem(n);
    cout<<"Enter the n partition sizes:";
    vector<int>arr(n);
    for(int i=0; i<n; i++)
    {
        int tp;
        cin>>tp;
        arr[i]=tp;
        mem.makearr(arr);
    }


    int m;
    int tp;

    while(1)
    {
        cout<<"Enter the memory required:";
        cin>>m;

        cout<<"Enter the Method to use:";
        cout<<"1:first_fit   2:best_fit   3:worst_fit  4:next_fit  5: exit";
        cin>>tp;
        switch(tp)
        {
            case 1:
                mem.first_fit(m);
                break;
            case 2:
                mem.best_fit(m);
                break;
            case 3:
                mem.worst_fit(m);
                break;
            case 4:
                mem.next_fit(m);
                break;
            case 5:
                exit(1);
        }

    }


    return 0;
    
}