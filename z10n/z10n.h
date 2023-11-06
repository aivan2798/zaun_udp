#ifndef Z10N_H_INCLUDED
#define Z10N_H_INCLUDED



#endif // Z10N_H_INCLUDED
#include<sys/socket.h>
#include<arpa/inet.h>
#include<map>
#include<mutex>
 #include<shared_mutex>
#include "babylon_constants.h"







static double ctInitTimer(int zion_portalfd,client_pod &active_pod)//,sockaddr active_addr)
{
    chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
    double max_time = 0;
 cout<<endl<<"\t\tSTARTED TIMERD\n";
    while(true)
    {
        active_pod.ct_kill_timer_lock.lock();
        bool kill = active_pod.ct_kill_timer;
        active_pod.ct_kill_timer_lock.unlock();

        chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();
        chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);
        double elasped_time=c_time.count();

        if(kill == true)
        {
            active_pod.ct_timeval = elasped_time;
            max_time = elasped_time;
            break;
        }


    }
    return max_time;
}


static double crInitTimer(int zion_portalfd,client_pod &active_pod)//,sockaddr active_addr)
{
    chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
    double max_time = 0;
 cout<<endl<<"\t\tSTARTED TIMERD\n";
 cout.flush();
    while(true)
    {
        active_pod.cr_kill_timer_lock.lock();
        bool kill = active_pod.cr_kill_timer;
        active_pod.cr_kill_timer_lock.unlock();

        chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();
        chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);
        double elasped_time=c_time.count();

        if(kill == true)
        {
            active_pod.cr_timeval = elasped_time;
            max_time = elasped_time;
            break;
        }


    }

    cout<<endl<<"\t\t cr killed\n"<<endl;
    cout.flush();
    return max_time;
}




static void inporter(int zsock,map<int,map<uint8_t,client_pod>> &client_pod_map,mutex &clientpod_map_lock,bool &is_on,mutex &ison_lock)
    {
        ison_lock.lock();

        is_on = true;

        ison_lock.unlock();

        uint8_t inport[DEFAULT_MTU];
        cout<<"\nInporter has started\n"<<endl;
        cout.flush();
        while(true)
        {
        memset(inport,0,DEFAULT_MTU);
        sockaddr tzion_addr;
        int tzion_addr_len = sizeof(tzion_addr);
        memset(&tzion_addr,0,tzion_addr_len);

        //global_sock_lock.lock();
        int inport_size = recvfrom(zsock,(char*)inport,DEFAULT_MTU,0,&tzion_addr,(socklen_t*)&tzion_addr_len);
        //global_sock_lock.unlock();

        if(inport_size>0)
        {
        cout<<endl<<("\t data in: ")<<inport_size<<endl;
        cout.flush();
        sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
        int ipaddr =  sinx->sin_addr.s_addr;

        uint8_t this_batch_ctx = inport[0];

        cout<<endl<<"using batch number: "<<0+this_batch_ctx<<endl;
        cout.flush();

        if(memcmp(&inport[1],CR_INIT_CMD_HEAD,sizeof(CR_INIT_CMD_HEAD))==0)
        {
           // sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
           // int ipaddr =  sinx->sin_addr.s_addr;
            clientwrkers_lock.lock();
            client_wrkers[ipaddr].push_front(this_batch_ctx);
            clientwrkers_lock.unlock();

            cout<<"\ninitialising client: "<<inet_ntoa(sinx->sin_addr)<<" with port: "<<ntohs(sinx->sin_port)<<endl;
            cout.flush();
            map<uint8_t,client_pod> &client_pod_s = client_pod_map[ipaddr];
            //map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = client_pod_map.find(ipaddr);
            map<uint8_t,client_pod>::iterator cpm_iterator = client_pod_s.find(this_batch_ctx);
            if(cpm_iterator==client_pod_s.end())
            {
              client_pod_s[this_batch_ctx].initZion(zsock,ipaddr,this_batch_ctx);
            }

            clientpod_map_lock.lock();
            //client_pod_map[ipaddr].export_addr=tzion_addr;
            //client_pod_map[ipaddr].export_addr_len=tzion_addr_len;

            
            client_pod &t_cp = client_pod_s[this_batch_ctx];
            ///client_pod &t_cp = (cpm_iterator->second)[this_batch_ctx];
            t_cp.export_addr=tzion_addr;
            t_cp.export_addr_len=tzion_addr_len;

            clientpod_map_lock.unlock();

            t_cp.logDura("\tusing  batch no: ",(0+this_batch_ctx));

            /*
            deque<array<uint8_t,DEFAULT_MTU>> &ref_inports = t_cp.inports;
            array<uint8_t,DEFAULT_MTU> temp_data;
            copy(begin(CT_INIT_CMD_HEAD),end(CT_INIT_CMD_HEAD),temp_data.begin());
            ref_inports.push_front(temp_data);
            */
            //global_sock_lock.lock();
            sendto(zsock,(char*)INIT_CMD_TAIL,sizeof(INIT_CMD_TAIL),0,&tzion_addr,tzion_addr_len);
            /*
            pbi_xlock.lock();
            {
              ///unique_lock<shared_mutex> lock(pbi_lock);
              present_batch_indexes.push_front(this_batch_ctx);
            }
            pbi_xlock.unlock();
            */
            //global_sock_lock.unlock();

          //  t_cp.cr_timer_thread = async(launch::async,crInitTimer,zsock,ref(t_cp));
        cout<<endl<<"finished setting up"<<endl;
        t_cp.toggle_crt();
        }
        else if(memcmp(&inport[1],CT_INIT_CMD_HEAD,sizeof(CT_INIT_CMD_HEAD))==0)
        {
    //        sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
  //          int ipaddr =  sinx->sin_addr.s_addr;
            ///map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = client_pod_map.find(ipaddr);
            map<uint8_t,client_pod> &client_pod_s = client_pod_map[ipaddr];
            map<uint8_t,client_pod>::iterator cpm_iterator = client_pod_s.find(this_batch_ctx);
            cout<<"\ninitialising client tx: "<<inet_ntoa(sinx->sin_addr)<<" with port: "<<ntohs(sinx->sin_port)<<endl;
            cout.flush();
            if(cpm_iterator==client_pod_s.end())
            {
              client_pod_s[this_batch_ctx].initZion(zsock,ipaddr,this_batch_ctx);
            }

            clientpod_map_lock.lock();
            client_pod &t_cp = client_pod_s[this_batch_ctx];
           /// client_pod &t_cp = (cpm_iterator->second)[this_batch_ctx];
            t_cp.inport_addr=tzion_addr;
            t_cp.inport_addr_len=tzion_addr_len;
            clientpod_map_lock.unlock();

            /*
            deque<array<uint8_t,DEFAULT_MTU>> &ref_inports = t_cp.inports;
            array<uint8_t,DEFAULT_MTU> temp_data;
            copy(begin(CT_INIT_CMD_HEAD),end(CT_INIT_CMD_HEAD),temp_data.begin());
            ref_inports.push_front(temp_data);
            */
           //global_sock_lock.lock();
            sendto(zsock,(char*)INIT_CMD_TAIL,sizeof(INIT_CMD_TAIL),0,&tzion_addr,tzion_addr_len);
           //global_sock_lock.unlock();

          //  t_cp.ct_timer_thread = async(launch::async,ctInitTimer,zsock,ref(t_cp));
          
          t_cp.toggle_ctt();
          cout<<endl<<"finished initialising client\n";
          cout.flush();
        }
        else if(memcmp(&inport[1],CR_INIT_CMD_FIN,sizeof(CR_INIT_CMD_FIN))==0)
        {
       //  int ipaddr = getIpInt(tzion_addr);
       cout<<"\t init almoist fin"<<endl;
       cout.flush();
         client_pod &active_pod = client_pod_map[ipaddr][this_batch_ctx];
         active_pod.cr_kill_timer_lock.lock();
         active_pod.cr_kill_timer = true;
         active_pod.cr_kill_timer_lock.unlock();

         active_pod.RX_TIME = active_pod.cr_timer_thread.get();

         cout<<endl<<"using rx time: "<<active_pod.RX_TIME<<endl;
         cout.flush();

         

         //global_sock_lock.lock();
         sendto(zsock,(char*)CR_INIT_CMD_FIN_OK,sizeof(CR_INIT_CMD_FIN_OK),0,&tzion_addr,tzion_addr_len);
         //global_sock_lock.unlock();
         active_pod.toggle_crt();
        }
        else if(memcmp(&inport[1],CT_INIT_CMD_FIN,sizeof(CT_INIT_CMD_FIN))==0)
        {
       //  int ipaddr = getIpInt(tzion_addr);
         cout<<"\t ct init almoist fin"<<endl;
         cout.flush();
         client_pod &active_pod = client_pod_map[ipaddr][this_batch_ctx];
         active_pod.ct_kill_timer_lock.lock();
         active_pod.ct_kill_timer = true;
         active_pod.ct_kill_timer_lock.unlock();

         active_pod.tx_timeout = active_pod.ct_timer_thread.get();
         active_pod.ct_init_ok = true;

         cout<<endl<<"using tx time: "<<active_pod.tx_timeout<<endl;
         cout.flush();
         active_pod.toggle_ctt();

        }
        else if(memcmp(&inport[1],PREP_HEAD,sizeof(PREP_HEAD))==0)
        {
           cout<<"\nprep head at: "<<inet_ntoa(sinx->sin_addr)<<" with port: "<<ntohs(sinx->sin_port)<<endl;
            cout.flush();
          client_pod &active_pod = client_pod_map[ipaddr][this_batch_ctx];
          active_pod.logPrep(tzion_addr);
          
        }
        else
        {
          cout<<"\ndata from client: "<<inet_ntoa(sinx->sin_addr)<<" with port: "<<ntohs(sinx->sin_port)<<endl;
            cout.flush();
          client_pod &active_pod = client_pod_map[ipaddr][this_batch_ctx];
          active_pod.inportsCtrl(&inport[1],(inport_size-2),tzion_addr,tzion_addr_len);
        }
        /*
        else if(memcmp(inport,BACK_HEAD,sizeof(BACK_HEAD))==0)
        {
            client_pod &active_pod = client_pod_map[ipaddr];
            active_pod.TCOUNT = 0;
            active_pod.batch_no = 0;
        }
        else if(memcmp(inport,BACK_TAIL,sizeof(BACK_TAIL))==0)
        {

        }
        else if(memcmp(inport,TRESET_HEAD,sizeof(TRESET_HEAD))==0)
        {
            client_pod &active_pod = client_pod_map[ipaddr];
            active_pod.RCOUNT=0;


        }
        else if(memcmp(inport,RRESET_TAIL,sizeof(RRESET_TAIL))==0)
        {

        }
        else
        {
            sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
            int ipaddr =  sinx->sin_addr.s_addr;

            clientpod_map_lock.lock();
            client_pod &t_cp = client_pod_map[ipaddr];
            //
            //t_cp.inport_addr=tzion_addr;
            //t_cp.inport_addr_len=tzion_addr_len;
            //
            clientpod_map_lock.unlock();

            if(t_cp.ct_init_ok==false)
            {
                t_cp.ct_kill_timer_lock.lock();
                t_cp.ct_kill_timer = true;
                t_cp.ct_kill_timer_lock.unlock();
                t_cp.ct_timer_thread.get();
                t_cp.ct_init_ok = true;
            }
            deque<array<uint8_t,DEFAULT_MTU>> &ref_inports = t_cp.inports;
            uint8_t batch_index = inport[1];
            uint8_t batch_no = inport[2];

            t_cp.rbatch_nos.push_back(batch_no);
            int rc = t_cp.RCOUNT;
            t_cp.RCOUNT = rc +1;
            array<uint8_t,DEFAULT_MTU> temp_data;
            copy(inport,inport+DEFAULT_MTU,temp_data.begin());
            ref_inports.push_front(temp_data);


        }
        */
        }
    }
   }



    static void exporter(int zion_socketfd,deque<export_pod> &exportpods_ref,mutex &exportpods_ref_lock)
    {
            //int ipaddr =  sinx->sin_addr.s_addr;

            exportpods_ref_lock.lock();
            export_pod &ref_export_pod = exportpods_ref.back();
            sockaddr extzion_addr = ref_export_pod.export_addr;
            int extzion_addr_len = ref_export_pod.export_addr_len;
            exportpods_ref_lock.unlock();

            array<uint8_t,DEFAULT_MTU> &ref_export = ref_export_pod.exports;
            int export_len = ref_export_pod.exports_size;

            //global_sock_lock.lock();
            sendto(zion_socketfd,(char*)ref_export.data(),export_len,0,&extzion_addr,extzion_addr_len);
            //global_sock_lock.unlock();
            
            exportpods_ref_lock.lock();
            exportpods_ref.pop_back();
            exportpods_ref_lock.unlock();
    }



class ZionServer
{
    int zion_socket;

    uint8_t active_tx_batch = 0;
    uint8_t active_rx_batch = 0;

    sockaddr_in zion_sin;
    int zion_sin_len;
    sockaddr zion_addr;
    int zion_addr_len;
    int &zsock = zion_socket;
    map<int,map<uint8_t,client_pod>> client_pod_map;
    mutex clientpod_map_lock;
    int client_pod_map_szs;

    future<void> inporter_thread;
    bool inp_is_on = false;
    mutex inp_is_on_lock;
    future<void> exporter_thread;

    map<int,map<uint8_t,client_pod>>::iterator xcpm_iterator;// = client_pod_map.begin();
    void logit(char *logtxt)
    {
        cout<<endl<<logtxt<<endl;
    }




    public:
        void initServer(uint16_t port_no)
        {
            zion_sin_len=sizeof(zion_sin);
            zion_addr_len=sizeof(zion_addr);
            memset(&zion_sin,0,zion_sin_len);
            memset(&zion_addr,0,zion_addr_len);

            zion_sin.sin_addr.s_addr =  INADDR_ANY;
            zion_sin.sin_family =  AF_INET;
            zion_sin.sin_port = htons(port_no);

            client_pod_map_szs = 0;

            cout<<endl<<"initialising server: "<<endl;
            //WSADATA data;
            //WSAStartup(MAKEWORD(1,1),&data);
            zion_socket = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
            xcpm_iterator = client_pod_map.begin();
            if(zion_socket<=0)
            {
                perror("the gods have halted your excess");

                return;
            }
            logit("\tThe gods welcome you here.");

            int bind_value = bind(zion_socket,(sockaddr*)&zion_sin,zion_sin_len);

            if(bind_value<0)
            {
                perror("sorry man, binding has failed");
                return;
            }

            logit("\tthe gods open their portal");

            inporter_thread = async(launch::async,inporter,zion_socket, ref(client_pod_map),ref(clientpod_map_lock),ref(inp_is_on),ref(inp_is_on_lock));
           // exporter_thread = async(launch::async,exporter,zion_socket, ref(client_pod_map),ref(clientpod_map_lock));
           logit("\nthread init");
            while(true)
            {
              inp_is_on_lock.lock();
              bool lock_status = inp_is_on;
              inp_is_on_lock.unlock();

              if(lock_status == true)
              {
                logit("\n in_on is true");
                break;
              }
            }
        }

        void zgetInport(int client_id,uint8_t *inport_data,int inport_data_len)
        {
            map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = client_pod_map.find(client_id);


            if(cpm_iterator==client_pod_map.end())
            {
              cout<<endl<<"no memories of client: "<<client_id<<" with addr: "<<inet_ntoa(getIpAddr_int(client_id))<<endl;
              cout.flush();
            }
            else
            {
              client_pod &clientpod = (cpm_iterator->second)[active_rx_batch];//client_pod_map[client_id];
              sockaddr xport_addr = clientpod.export_addr;
              int xport_addr_len = clientpod.export_addr_len;

              cout<<"\tin found client: "<<inet_ntoa(getIpAddr(xport_addr))<<" with port: "<<ntohs(getPortNo(xport_addr))<<endl;
              cout.flush();

              //clientpod.sendExports(export_data,export_data_len);

            }
        }

        int zexport(int client_id,uint8_t *export_data,int export_data_len)
        {

            map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = client_pod_map.find(client_id);
            int sent_data = 0;
            uint8_t ex_batch_indx = 0;//last_batch_ind;
            if(cpm_iterator==client_pod_map.end())
            {
              cout<<endl<<"no memories of client: "<<client_id<<" with addr: "<<inet_ntoa(getIpAddr_int(client_id))<<endl;
              cout.flush();
            }
            else
            {

              cout<<endl<<":preparing out: "<<(0+ex_batch_indx)<<endl;
              cout.flush();

              /*
              pbi_xlock.lock();
              {
               //   shared_lock<shared_mutex> lock(pbi_lock);
                  int pbi_sz = present_batch_indexes.size();

                  if(pbi_sz>0)
                  {
                    ex_batch_indx = present_batch_indexes.back();
                  }
                  else
                  {
                    ex_batch_indx = last_batch_ind;
                  }

              }
              pbi_xlock.unlock();
              */
              while(true)
              {
              ///clientwrkers_lock.lock();
              int cw_sz = client_wrkers[client_id].size();
              if(cw_sz>0)
              {
                ex_batch_indx = client_wrkers[client_id].back();
              

              client_pod &clientpod = (cpm_iterator->second)[ex_batch_indx];
              ///client_pod_map[client_id];

              clientpod.export_addr_lock.lock();
              sockaddr xport_addr = clientpod.export_addr;
              int xport_addr_len = clientpod.export_addr_len;
              clientpod.export_addr_lock.unlock();

              cout<<"\tex found client: "<<inet_ntoa(getIpAddr(xport_addr))<<" with port: "<<ntohs(getPortNo(xport_addr))<<" and batch index: "<<(0+ex_batch_indx)<<endl;
              cout.flush();
            

              sent_data = clientpod.sendExports(export_data,export_data_len);
              cout<<"\n\t\tsent clientpod data: "<<sent_data<<endl;
              cout.flush();
              if (sent_data<=0)
              {
                continue;
              }
              else
              {
                  break;
              }
              }
              ///clientwrkers_lock.unlock();

            }
            }
            return sent_data;
        }

        void zinport(int client_id,uint8_t *inport_data,int inport_data_len)
        {
            map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = client_pod_map.find(client_id);


            if(cpm_iterator==client_pod_map.end())
            {
              cout<<endl<<"no xmemories of client: "<<client_id<<" with addr: "<<inet_ntoa(getIpAddr_int(client_id))<<endl;
              cout.flush();
            }
            else
            {
              client_pod &clientpod = client_pod_map[client_id][active_rx_batch];
              sockaddr xport_addr = clientpod.export_addr;
              int xport_addr_len = clientpod.export_addr_len;

              cout<<"\t xfound client: "<<inet_ntoa(getIpAddr(xport_addr))<<" xwith port: "<<ntohs(getPortNo(xport_addr))<<endl;
              cout.flush();

              while(true)
              {
                int in_sz = clientpod.getInport(inport_data,inport_data_len);
                if(in_sz>0)
                {
                  break;
                }
              }
            }
        }

        int waitDef()
        {
          logit("\nwait def started");
          int defman;
          int cz = 0;

          int xz = 0;
          xz = client_pod_map_szs;
          while(true)
          {
          clientpod_map_lock.lock();
          cz = client_pod_map.size();
          clientpod_map_lock.unlock();
            
            if(cz>client_pod_map_szs)
            {
                client_pod_map_szs = cz;
                break;
            }
          }

          //clientpod_map_lock.lock();
           
         // clientpod_map_lock.unlock();
          /*
          while(true)
          {
              //int client_map_sz = client_pod_map.size();
             clientpod_map_lock.lock();
             map<int,client_pod>::iterator cpm_iterator = client_pod_map.begin()+client_pod_map_szs;
             clientpod_map_lock.unlock();


              clientpod_map_lock.lock();
              map<int,client_pod>::iterator cpm_iterator_end = client_pod_map.end();
              clientpod_map_lock.unlock();


              if(xcpm_iterator!=cpm_iterator_end)
              {

               defman = xcpm_iterator->first;
               xcpm_iterator++;
               break;
              }
          }*/
              map<int,map<uint8_t,client_pod>>::iterator cpm_iterator = (client_pod_map.begin());

              for(int xxbl=0;xxbl<xz;xxbl++)
              {
                cpm_iterator++;
              }
              defman = cpm_iterator->first;
              cout<<endl<<"wait def started: "<<defman<<endl;
              cout.flush();
          return defman;
        }

};
