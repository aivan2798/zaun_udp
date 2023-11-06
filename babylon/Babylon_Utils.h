#ifndef BABYLON_UTILLS_H_INCLUDED
#define BABYLON_UTILLS_H_INCLUDED



#endif // BABYLON_UTILLS_H_INCLUDED


void setRTO(int asock,double millisecs)
{
    int timeoutval = (millisecs+5)/1000;
    setsockopt(asock,SOL_SOCKET,SO_RCVTIMEO,(const char *)timeoutval,sizeof(timeoutval));
}

double timerd(mutex& killtro_lock,bool& killtro)
{
 chrono::time_point<chrono::steady_clock> init_timepoint= chrono::steady_clock::now();
 double max_time = 10;
 cout<<endl<<"\t\tSTARTED TIMERD\n";
 while(true)
 {
  chrono::time_point<chrono::steady_clock> now_timepoint= chrono::steady_clock::now();

  chrono::duration<double,milli> c_time=chrono::duration_cast<chrono::milliseconds>(now_timepoint-init_timepoint);

  double elasped_time=c_time.count();
  killtro_lock.lock();
    bool killme =  killtro;
  killtro_lock.unlock();

  if(killme==true)
  {
    cout<<"\nbroken wings: "<<elasped_time<<endl<<endl;
    cout.flush();
    max_time = elasped_time;
    break;
  }


 }


return max_time;
}
