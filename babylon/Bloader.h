#ifndef BLOADER_H_INCLUDED
#define BLOADER_H_INCLUDED



#endif // BLOADER_H_INCLUDED
#include<iostream>

using namespace std;
//starting project babylon
#include "Babylon.h"

extern "C"
{
    Babylon babylon_obj;

    void initBabylon(char *zion_coord,uint16_t zion_hole)
    {
        babylon_obj = Babylon(zion_coord,zion_hole);
    }

    int sendData(uint8_t *datum_bank, int datum_len)
    {
        return babylon_obj.sendExports(datum,datum_len);
    }

    int recvData(uint8_t *datum_bank, int datum_len)
    {
        return babylon_obj.getInport(datum,datum_len);
    }
}
