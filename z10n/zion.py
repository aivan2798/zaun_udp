import ctypes,os

class Zion:
    
    def __init__(self):
        self.libpath = os.path.dirname(os.path.abspath(__file__))+"/"+"libzion.so"
        self.lib = ctypes.CDLL(self.libpath)
        self.lib.getInport.argtypes=[ctypes.c_uint,ctypes.c_uint]
        self.lib.getInport.restype = ctypes.c_char_p
        self.lib.pushZexport.restype = ctypes.c_int
        self.lib.zlisten.restype = ctypes.c_int

    def initZion(self):
         self.lib.start_zion()
        
    def z_listen(self):
        return self.lib.zlisten()

    def getData(self,zid,max_len):
        datax = self.lib.getInport(zid,max_len)
        data = ctypes.cast(datax,ctypes.c_char_p)

        return data.value

    def sendData(self,zid,data):
        data_len = len(data)
        sent_len = self.lib.pushZexport(ctypes.c_int(zid),ctypes.c_char_p(data),ctypes.c_int(data_len))
        return sent_len


        


