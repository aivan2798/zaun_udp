#ifndef BABYLON_H_INCLUDED
#define BABYLON_H_INCLUDED



#endif // BABYLON_H_INCLUDED
#include<deque>
#include<vector>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<future>
#include<array>
#include<map>
#include<string.h>
#include "babylon_constants.h"
#include "Babylon_Utils.h"

//check out rreset

void doTimeup(double timeup,bool &killa,mutex &killa_lock,int sendfd,uint8_t *dtm, int dtm_size,sockaddr sad, int sad_len)
{
    chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
    double max_time = 0;
    while(true)
    {
        chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();

        chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);

        double elasped_time=c_time.count();
        killa_lock.lock();
        bool killme =  killa;
        killa_lock.unlock();

        if(killme==true)
        {
            cout<<"\nbroken wings: "<<elasped_time<<endl<<endl;
            cout.flush();
            max_time = elasped_time;
            break;
        }
        if(elasped_time==timeup)
        {
            sendto(sendfd,(char*)dtm,dtm_size,0,&sad,sad_len);
        }
    }
}





//deque<array<uint8_t,DEFAULT_MTU>> inports;
deque<vector<uint8_t>> inports;
 mutex inports_lock;

  map<uint8_t,map<uint8_t,vector<uint8_t>>> inports_container;
 mutex inports_container_lock;

uint8_t batch_men = 0;
mutex batch_men_lock;

class Babylon
{
 //int babylon_socket=0;
 uint8_t my_zone = 0;
 mutex my_zone_lock;

 int babylon_socket_inports;// = 0;
 mutex bsi_lock;
 int babylon_socket_exports;// = 0;
 mutex bse_lock;
 map<uint8_t,array<uint8_t,DEFAULT_MTU>> pdu_cache;
 map<uint8_t,array<uint8_t,DEFAULT_MTU>> xpdu_cache;
 int RCOUNT = 1;
 int TCOUNT = 1;
 int MAX_TCOUNT = 5;
 uint8_t batch_index=1;
 uint8_t batch_no=1;
 uint8_t last_rbatch_no;

 uint8_t babylon_inports[DEFAULT_MTU];
 int back_head_data_len;
 uint8_t back_head_data[DEFAULT_MTU];

 vector<uint8_t> babylon_exports;
 uint32_t bin_len=DEFAULT_MTU;
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

 vector<uint8_t> batch_nos;
 vector<uint8_t> missed_batch_nos;

 int8_t def_batch_nos[5]={1,2,3,4,5};

 double tx_time_constant = 0;

 bool tcountReseted = false;

 /*
 deque<array<uint8_t,DEFAULT_MTU>> inports;
 mutex inports_lock;
 */




 future<void> inports_thread;

 int timeup = 5 * 1000;
// mutex tcountReseted;
 void logit(char *logtxt)
 {
     cout<<endl<<logtxt<<endl;
     cout.flush();
 }

 public:
 void setMyZone()
 {
  batch_men_lock.lock();
  my_zone = batch_men+1;
  batch_men = my_zone;
  batch_men_lock.unlock();
  cout<<endl<<"using context : "<<(0+my_zone)<<endl;
  cout.flush();
 }

 void initBabylon(const char *ip_addr, uint16_t dest_port)//, uint8_t *in_buffer, uint32_t inbuffer_len, uint8_t *out_buffer, uint32_t outbuffer_len)
 {
    //initialise constants

    setMyZone();
    memset(&god_dest,0,sizeof(god_dest));
    god_dest.sin_family=AF_INET;
    god_dest.sin_addr.s_addr=inet_addr(ip_addr);
    god_dest.sin_port=htons(dest_port);
    god_dest_len=sizeof(god_dest);

    sockaddr *goddest_sa_ptr=(sockaddr*)&god_dest;
    goddest_sa = *goddest_sa_ptr;
    goddest_sa_len=sizeof(goddest_sa);

//    pdu_cache = new map<uint8_t,array<uint8_t,DEFAULT_MTU>>;

    //babylon_inports=in_buffer;
    //bin_len=inbuffer_len;
    memset(babylon_inports,0,DEFAULT_MTU);
//    babylon_exports=out_buffer;
   // bout_len=inbuffer_len;
    //memset(babylon_exports,0,bout_len);
//    WSADATA data;
  //  WSAStartup(MAKEWORD(1,1),&data);
    babylon_socket_inports=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    babylon_socket_exports=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    cout<<endl<<"socket fd: "<<babylon_socket_exports<<endl;
    if(babylon_socket_exports>0)
    {
        logit("The gate of the gods has been open");
        cout<<"\tattempting connection to: "<<ip_addr<<" : "<<dest_port<<endl;
        cout.flush();

        initCRTunnel();
        initCTTunnel();

        inports_thread = async(launch::async,&Babylon::babylonInportsMan,this);
    }
    else
    {
        perror("King Nebu has halted the gates");
    }
 }


 bool initCRTunnel()
 {
   uint8_t raw_datum[5];
   memset(raw_datum,0,(sizeof(raw_datum)));
   cout<<"\t raw datum size: "<<sizeof(raw_datum)<<endl;
   cout.flush();
   bool killman = false;
   mutex killman_lock;

   future<double> timerd_fx;
   future<void> timer_fx;
  // timer_fx = async(launch::async,&Babylon::sendRawExportsXThrd,this,(double)5000,ref(killman),ref(killman_lock),CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD));
    setsockopt(babylon_socket_inports,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeup,sizeof(timeup));
   while(true)
   {
       logit("sent blouse");
       int xport_weight = sendRawExportsX(CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD));
        logit("blousew okay");       //future_already_retrieved



       timerd_fx = async(launch::async,timerd,ref(killman_lock),ref(killman));
       ///doTimeup,,&(this->sendRawExportsX),CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD));
       //killman_lock.lock();

       int inport_size = getRawInports(raw_datum,sizeof(raw_datum));

       //killman_lock.unlock();
       cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;

       if(inport_size>0)
       {
           if(memcmp(raw_datum,INIT_CMD_TAIL,sizeof(INIT_CMD_TAIL))==0)
           {
                killman = true;
                logit("killing timer");
         //       tx_time_constant = timerd_fx.get();
                break;
           }
       }
       else
       {
           perror("\ndamn it");
       }

       killman = true;
       tx_time_constant = timerd_fx.get();

     //   break;
   }

    /*
   while(true)
   {
        sendRawExportsX(CR_INIT_CMD_FIN,sizeof(CR_INIT_CMD_FIN));
        int inport_size = getRawInports(raw_datum,sizeof(raw_datum));
        cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;
        cout.flush();

        if(memcmp(raw_datum,CR_INIT_CMD_FIN_OK,sizeof(CR_INIT_CMD_FIN_OK))==0)
           {

                logit("init finished");
         //       tx_time_constant = timerd_fx.get();
                break;
           }


   }
   */
   return true;

 }


 bool initCTTunnel()
 {
   uint8_t raw_datum[5];
   memset(raw_datum,0,sizeof(raw_datum));

   bool killman = false;
   mutex killman_lock;

   while(true)
   {
       int xport_weight = sendRawExports(CT_INIT_CMD_HEAD,sizeof(CT_INIT_CMD_HEAD));
       //future_already_retrieved


       future<double> timerd_fx = async(launch::async,timerd,ref(killman_lock),ref(killman));
       int inport_size = getRawInportsX(raw_datum,sizeof(raw_datum));
       cout<<endl<<"raw inports size: "<<inport_size<<" with data: "<<(char*)raw_datum<<endl;
       cout.flush();
       if(inport_size>0)
       {
           if(memcmp(raw_datum,INIT_CMD_TAIL,sizeof(INIT_CMD_TAIL))==0)
           {
                killman = true;
                tx_time_constant = timerd_fx.get();
                break;
           }
       }

       killman = true;
       tx_time_constant = timerd_fx.get();


   }

///   sendRawExportsX(CT_INIT_CMD_FIN,sizeof(CT_INIT_CMD_FIN));
   return true;

 }


 int getInports()
 {
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);;

   ///  while(true)
    /// {
        ///bsi_lock.lock();
        inports_weight=recvfrom(babylon_socket_inports, (char *)babylon_inports,bin_len, 0, &inports_city,(socklen_t*) &inports_city_len);
        ///bsi_lock.unlock();
        if(inports_weight>0)
        {
       //     break;
        //}
     //}

            inportsCtrl(inports_weight);
        }
     return inports_weight;
 }

  int readInport(uint8_t *inports_buff,int inports_buff_len)
  {

  }

  int getInports(uint8_t *inports_buff,int inports_buff_len)
 {
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);;

   ///  while(true)
    /// {
        inports_weight=recvfrom(babylon_socket_inports, (char *)inports_buff,inports_buff_len, 0, &inports_city, (socklen_t*)&inports_city_len);
        if(inports_weight>0)
        {
       //     break;
        //}
     //}

            inportsCtrl(inports_weight);
        }
     return inports_weight;
 }

/*main internal getInport
    int getInport(uint8_t *inports_buff,int inports_buff_len)
    {
        inports_lock.lock();
        int in_size = inports.size();
        inports_lock.unlock();
        int inported_ref_size = -1;
        while(true)
        {
        if(in_size>0)
        {
            array<uint8_t,DEFAULT_MTU> &inported_ref = inports.back();
            //int
            inported_ref_size = inported_ref.size();

            if(inported_ref_size>inports_buff_len)
            {
                //return -1;
                logit("error size");
            }
            copy(inported_ref.begin(),inported_ref.end(),inports_buff);
            logit("data just coipied");
            inports_lock.lock();
            inports.pop_back();
            inports_lock.unlock();
            break;
        }
        else
        {
          //  return -1;
        }
        return inported_ref_size;
        }
    }
    */

 int getInportsX()
 {
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);

   //  while(true)
    // {
      inports_weight=recvfrom(babylon_socket_exports, (char *)babylon_inports,bin_len, 0, &inports_city, (socklen_t*)&inports_city_len);
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
     int inports_city_len = sizeof(inports_city);

    // while(true)
    // {
     setsockopt(babylon_socket_inports,SOL_SOCKET,SO_RCVTIMEO,(char*)&timeup,sizeof(timeup));
     ///bsi_lock.lock();
     inports_weight=recvfrom(babylon_socket_inports, (char *)raw_inport,raw_inport_size, 0, &inports_city, (socklen_t*)&inports_city_len);
     ///bsi_lock.unlock();
       if(inports_weight>0)
       {
       //     break;
     //   }
    //  }
        cout<<endl<<"received data: "<<(char *)raw_inport<<endl;
        cout.flush();
     //   inportsCtrl(inports_weight);
       }
       else
       {
           perror("\tdamnit");
       }
     return inports_weight;
 }

  int getRawInportsX(uint8_t *raw_inport, int raw_inport_size)
 {
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);

     //while(true)
     //{
        inports_weight=recvfrom(babylon_socket_exports, (char *)raw_inport,raw_inport_size, 0, &inports_city, (socklen_t*)&inports_city_len);

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
     int inports_city_len = sizeof(inports_city);

    // while(true)
    // {
        inports_weight=recvfrom(babylon_socket_exports, (char *)raw_inport,raw_inport_size, 0, &inports_city, (socklen_t*)&inports_city_len);

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
     int inports_city_len  = sizeof(inports_city);;
     int exports_weight = 0;



    if(TCOUNT<=MAX_TCOUNT)
    {

     if(batch_no==3)
     {


         /*array<uint8_t,DEFAULT_MTU> sent_pdu_ref ;//= pdu_cache.at(TCOUNT);
         copy(export_load,export_load+exports_weight,sent_pdu_ref.begin());
         cout<<endl<<"sending: "<<(char*)sent_pdu_ref.data()<<endl;
         pdu_cache.insert({{batch_no,sent_pdu_ref}});//array<uint8_t,DEFAULT_MTU>()}});*/

         pdu_cache.insert({{batch_no,array<uint8_t,DEFAULT_MTU>()}});
         array<uint8_t,DEFAULT_MTU> &sent_pdu_ref = pdu_cache.at(batch_no);
         sent_pdu_ref.fill((uint8_t)0);
         copy(export_load,export_load+export_raw_weight,sent_pdu_ref.begin());

         cout<<endl<<"sending: "<<(char*)sent_pdu_ref.data()<<" with size: "<<export_raw_weight<<endl;
         cout.flush();
         cout<<endl<<TCOUNT<<" packet ghosted"<<endl;
         cout.flush();
         TCOUNT= TCOUNT + 1;
         batch_no = batch_no + 1;
     }
     else
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


         pdu_cache.insert({{batch_no,array<uint8_t,DEFAULT_MTU>()}});
         array<uint8_t,DEFAULT_MTU> &sent_pdu_ref = pdu_cache.at(batch_no);
         copy(export_load,export_load+exports_weight,sent_pdu_ref.begin());
         TCOUNT= TCOUNT + 1;
         batch_no = batch_no + 1;

     }
     }


    }
    else
    {

        /*if(reseted==true)
         {
         TCOUNT= TCOUNT + 1;
         batch_no = batch_no + 1;
         }
         */
    }

    if(TCOUNT>MAX_TCOUNT)
    {
         bool reseted = resetTCOUNT();
    }

     return exports_weight;
 }



 int packCMDLoad(uint8_t *cmd_type,int cmd_type_len,vector<uint8_t> &cmd)
 {
    cout<<endl<<"packing cmd of len: "<<cmd_type_len<<" and val: "<<(char*)cmd_type<<endl;
    cout.flush();

    cmd.clear();
    cmd.push_back(CTRL_CMD);
    cmd.insert(cmd.end(),cmd_type,cmd_type+(cmd_type_len));
    int temp_bexp_len=cmd.size();
    return temp_bexp_len;
 }



 bool resetTCOUNT()
 {
     TCOUNT = 1;
     batch_no = 1;
     uint8_t inport_load[DEFAULT_MTU];
     babylon_socket_exports = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
     while(true)
     {
       memset(inport_load,0,sizeof(inport_load));
       vector<uint8_t> cmd_pdu;
       //cmd_pdu.reserve(DEFAULT_MTU);
       int cmd_pdu_len = packCMDLoad(TRESET_HEAD,sizeof(TRESET_HEAD),cmd_pdu);
       sendRawExports(cmd_pdu.data(),cmd_pdu_len);
       cout<<endl<<"RESETING TCOUNT\n\n"<<endl;
       cout.flush();
       int inported_len=getRawInportsxX(inport_load,DEFAULT_MTU);//inport_load,sizeof(inport_load));
       cout<<endl<<"reset data: "<<(char*)inport_load<<" with len: "<<inported_len<<endl;
       cout.flush();
       memcpy(babylon_inports,inport_load,DEFAULT_MTU);
       inports_weight = inported_len;
       if(memcmp(&inport_load[1],BACK_HEAD,sizeof(BACK_HEAD))==0)
       {
           logit("\tback head in");
            inportsCtrl(inported_len);
            break;
       }
       else
       {
           logit("\t\tcrazy data");
           inportsCtrl(inported_len);
       }
     }


     return true;

 }

//packs a packet into a babylon pdu
 void packExportLoad(uint8_t *export_load, int export_raw_weight)
 {

  babylon_exports.clear();
  babylon_exports.push_back(my_zone);
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
     logit("@sending raw cmd");
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);
     int raw_exports_weight = raw_export_weight+2;
     uint8_t raw_exports[raw_exports_weight];

     memset(raw_exports,0,raw_exports_weight);

     raw_exports[0]=my_zone;

     logit("mmemcpier");
     memcpy(&raw_exports[1],raw_export,raw_export_weight);
    /// bsi_lock.lock();


     int exported_weight=sendto(babylon_socket_exports, (char*)raw_exports,raw_exports_weight, 0,&goddest_sa,goddest_sa_len);

     if(exported_weight<=0)
     {
         perror("No load to port");
     }


     return exported_weight;
 }



 int sendRawExportsX(uint8_t *raw_export,int raw_export_weight)
 {
     cout<<endl<<"sending raw socket X"<<endl;
     cout.flush();
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);
     int raw_exports_weight = raw_export_weight+2;
     uint8_t raw_exports[raw_exports_weight];

     memset(raw_exports,0,raw_exports_weight);

     raw_exports[0]=my_zone;
     memcpy(&raw_exports[1],raw_export,raw_export_weight);
     ///bsi_lock.lock();
     int exported_weight=sendto(babylon_socket_inports, (char*)raw_exports,raw_exports_weight, 0,&goddest_sa,goddest_sa_len);
     ///bsi_lock.unlock();
     if(exported_weight<=0)
     {
         perror("No load to port");
     }


     return exported_weight;
 }


 void sendRawExportsXThrd(double timeup,bool &killa,mutex &killa_lock,uint8_t *raw_export,int raw_export_weight)
 {
    bsi_lock.lock();
    int exported_weight=sendto(babylon_socket_inports, (char*)raw_export,raw_export_weight, 0,&goddest_sa,goddest_sa_len);
    bsi_lock.unlock();

    chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
    double max_time = 0;
    while(true)
    {
        chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();

        chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);

        double elasped_time=c_time.count();
        killa_lock.lock();
        bool killme =  killa;
        killa_lock.unlock();

        if(killme==true)
        {
            cout<<"\nbroken send fx wings: "<<elasped_time<<endl<<endl;
            cout.flush();
            max_time = elasped_time;
            break;
        }
        if(elasped_time==timeup)
        {
            bsi_lock.lock();
            int exported_weight=sendto(babylon_socket_inports, (char*)raw_export,raw_export_weight, 0,&goddest_sa,goddest_sa_len);
            bsi_lock.unlock();
            cout<<endl<<"repeating fx"<<endl;
            cout.flush();

            init_timepoint= chrono::steady_clock::now();
        }
    }
 }




 //controls the command's circuitory
 void inportsCtrl(uint32_t inport_weight)
 {
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
            logit("data in");

            ctrlData(babylon_inports[1],babylon_inports[2],&babylon_inports[3],inport_weight-(3));
            logit("data parsed");

        break;

        default:
            logit("oops");
        break;
    }
 }


 //processes the commands
 void ctrlCmd(uint8_t *cmd,int cmd_len)
 {
    if(memcmp(cmd,RRESET_HEAD,cmd_len)==0)
    {
     uint8_t *payload= &babylon_inports[cmd_len+1];
     int payload_len= inports_weight - (1+cmd_len);
     ctrlRRESETHEAD();
    }
    else if(memcmp(cmd,RRESET_TAIL,cmd_len)==0)
    {
     ctrlRRESETTAIL();
    }
    else if(memcmp(cmd,BACK_HEAD,cmd_len)==0)
    {
     uint8_t *payload= babylon_inports+ (cmd_len+1);
     int payload_len= inports_weight - (cmd_len+1);
     memcpy(back_head_data,payload,payload_len);
     back_head_data_len = payload_len;
     cout<<endl<<"back head payload len: "<<payload_len<<endl;
     cout.flush();
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
        logit("\tsozzi, no command\n");
    }


 }

 void ctrlRRESETHEAD()//int batch_nox)
 {
    getMissingPDUs();
    babylon_socket_inports = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    ///setMyZone();
    logit("Trying to reset");
    int missed_batchnos_len = missed_batch_nos.size();
    vector<uint8_t> back_head_pdu;

    back_head_pdu.push_back(CTRL_CMD);
   // copy(begin(BACK_HEAD),end(BACK_HEAD),back_head_pdu.begin()+1);

    back_head_pdu.insert(back_head_pdu.end(),BACK_HEAD,BACK_HEAD+sizeof(BACK_HEAD));

    int temp_backhead_size=back_head_pdu.size();
   // copy(missed_batch_nos.begin(),missed_batch_nos.end(),back_head_pdu.begin()+temp_backhead_size);
   back_head_pdu.insert(back_head_pdu.end(),missed_batch_nos.begin(),missed_batch_nos.end());
    ///uint8_t raw_datum[DEFAULT_MTU];
    while(true)
    {
       // memset(raw_datum,0,DEFAULT_MTU);
        sendRawExportsX(back_head_pdu.data(),back_head_pdu.size());
        logit("sent reset");
        /*
        int inport_size = getRawInports(raw_datum,sizeof(raw_datum));

       //killman_lock.unlock();
        cout<<endl<<"dsize: "<<inport_size<<" raw init data: "<<(char*)raw_datum<<endl;
        cout.flush();

        if(inport_size>0)
        {
           if(memcmp(&raw_datum[1],BACK_TAIL,sizeof(BACK_TAIL))==0)
           {
                logit("back tail here");
                ctrlCmd(&raw_datum[1],(inport_size-1));
                break;
           }
       }
       else
       {
           perror("\ndamn it");
       }
       */
       break;
    }
    batch_nos.clear();
    missed_batch_nos.clear();

 }

  public:
   void babylonInportsMan();//int bfd,Babylon &babylon);

  void ctrlRRESETTAIL()
 {

 }

  void ctrlTRESETHEAD()
 {

 }

  void ctrlTRESETTAIL()
 {
    cout<<endl<<"\t\tattemping Trest"<<endl;
    cout.flush();
    sendMissingPDUs();
 }

 void findUnsentPDUs()
 {

 }

 void ctrlBACKHEAD()
 {
    uint8_t inport_load[DEFAULT_MSS];
    while(true)
    {
            vector<uint8_t> cmd_pdu;
            packCMDLoad(BACK_TAIL,sizeof(BACK_TAIL),cmd_pdu);
            sendRawExports(cmd_pdu.data(),cmd_pdu.size());
            int inported_len=getRawInportsxX(inport_load,DEFAULT_MSS);//inport_load,sizeof(inport_load));
            memcpy(babylon_inports,inport_load,DEFAULT_MTU);
            if(memcmp(&inport_load[1],TRESET_TAIL,sizeof(TRESET_TAIL))==0)
            {
                logit("\ttreset tail");

                inportsCtrl(inported_len);
                break;
            }
            else
            {
                logit("\tgabage tail");
                inportsCtrl(inported_len);
                break;
            }
    }

 }

  void ctrlBACKTAIL()
 {
    vector<uint8_t> rreset_tail_pdu;
    logit("received back tail");
    rreset_tail_pdu.push_back(CTRL_CMD);
   // copy(begin(BACK_HEAD),end(BACK_HEAD),back_head_pdu.begin()+1);

    rreset_tail_pdu.insert(rreset_tail_pdu.end(),begin(RRESET_TAIL),end(RRESET_TAIL));

    int temp_backhead_size=rreset_tail_pdu.size();
   // copy(missed_batch_nos.begin(),missed_batch_nos.end(),back_head_pdu.begin()+temp_backhead_size);
   //rreset_tail_pdu.insert(rreset_tail_pdu.end(),missed_batch_nos.begin(),missed_batch_nos.end());

    ///sendRawExportsX(rreset_tail_pdu.data(),rreset_tail_pdu.size());
    RCOUNT=0;

 }


 void getMissingPDUs()
 {
     uint8_t *serie = batch_nos.data();
     int ss=batch_nos.size();
     int ds=sizeof(def_batch_nos);
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
    xpdu_cache = pdu_cache;
    pdu_cache.clear();
    for(int pdu_indx=0;pdu_indx<(back_head_data_len);pdu_indx++)
    {
        uint8_t pind = back_head_data[pdu_indx];
        array<uint8_t,DEFAULT_MTU> &dtum = xpdu_cache[pind];
        cout<<endl<<"\tsending pdu: "<<0+pind<<" datm: "<<(char*)dtum.data()<<endl;
        cout.flush();
        sendExports(dtum.data(),dtum.size());
    }
    memset(back_head_data,0,sizeof(back_head_data));
    back_head_data_len = 0;
    xpdu_cache.clear();
    //pdu_cache.clear();
 }




 void ctrlData(uint8_t batch_index, uint8_t batch_no, uint8_t *payload,int payload_len)
 {
  batch_nos.push_back(batch_no);
  cout<<endl<<"using batch no: "<<0+batch_no<<" with payload len: "<<payload_len<<endl;
  //cout<<"\tpayload: "<<(char*)payload<<endl;
  RCOUNT=RCOUNT+1;
  //memset(babylon_inports,0,bin_len);

  inports_lock.lock();
  ///inports.push_front(array<uint8_t,DEFAULT_MTU>());
  inports.push_front(vector<uint8_t>(payload,payload+payload_len));
  ///array<uint8_t,DEFAULT_MTU> &inport_pdu_ref = inports.front();
  vector<uint8_t> &inport_pdu_ref = inports.front();
  //vector<uint8_t> &inport_pdu_ref = inports.front();
  //copy(payload,payload+(payload_len),inport_pdu_ref.begin());

  cout<<"\tpayload: "<<(char*)inport_pdu_ref.data()<<endl;
  inports_lock.unlock();

    /*
  inports_container_lock.lock();
  inports_container[batch_index][batch_no]=vector<uint8_t>(payload,payload+payload_len);
  inports_container_lock.unlock();
  */


  cout<<endl<<"\t\tusing new batch indexes"<<endl<<endl;
  cout.flush();

   memset(babylon_inports,0,bin_len);
 }


};



void Babylon::babylonInportsMan()//int bfd,Babylon &babylon)
{
     sockaddr inports_city;
     int inports_city_len = sizeof(inports_city);;

   while(true)
     {
        cout<<endl<<"waiting for data"<<endl;
        cout.flush();
        ///bsi_lock.lock();
        inports_weight=recvfrom(babylon_socket_inports, (char *)babylon_inports,bin_len, 0, &inports_city, (socklen_t*)&inports_city_len);
        ///bsi_lock.unlock();
        cout<<endl<<"data of len: "<<inports_weight<<endl;
        cout.flush();
       // logit("no data");
        if(inports_weight>0)
        {
       //     break;
        //}
     //}

            inportsCtrl(inports_weight);
            cout<<endl<<"inport parsed"<<endl;
            cout.flush();
        }
      }

}

int getInports(uint8_t *inports_buff,int inports_buff_len)
{
        /*
        inports_lock.lock();
        int in_size = inports.size();
        inports_lock.unlock();
        */
        int in_size = 0;
        int inported_ref_size = -1;
        while(true)
        {
         //cout<<endl<<"do sum th: "<<in_size<<endl;
         //cout.flush();
         inports_lock.lock();
         in_size = inports.size();
         inports_lock.unlock();
        if(in_size>0)
        {
            cout<<endl<<"do sum"<<endl;
            cout.flush();
            ///array<uint8_t,DEFAULT_MTU>
            vector<uint8_t> &inported_ref = inports.back();
            //int
            inported_ref_size = inported_ref.size();

            if(inported_ref_size>inports_buff_len)
            {
                //return -1;
            cout<<endl<<"error size";
            }
            copy(inported_ref.begin(),inported_ref.end(),inports_buff);
            cout<<endl<<endl<<"data just coipied"<<endl;
            inports_lock.lock();
            inports.pop_back();
            inports_lock.unlock();
            break;
        }
        else
        {
          //  return -1;
        }

        }
        return inported_ref_size;
}


int XgetInports(uint8_t *inports_buff,int inports_buff_len)
{
        /*
        inports_lock.lock();
        int in_size = inports.size();
        inports_lock.unlock();
        */
        int use_batch_index = 0;
        int use_batch_no=0;
        int in_size = 0;
        int inported_ref_size = -1;
        while(true)
        {
         //cout<<endl<<"do sum th: "<<in_size<<endl;
         //cout.flush();
         inports_container_lock.lock();
         in_size = inports_container.size();
         inports_container_lock.unlock();
        if(in_size>0)
        {
            inports_container_lock.lock();
            map<uint8_t,map<uint8_t,vector<uint8_t>>>::iterator it_inner_data_map = inports_container.begin();
            //[use_batch_index];
            map<uint8_t,vector<uint8_t>> &inner_data_map = it_inner_data_map->second;
            int inner_insize = inner_data_map.size();
            inports_container_lock.unlock();

            if(inner_insize>0)
            {
                map<uint8_t,vector<uint8_t>>::iterator it_inported_ref = inner_data_map.begin();
                vector<uint8_t> &inported_ref = it_inported_ref->second;
                //[use_batch_no];
                inported_ref_size = inported_ref.size();

                if(inported_ref_size>inports_buff_len)
                {
                //return -1;
                    cout<<endl<<"error size";
                }

                copy(inported_ref.begin(),inported_ref.end(),inports_buff);
                cout<<endl<<endl<<"open data just coipied: "<<inner_insize<<" with data:"<<(char*)inported_ref.data()<<endl;
             inner_data_map.erase(it_inported_ref);
            }
            else
            {
             inports_container.erase(it_inner_data_map);
            }




            break;
        }
        else
        {
          //  return -1;
        }

        }
        return inported_ref_size;
}



void startBabylonia()
{
    Babylon bsock;
    bsock.initBabylon("192.168.1.151",8055);

}

void startAll(int nub)
{
    startBabylonia();
    startBabylonia();
}



