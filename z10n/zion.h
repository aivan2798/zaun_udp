#include<iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "z10n.h"
//merging ctrl resets
//uint8_t *buffer = new uint8_t(DEFAULT_MTU+1);
uint8_t buffer[DEFAULT_MTU];
extern "C"
{
  ZionServer zs;
  
//  int dc = 0;
  int defclient = 0;
   
  //cout<<endl<<"using client: "<< dc<<endl;
 //cout.flush();
  void start_zion()
  {
    zs.initServer(8055);
  //  buffer = new uint8_t(DEFAULT_MTU+1);
   // dc=zs.waitDef();
    defclient = inet_addr("192.168.43.122");
  }

  int zlisten()
  {
    int dc=zs.waitDef();
    cout<<"\nusing dc: "<<dc<<endl;
    cout.flush();
    return dc;
  }

  int pushZexport(int dc,uint8_t *data,int data_len)
  {
    return zs.zexport(dc,(uint8_t*)data,data_len);
  }

  uint8_t* getInport(int dc,int buffer_max_sz)
  {
   // memset(buffer,0,sizeof(buffer));
   // uint8_t *buffer = new uint8_t(buffer_max_sz);
   // memset(buffer,0,DEFAULT_MTU);
    
    cout<<endl<<"\nCREATING BUFFER...."<<dc<<"with size: "<<sizeof(buffer)<<endl<<"def size: "<<DEFAULT_MTU<<endl<<endl;
    cout.flush();
   
    //char *dtmxx = "
    zs.zinport(dc,buffer,buffer_max_sz);
    //";
   // memcpy(buffer,dtmxx,strlen(dtmxx));

    cout<<endl<<"memory used: "<<(int)buffer<<endl;
    cout.flush();
    
    return buffer;
  }

  void killmem(uint8_t *kill_pt)
  {
      //delete(kill_pt);
      cout<<endl<<"memory killed: "<<(int)kill_pt<<endl;
      cout.flush();
  }

}