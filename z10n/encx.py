import ctypes,os
def encry(txt):
    #txt = "hello darkness my old friend"
    txt_len = len(txt)
    libpath = os.path.dirname(os.path.abspath(__file__))+"/"+"libenko.so"
    print(libpath)
    lib = ctypes.CDLL(libpath)
    print("libpath encry")
    lib.encp.restype = ctypes.POINTER(ctypes.c_char)
    atxt = txt
    if type(txt) is str:
        atxt = txt.encode("ascii")
    ans = lib.encp(ctypes.c_char_p(atxt),ctypes.c_int(txt_len+1))
    cont = ctypes.cast(ans,ctypes.c_char_p)
    dec = str(cont.value,"ascii")
    lib.freemem(cont)
    return dec

#print(str(dec,"utf-8",errors="ignore"))

def decry(b64):
    print("\nstarted")
    libpath = os.path.dirname(os.path.abspath(__file__))+"/"+"libenko.so"
    print("\nlibpath loaded")
    print(libpath)
    lib = ctypes.CDLL(libpath)
    print("ready lib")
    b64_len = len(b64)
    b_b64 = b64#str(b64,"ascii")#.encode("ascii")
    print(b_b64)
    lib.dencp.restype=ctypes.c_char_p
    print("ready pizza")
    ansd = lib.dencp(ctypes.c_char_p(b_b64),ctypes.c_int(b64_len))
    print(len(ansd))
    print("aftra dec")
    contd = ctypes.cast(ansd,ctypes.c_char_p)
    decval = contd.value
   # lib.freemem(contd)
    print(str(contd.value,"ascii",errors="backslashreplace"))
    #return str(enc,"ascii",errors="replace")
    return decval

#print(str(encry("fly away"),"utf-8",errors="ignore"))
#print(str(enc,"utf-8",errors="ignore"))
#print(str(AES.block_size))

#etxt = encry("hello")
#dtxt = decry(etxt)
#print(etxt)
#print(dtxt)