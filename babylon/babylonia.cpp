#include<iostream>

using namespace std;

#include "Babylon.h"

extern "C"
{
    Babylon bsock;
    uint8_t *datum;
    void startBabylon(char *server_ip,int server_port)
    {
        bsock.initBabylon(char *server_ip,(int16_t)server_port)
    }

    int pushZexport(uint8_t *data,int data_len)
    {
        bsock.sendExports((uint8_t*)data,data_len);
        //strlen(data));
    }

    uint8_t* getInport(int buffer_max_sz)
    {
        datum = new uint8_t(buffer_max_sz);
        memset(datum,0,buffer_max_sz);
        int datum_sz =  bsock.getInport(datum,buffer_max_sz);
    }

}