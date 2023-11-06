#ifndef ZION_UTILS_H_INCLUDED
#define ZION_UTILS_H_INCLUDED



#endif // ZION_UTILS_H_INCLUDED


int getIpInt(sockaddr &tzion_addr)
    {
        sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
        int ipaddr =  sinx->sin_addr.s_addr;

        return ipaddr;
    }



    in_addr getIpAddr(sockaddr &tzion_addr)
    {
        sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
        //int ipaddr =  sinx->sin_addr.s_addr;

        return sinx->sin_addr;
    }

    in_addr getIpAddr_int(int tzion_addr)
    {
        sockaddr_in *sinx;
        sinx->sin_family = AF_INET;
        sinx->sin_addr.s_addr = tzion_addr;
        //int ipaddr =  sinx->sin_addr.s_addr;

        return sinx->sin_addr;
    }

    short getPortNo(sockaddr &tzion_addr)
    {
        sockaddr_in *sinx = (sockaddr_in*)&tzion_addr;
        //int ipaddr =  sinx->sin_addr.s_addr;

        return sinx->sin_port;
    }


double doTimer(mutex& killtro_lock,bool& killtro,double &xkill_time,int zion_socket,sockaddr &xon_addr,int &xon_addr_len,vector<uint8_t> &dtDatax,int &dtData_sizex)
{
  vector<uint8_t> dtData = dtDatax;
  int dtData_size = dtData_sizex;
  double kill_time = xkill_time;
  sockaddr on_addr = xon_addr;
  int on_addr_len = xon_addr_len;



 chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
 double max_time = 0;
 cout<<endl<<"\t\tSTARTED TIMERD\n";
 while(!killtro)
 {
  chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();

  chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);

  double elasped_time=c_time.count();
  //killtro_lock.lock();
    bool dtkillme =  killtro;
  //killtro_lock.unlock();
  //  cout<<"\n\t\t dtkillme: "<<killtro<<endl;
  //  cout.flush();
  if(dtkillme==true)
  {
    cout<<"\n\n\t\t\t********broken wings: "<<dtkillme<<" \t\t"<<elasped_time<<endl<<endl;
    cout.flush();
    max_time = elasped_time;
    break;
  }

  if(elasped_time>=kill_time)
  {
    //  cout<<endl<<"\n\n\n\t\t\tpacket time ewxpired\n";
    //  cout.flush();
      int ixsend = sendto(zion_socket,(char*)dtData.data(),dtData_size,0,&on_addr,on_addr_len);

      if(ixsend<=0)
      {
        perror("\t\t\t****RESENDING PACKET FAILED");
      }
      else
      {
     //   perror("\t\t\t***packet resent okayed");
      }
      init_timepoint = chrono::steady_clock::now();
      //cout<<endl<<endl<<"\t\t****say your prayer: "<<elasped_time<<endl<<endl;
      //cout.flush();
  }
  //  cout<<"say your prayerz\n";
  //  cout.flush();

 }

xkill_time = true;
return max_time;
}

double doTimerX(mutex& killtro_lock, atomic<bool>& killtro, double& xkill_time, int zion_socket, sockaddr& xon_addr, int& xon_addr_len, const vector<uint8_t>& dtDatax, int dtData_sizex)
{
  const vector<uint8_t>& dtData = dtDatax;
  const int dtData_size = dtData_sizex;
  const double kill_time = xkill_time;
  const sockaddr& on_addr = xon_addr;
  const int on_addr_len = xon_addr_len;

  chrono::time_point<chrono::steady_clock> init_timepoint = chrono::steady_clock::now();
  double max_time = 0;
  cout << endl << "\t\tSTARTED TIMERD\n";
  while (true)
  {
    chrono::time_point<chrono::steady_clock> now_timepoint = chrono::steady_clock::now();

    chrono::duration<double, milli> c_time = now_timepoint - init_timepoint;

    double elasped_time = c_time.count();
    bool dtkillme = killtro.load(memory_order_relaxed);
    if (dtkillme)
    {
      cout << "\n\n\t\t\t********broken wings: " << dtkillme << " \t\t" << elasped_time << endl << endl;
      max_time = elasped_time;
      break;
    }

    if (elasped_time >= kill_time)
    {
      int ixsend = sendto(zion_socket, dtData.data(), dtData_size, 0, &on_addr, on_addr_len);

      if (ixsend <= 0)
      {
        perror("\t\t\t****RESENDING PACKET FAILED");
      }
      else
      {
        //perror("\t\t\t***packet resent okayed");
      }
      init_timepoint = chrono::steady_clock::now();
    }
    else
    {
    //   cout<<"\n\t\twake up\n\n"<<endl;
    //  cout.flush();
      chrono::duration<double, milli> sleep_time(kill_time - elasped_time);
      //this_thread::sleep_for(sleep_time);
      int sleep_time_int =  int (kill_time);
      this_thread::sleep_for(chrono::milliseconds(200));
    //  cout<<"\n\t\twaking up\n\n"<<endl;
      //cout.flush();
    }
  }
  killtro.store(false);
  return max_time;
}


/*double doTimerX(mutex& killtro_lock, atomic<bool>& killtro, double& xkill_time, int zion_socket, sockaddr& xon_addr, int& xon_addr_len, const vector<uint8_t>& dtDatax, int dtData_sizex)
{
  const vector<uint8_t>& dtData = dtDatax;
  const int dtData_size = dtData_sizex;
  const double kill_time = xkill_time;
  const sockaddr& on_addr = xon_addr;
  const int on_addr_len = xon_addr_len;

  chrono::time_point<chrono::high_resolution_clock> init_timepoint = chrono::high_resolution_clock::now();
  double max_time = 0;
  cout << endl << "\t\tSTARTED TIMERD\n";
  
  while (true)
  {
    chrono::time_point<chrono::high_resolution_clock> now_timepoint = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> c_time = now_timepoint - init_timepoint;

    double elapsed_time = c_time.count();

    {
      //lock_guard<mutex> lock(killtro_lock);  // Acquire the lock to synchronize access to `killtro`
      killtro_lock.lock();
      bool dtkillme = killtro.load(memory_order_relaxed);
      killtro_lock.unlock();
      if (dtkillme)
      {
        cout << "\n\n\t\t\t********broken wings: " << dtkillme << " \t\t" << elapsed_time << endl << endl;
        max_time = elapsed_time;
        break;
      }
    }

    if (elapsed_time >= kill_time)
    {
      int ixsend = sendto(zion_socket, dtData.data(), dtData_size, 0, &on_addr, on_addr_len);

      if (ixsend <= 0)
      {
        perror("\t\t\t****RESENDING PACKET FAILED");
      }
      else
      {
        //perror("\t\t\t***packet resent okayed");
      }
      init_timepoint = chrono::high_resolution_clock::now();
    }
    else
    {
      chrono::duration<double, milli> sleep_time(kill_time - elapsed_time);
      int sleep_time_int = int(sleep_time.count());
      this_thread::sleep_for(chrono::milliseconds(100));
    }
  }
 // {
  //  lock_guard<mutex> lock(killtro_lock);  // Acquire the lock to synchronize access to `killtro`
    killtro_lock.lock();
    killtro.store(false);
    killtro_lock.unlock();
 // }
  return max_time;
}
*/



double doTimer_cv(mutex& killtro_lock, condition_variable& killtro_cv,
                     atomic<bool>& killtro, double& xkill_time, int zion_socket,
                     sockaddr& xon_addr, int xon_addr_len, vector<uint8_t>& dtData,
                     int dtData_size)
{
    const double kill_time = xkill_time;
    const sockaddr& on_addr = xon_addr;
  const int on_addr_len = xon_addr_len;
    chrono::time_point<chrono::steady_clock> init_timepoint = chrono::steady_clock::now();
    double max_time = 0;
    cout << "\n\t\tSTARTED TIMERD\n";
    while (true) {
        unique_lock<mutex> lock(killtro_lock);
        bool dtkillme = killtro.load();
        if (dtkillme == true) {
            lock.unlock();
            cout << "\n\n\t\t\t********broken wings: " << dtkillme << "\t\t"
                 << chrono::duration_cast<chrono::milliseconds>(
                        chrono::steady_clock::now() - init_timepoint).count() << endl << endl;
            max_time = chrono::duration_cast<chrono::milliseconds>(
                        chrono::steady_clock::now() - init_timepoint).count();
            break;
        }

        if (chrono::duration_cast<chrono::milliseconds>(
                chrono::steady_clock::now() - init_timepoint).count() >= kill_time) {
            int ixsend = sendto(zion_socket, (char*) dtData.data(), dtData_size, 0, &on_addr, on_addr_len);
            if (ixsend <= 0) {
                perror("\t\t\t****RESENDING PACKET FAILED");
            }
            else {
                //perror("\t\t\t***packet resent okayed");
            }
            init_timepoint = chrono::steady_clock::now();
        }
        else {
            // wait until the kill signal is set or until 500 milliseconds have elapsed
            cout<<"\n\n\t\tinit wait"<<endl;
            cout.flush();
            killtro_cv.wait_for(lock, chrono::milliseconds(500));
            cout<<endl<<"\t\twait finished"<<endl;
            cout.flush();
        }
    }
    return max_time;
}


void doTimerXX(mutex& global_slock, atomic<bool>& killtro, double& xkill_time, int zion_socket, sockaddr& xon_addr, int& xon_addr_len, const vector<uint8_t>& dtDatax, int dtData_sizex)
{
  const vector<uint8_t> dtData = dtDatax;
  const int dtData_size = dtData_sizex;
  const double kill_time = 500;//xkill_time;
  const sockaddr on_addr = xon_addr;
  const int on_addr_len = xon_addr_len;
   std::this_thread::sleep_for(std::chrono::milliseconds(200));
   while (!killtro.load()) {
        // Perform the task
        std::cout << "Performing task..." << std::endl;
        cout.flush();
        
        // Wait for the specified timeout
        //global_sock_lock.lock();
        int ixsend = sendto(zion_socket, dtData.data(), dtData_size, 0, &on_addr, on_addr_len);
        //global_sock_lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    std::cout << "Kill signal received. Terminating..." << std::endl;
    cout.flush();

    // ...
    killtro.store(false);
   // return kill_time;
}