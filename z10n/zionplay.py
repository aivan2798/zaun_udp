from zion import Zion
import threading
import os,json
import encx as enc


def ondl(peerfd,song_title,maxfilex):
            db = os.path.getsize(song_title+".mp3")
            db_name = song_title+".mp3"
            #self.active_dl
            #pdx["filename"]#+".m4a"
            #temp_fn = pdx["tmpfilename"]
            print("so far downloaded: "+str(db)+" at: "+db_name)
            dl_file = open(db_name,"rb")
            istrm_len = db
            strm_len = istrm_len
            lilstream = dl_file.read(db)
            dt_sp = 0
            dt_ep = 1024
            d_count = 0

            ptc = d_count
            rangec = 0
            file_ind = 0
            while(True):
                #self.maxfilex.send(lilstream[dt_sp:(dt_ep)])
                rangec = d_count - ptc
                #if(rangec==20):
                #    yorn = input("continue y/n")
                #    ptc = d_count
                #    if yorn=="y":
                #        print("okay")
                #    else:
                #        break
                payloada = enc.encry(lilstream[dt_sp:(dt_ep)])
                
               # payloada = payloada+'\0'
                load_len = 1024
                print("payload len: "+str(len(payloada)))
                file_meta = dict()
                file_meta["fp"]=d_count
                #file_ind
                file_meta["fs"]=1024
                file_meta["fd"]=payloada

                jpayloada = json.dumps(file_meta)

                maxfilex.sendData(peerfd,bytes(jpayloada,"ascii"))
                print("\t\t\t*****sent: "+jpayloada+" len: "+str(len(jpayloada)))
                strm_len = strm_len - 1024
                dt_sp = dt_ep
                dt_ep = dt_ep + 1024
                print(f"strm_len {strm_len}")
                print(f"\n\n\t\t________________\t*******d_count : {d_count}")
                d_count = d_count+1
            #break
                if(strm_len<1024):
                    dt_ep = strm_len-1
                    #self.maxfilex.send(lilstream[dt_sp:])
                    payloadb = enc.encry(lilstream[dt_sp:])

                    file_meta = dict()
                    file_meta["fp"]=d_count
                    #file_ind
                    file_meta["fs"]=istrm_len - dt_sp
                    file_meta["fd"]=payloadb

                    jpayloadb = json.dumps(file_meta)
                    maxfilex.sendData(peerfd,bytes(jpayloadb,"ascii"))
                    print("\t\t\t*****sent: "+jpayloadb+" len: "+str(len(jpayloadb)))
                    print(f"strm_len {strm_len}")
                    print(f"\n\n\t\t________________\t*******d_count : {d_count}")
                    d_count = d_count+1
                    break
            
            #print("sent total: "+str(istrm_len)+" and end size: "+str(strm_len))
          #  if(os.path.exists(db_name)):
          #    os.remove(db_name)
              #print("removed lag")
         #   else:
         #     print("no file")
            print("\tDOWNLOAD FINISHED")


def inportsman(zid,xion):
    while(True):
        print("\nneera\n")
       # datax = 0
        #print("loo")
        datax = xion.getData(zid,1500)

        #data = ctypes.cast(datax,ctypes.c_char_p)
        #print(data.value)
        print(datax)
       # lib.killmem(ctypes.c_char_p(datax))
    
        print(str(datax,"ascii"))



zionx = Zion()
zionx.initZion()
print("welcome homie")
zid = zionx.z_listen()
print("using zid {:d}".format(zid))
threading.Thread(target=inportsman,args=(zid,zionx)).start()

while(True):
   # data = input("any data sir")
   # print(data)
    #bydata = bytes(data,"ascii")
 #   ondl(zid,"Sab0103",zionx)
    ondl(zid,"quesera",zionx)
    exit()
    break
   # zionx.sendData(zid,bydata)
#exit()
