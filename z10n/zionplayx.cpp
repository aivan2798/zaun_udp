#include "zion.h"

using namespace std;

int main()
{
    start_zion();
    int zind = zlisten();

    cout<<"\nclient connected: "<<zind<<endl;
    cout.flush();

    while(true)
    {
        char data[256];
        memset(data,0,256);
        cin>>data;

        pushZexport(zind,data,strlen(data));
    }
}