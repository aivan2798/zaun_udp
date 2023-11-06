#ifndef SEDAR_H_INCLUDED
#define SEDAR_H_INCLUDED



#endif // SEDAR_H_INCLUDED


#ifndef BABYLON_H_INCLUDED
#define BABYLON_H_INCLUDED



#endif // BABYLON_H_INCLUDED
#include<deque>
#include<vector>
#include<winsock2.h>
#include<future>
#include<array>
#include<map>
#include "babylon_constants.h"
#include "Babylon_Utils.h"
class Babylon
{
 //int babylon_socket=0;
 int babylon_socket_inports;// = 0;
 int babylon_socket_exports;// = 0;
 client_pod *sedar_pod = (new client_pod);
 map<uint8_t,array<uint8_t,DEFAULT_MTU>> *pdu_cache;
 int RCOUNT = 0;
 int TCOUNT = 0;
 int MAX_TCOUNT = 0;
 uint8_t batch_index=0;
 uint8_t batch_no=0;
 uint8_t last_rbatch_no;

 uint8_t *babylon_inports;//[DEFAULT_MTU];
 int babylon_inports_len;

 vector<uint8_t> babylon_exports;
 uint32_t bin_len;
 uint32_t bout_len;

 uint8_t *babylon_inportsx;
 uint8_t *babylon_exportsx;
 uint32_t bin_lenx;
 uint32_t bout_lenx;


 sockaddr_in god_dest;
 int god_dest_len=0;

 sockaddr goddest_sa;
 int goddest_sa_len;

 int inports_weight;

 vector<uint8_t> &batch_nos = sedar_pod->rbatch_nos;
 vector<uint8_t> &missed_batch_nos = sedar_pod->missed_batch_nos;

 int8_t def_batch_nos[5]={1,2,3,4,5};

 double tx_time_constant = 0;

 //client_pod sedar_pod;
 void logit(char *logtxt)
 {
     cout<<endl<<logtxt<<endl;
 }

 public:
 Babylon()
 {
    //initialise constants
    //sedar_pod = *(new client_pod);


 }

 int getInports()
 {
     sockaddr inports_city;
     int inports_city_len;

   ///  while(true)
    /// {
        inports_weight=recvfrom(babylon_socket_inports, (char *)babylon_inports,bin_len, 0, &inports_city, &inports_city_len);
        if(inports_weight>0)
        {
       //     break;
        //}
     //}

            inportsCtrl(inports_weight);
        }
     return inports_weight;
 }

 int getInportsX()
 {
     sockaddr inports_city;
     int inports_city_len;

   //  while(true)
    // {
      inports_weight=recvfrom(babylon_socket_exports, (char *)babylon_inports,bin_len, 0, &inports_city, &inports_city_len);
        if(inports_weight>0)
        {
      //      break;
       // }
     //}

            inportsCtrl(inports_weight);
        }
     return inports_weight;
 }


 int getRawInports(uint8_t *raw_inport, int raw_inport_size)
 {
     sockaddr inports_city;
     int inports_city_len;

    // while(true)
    // {
     inports_weight=recvfrom(babylon_socket_inports, (char *)raw_inport,raw_inport_size, 0, &inports_city, &inports_city_len);

       if(inports_weight>0)
       {
       //     break;
     //   }
    //  }
        inportsCtrl(inports_weight);
       }
     return inports_weight;
 }

  int getRawInportsX(uint8_t *raw_inport, int raw_inport_size)
 {
     sockaddr inports_city;
     int inports_city_len;

     //while(true)
     //{
        inports_weight=recvfrom(babylon_socket_exports, (char *)raw_inport,raw_inport_size, 0, &inports_city, &inports_city_len);

        if(inports_weight>0)
        {
       //     break;
       // }
     //}
            inportsCtrl(inports_weight);
        }

     return inports_weight;
 }

 //get data via xport channel without contriol
 int getRawInportsxX(uint8_t *raw_inport, int raw_inport_size)
 {
     sockaddr inports_city;
     int inports_city_len;

    // while(true)
    // {
        inports_weight=recvfrom(babylon_socket_exports, (char *)raw_inport,raw_inport_size, 0, &inports_city, &inports_city_len);

       // if(inports_weight>0)
      //  {
      //      break;
       // }
    // }
    // inportsCtrl(inports_weight);

     return inports_weight;
 }


 int sendExports(uint8_t *export_load, int export_raw_weight)
 {
     sockaddr inports_city;
     int inports_city_len;
     int exports_weight = 0;


    if(TCOUNT<=MAX_TCOUNT)
    {
     packExportLoad(export_load,export_raw_weight);
     bout_len = babylon_exports.size();
     exports_weight=sendto(babylon_socket_exports, (char*)babylon_exports.data(),bout_len, 0,&goddest_sa,goddest_sa_len);

     if(exports_weight<=0)
     {
         perror("No load to port");
     }
     else
     {

         pdu_cache->insert({{batch_no,array<uint8_t,DEFAULT_MTU>()}});
         array<uint8_t,DEFAULT_MTU> &sent_pdu_ref = pdu_cache->at(TCOUNT);
         copy(export_load,export_load+exports_weight,sent_pdu_ref.begin());
         TCOUNT= TCOUNT + 1;
         batch_no = batch_no + 1;

     }
    }
    else
    {
        bool reseted = resetTCOUNT();
        /*if(reseted==true)
         {
         TCOUNT= TCOUNT + 1;
         batch_no = batch_no + 1;
         }
         */
    }

     return exports_weight;
 }


 bool resetTCOUNT()
 {
     TCOUNT = 0;
     batch_no = 0;
     uint8_t inport_load[1460];
     while(true)
     {
       memset(inport_load,0,sizeof(inport_load)/4);
       sendRawExports(TRESET_HEAD,sizeof(TRESET_HEAD));


       int inported_len=getRawInportsxX(inport_load,DEFAULT_MSS);//inport_load,sizeof(inport_load));
       if(memcmp(inport_load,BACK_HEAD,sizeof(BACK_HEAD)/4)>0)
       {
            inportsCtrl(inported_len);
            break;
       }
       else
       {
           inportsCtrl(inported_len);
       }
     }
     return true;

 }

//packs a packet into a babylon pdu
 void packExportLoad(uint8_t *export_load, int export_raw_weight)
 {
  babylon_exports.push_back(DATA_CMD);
  babylon_exports.push_back(batch_index);
  babylon_exports.push_back(batch_no);

  int temp_bexp_len=babylon_exports.size();

  babylon_exports.insert(babylon_exports.end(),export_load,export_load+export_raw_weight);
  //copy(begin(export_load),end(export_load),);
 }


 //send packet without alerting TCOUNT and batch numbers
 int sendRawExports(uint8_t *raw_export,int raw_export_weight)
 {
     sockaddr inports_city;
     int inports_city_len;


     int exported_weight=sendto(babylon_socket_exports, (char*)raw_export,raw_export_weight, 0,&goddest_sa,goddest_sa_len);

     if(exported_weight<=0)
     {
         perror("No load to port");
     }


     return exported_weight;
 }

 int sendRawExportsX(uint8_t *raw_export,int raw_export_weight)
 {
     sockaddr inports_city;
     int inports_city_len;


     int exported_weight=sendto(babylon_socket_inports, (char*)raw_export,raw_export_weight, 0,&goddest_sa,goddest_sa_len);

     if(exported_weight<=0)
     {
         perror("No load to port");
     }


     return exported_weight;
 }


 //controls the command's circuitory
 void inportsCtrl(uint32_t inport_weight,uint8_t *babylon_inports_ptr,int babylon_inports_size)
 {
    babylon_inports = babylon_inports_ptr;
    inports_weight = inport_weight;
    uint8_t cmd_byte= babylon_inports[0];
    uint8_t cmd[4];
    int cmd_size=sizeof(cmd);

    switch(cmd_byte)
    {

        case CTRL_CMD:
             memcpy(cmd,&babylon_inports[1],cmd_size);

            ctrlCmd(cmd,cmd_size);
        break;

        case DATA_CMD:

            ctrlData(babylon_inports[1],babylon_inports[2],&babylon_inports[3]);
            logit("data in");
        break;

        default:

        break;
    }
 }


 //processes the commands
 void ctrlCmd(uint8_t *cmd,uint32_t cmd_len)
 {
    if(memcmp(cmd,RRESET_HEAD,cmd_len)==0)
    {
     uint8_t *payload= &babylon_inports[cmd_len];
     int payload_len= inports_weight - (1+cmd_len);
     ctrlRRESETHEAD();
    }
    else if(memcmp(cmd,RRESET_TAIL,cmd_len)==0)
    {
     ctrlRRESETTAIL();
    }
    else if(memcmp(cmd,BACK_HEAD,cmd_len)==0)
    {
     ctrlBACKHEAD();
    }
    else if(memcmp(cmd,BACK_TAIL,cmd_len)==0)
    {
     ctrlBACKTAIL();
    }
    else if(memcmp(cmd,TRESET_HEAD,cmd_len)==0)
    {
     ctrlTRESETHEAD();
    }
    else if(memcmp(cmd,TRESET_TAIL,cmd_len)==0)
    {
     ctrlTRESETTAIL();
    }
    else
    {

    }


 }

 void ctrlRRESETHEAD()//int batch_nox)
 {
    getMissingPDUs();
    int missed_batchnos_len = missed_batch_nos.size();
    vector<uint8_t> back_head_pdu;

    back_head_pdu.push_back(CTRL_CMD);
   // copy(begin(BACK_HEAD),end(BACK_HEAD),back_head_pdu.begin()+1);

    back_head_pdu.insert(back_head_pdu.end(),begin(BACK_HEAD),end(BACK_HEAD));

    int temp_backhead_size=back_head_pdu.size();
   // copy(missed_batch_nos.begin(),missed_batch_nos.end(),back_head_pdu.begin()+temp_backhead_size);
   back_head_pdu.insert(back_head_pdu.end(),missed_batch_nos.begin(),missed_batch_nos.end());

    sendRawExports(back_head_pdu.data(),back_head_pdu.size());

 }


  void ctrlRRESETTAIL()
 {
     sendMissingPDUs();
 }

  void ctrlTRESETHEAD()
 {
    getMissingPDUs();
    int missed_batchnos_len = missed_batch_nos.size();
    vector<uint8_t> back_head_pdu;

    back_head_pdu.push_back(CTRL_CMD);
   // copy(begin(BACK_HEAD),end(BACK_HEAD),back_head_pdu.begin()+1);

    back_head_pdu.insert(back_head_pdu.end(),begin(BACK_HEAD),end(BACK_HEAD));

    int temp_backhead_size=back_head_pdu.size();
   // copy(missed_batch_nos.begin(),missed_batch_nos.end(),back_head_pdu.begin()+temp_backhead_size);
    back_head_pdu.insert(back_head_pdu.end(),missed_batch_nos.begin(),missed_batch_nos.end());

    sendRawExports(back_head_pdu.data(),back_head_pdu.size());
 }

  void ctrlTRESETTAIL()
 {
    sendMissingPDUs();
 }

 void ctrlBACKHEAD()
 {
    uint8_t inport_load[DEFAULT_MSS];
   if(true)
    {
            sendRawExports(BACK_TAIL,sizeof(BACK_TAIL)/4);
           // int inported_len=getRawInportsxX(inport_load,DEFAULT_MSS);//inport_load,sizeof(inport_load));

            /*if(memcmp(inport_load,TRESET_TAIL,sizeof(TRESET_TAIL)/4)>0)
            {
                inportsCtrl(inported_len);
                break;
            }
            */
                inportsCtrl(inported_len);
    }

 }

  void ctrlBACKTAIL()
 {
    vector<uint8_t> rreset_tail_pdu;

    rreset_tail_pdu.push_back(CTRL_CMD);
   // copy(begin(BACK_HEAD),end(BACK_HEAD),back_head_pdu.begin()+1);

    rreset_tail_pdu.insert(rreset_tail_pdu.end(),begin(BACK_HEAD),end(BACK_HEAD));

    int temp_backhead_size=rreset_tail_pdu.size();
   // copy(missed_batch_nos.begin(),missed_batch_nos.end(),back_head_pdu.begin()+temp_backhead_size);
   //rreset_tail_pdu.insert(rreset_tail_pdu.end(),missed_batch_nos.begin(),missed_batch_nos.end());

    sendRawExports(rreset_tail_pdu.data(),rreset_tail_pdu.size());
    RCOUNT=0;

 }


 void getMissingPDUs()
 {
     uint8_t *serie = batch_nos.data();
     int ss=batch_nos.size();
     int ds=sizeof(def_batch_nos)/4;
    for(int x=0;x<ss;x++)
    {
    uint8_t snum = serie[x];
    for(int xl=0;xl<ds;xl++)
    {
     if(snum==def_batch_nos[xl])
     {
        def_batch_nos[xl]=0;
     }
    }
    }

    for(int xp=0;xp<ds;xp++)
    {
        uint8_t dm=def_batch_nos[xp];
        if(dm!=0)
        {
            missed_batch_nos.push_back(dm);
    //cout<<endl<<"missing: "<<dm<<endl;
        }
    }
 }

 void sendMissingPDUs()
 {
    for(int pdu_indx=0;pdu_indx<payload_len;pdu_indx++)
    {
        uint8_t pind = babylon_inports[pdu_indx+5];
        sendExports(pdu_cache[pind].data(),pdu_cache[pind].size());
    }
 }




 void ctrlData(uint8_t batch_index, uint8_t batch_no, uint8_t *payload)
 {
  batch_nos.push_back(batch_no);
  RCOUNT=RCOUNT+1;
 }
};




class Sedar
{
    client_pod sedar_pod;
public:
    Sedar()
    {
     sedar_pod = *(new client_pod);
    }
};


