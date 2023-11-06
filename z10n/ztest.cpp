#include<iostream>
#include<future>
#include<pthread.h>

using namespace std;




//
extern "C"
{

    future<void> fx_man;

    static void hello()
    {
    int x = 0;
    
        while(true)
        {
            cout<<endl<<"Hello seattle: "<<x<<endl;
            cout.flush();
            if(x==10)
            {
                
                break;
            }
            x = x + 1;
        }

     
    }

    void start_fx()
    {
        fx_man = async(launch::async,hello);

       // fx_man.get();
        cout<<endl<<"seattle finished"<<endl;
        cout.flush();
    }

    
}



int main()
{

 //   hello();
}