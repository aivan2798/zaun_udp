#include<iostream>
#include<fstream>
using namespace std;
//starting project babylon
#include "Babylon.h"
#include "enc.h"
map<int,string>meta_cache;
int ctx = 0;

bool fin_tx = false;

void pukeFile(uint8_t *xrbytes,int xrbytes_len)
{

    string yflex_str = string((char*)xrbytes,xrbytes_len);
    int xid_pos = yflex_str.find(':');

    int xptr = stoi(yflex_str.substr(0,xid_pos));

    cout<<endl<<"total bytes should be: "<<xptr<<endl;
    cout.flush();

    string rbytes = yflex_str.substr(xid_pos+1,-1);
    int rbytes_len = rbytes.length();

    string flex_str = rbytes;
    //string((char*)rbytes,rbytes_len);

    int id_pos = flex_str.find(':');

    int ptr = stoi(flex_str.substr(0,id_pos));



    //map<int,string>::iterator it_meta_cache = meta_cache.find(ptr);
    string str_rbytes = flex_str.substr(id_pos+1,-1);
    meta_cache[ptr]=str_rbytes;

    ctx = ctx+1065;

    if(ctx>=xptr)
    {
        fin_tx = true;
    }

    /*
    if((ptr==ctx))
    {
        string str_rbytes = flex_str.substr(id_pos+1,-1);
        vector<uint8_t> deco_data = base64_decode(str_rbytes);

        cout<<endl<<"deco_dtaa of len: "<<deco_data.size()<<" and value "<<str_rbytes<<endl;
        ofstream rfile("maxy.mp3",ios::binary|ios::app);


    //rfile.write(reinterpret_cast<char*>(rbytes),rbytes_len);
        rfile.write(reinterpret_cast<char*>(deco_data.data()),deco_data.size());
    //rfile.flush();
        rfile.close();
        ctx = ctx+1;
        map<int,string>::iterator it_meta_cache = meta_cache.find(ctx);

        if(it_meta_cache==meta_cache.end())
        {
            cout<<"no lost dat here: "<<endl;
        }
        else
        {
            string xstr_rbytes = it_meta_cache->second;
            vector<uint8_t> deco_data = base64_decode(xstr_rbytes);

            cout<<endl<<"lost deco_dtaa of len: "<<deco_data.size()<<" and value "<<xstr_rbytes<<endl;
            ofstream rfile("maxy.mp3",ios::binary|ios::app);


    //rfile.write(reinterpret_cast<char*>(rbytes),rbytes_len);
            rfile.write(reinterpret_cast<char*>(deco_data.data()),deco_data.size());
    //rfile.flush();
            rfile.close();
            ctx = ctx+1;

            meta_cache.erase(it_meta_cache);
        }
    }
    else
    {
        meta_cache[ptr]=str_rbytes;
        cout<<endl<<"added lost one"<<endl;
        //ctx=ctx+1;

    }
    */



    //vector<uint8_t> deco_data = base64_decode(string((char*)rbytes,rbytes_len));


}


void xlpukeFile()
{
    int file_no = meta_cache.size();

    if(file_no>0)
    {
        for(int file_ind=0;file_ind<file_no;file_ind++)
        {
            string xstr_rbytes = meta_cache[file_ind];

            vector<uint8_t> deco_data = base64_decode(xstr_rbytes);

            cout<<endl<<"lost deco_dtaa of len: "<<deco_data.size()<<" and value "<<xstr_rbytes<<endl;
            ofstream rfile("maxy.mp3",ios::binary|ios::app);

            rfile.write(reinterpret_cast<char*>(deco_data.data()),deco_data.size());

            rfile.close();

        }

    }

}

void pukeInports()///Babylon &asock)
{
    while(true)
    {
        uint8_t datum[DEFAULT_MTU];
        memset(datum,0,DEFAULT_MTU);

        if(fin_tx==false)
        {

            int datum_sz =  getInports(datum,DEFAULT_MTU);
            cout<<endl<<"outted with data len: "<<datum_sz<<endl<<endl;
            cout.flush();





        if(datum_sz>0)
        {
            cout<<"received datum: "<<(char*)datum<<" with data len: "<<datum_sz<<endl<<endl;
            pukeFile(datum,datum_sz);
        }
        }
        else
        {
            xlpukeFile();
            break;
        }
    }


}
 future<void> pukeman;

int main()
{

    ///Babylon bsock("192.168.43.164",8055);
   /// Babylon bsock("3.111.75.190",8058);
   ///Babylon asock;
    ///asock.initBabylon("192.168.1.151",8055);
    ///startBabylonia();
    ///startAll(2);
    Babylon bsock;
    bsock.initBabylon("192.168.1.151",8055);
    Babylon csock;
    csock.initBabylon("192.168.1.151",8055);

    pukeman = async(launch::async,pukeInports);//,ref(bsock));
    while(true)
    {
        char data[DEFAULT_MTU];
        memset(data,0,DEFAULT_MTU);
        cout<<endl<<"PLEASE INPUT: ";
        cin>>data;
        cout<<"\n\t data len: "<<strlen(data)<<endl;
        ///bsock.sendExports((uint8_t*)data,strlen(data));
    }


   /* while(true)
    {
        char data[DEFAULT_MTU];
        memset(data,0,DEFAULT_MTU);
        bsock.getInports();//(uint8_t*)data,DEFAULT_MTU);

       // cout<<"\n\t recieved len: "<<data<<endl;

    }
    */
}
