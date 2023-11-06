import ctypes,os

libpath = os.path.dirname(os.path.abspath(__file__))+"/"+"libzion.so"


print(libpath)
lib = ctypes.CDLL(libpath)
print("library set")
#pointer_type = ctypes.POINTER(ctypes.c_ubyte)
lib.getInport.argtypes=[ctypes.c_uint]
#data_block = (ctypes.c_ubyte * 256)()
#pointer = ctypes.cast(ctypes.addressof(data_block, pointer_type))
#_lib.f(pointer, 256)
lib.getInport.restype = ctypes.c_char_p
lib.start_zion()

while(True):
    print("\nneera\n")
    datax = lib.getInport(1500)

    data = ctypes.cast(datax,ctypes.c_char_p)
    #print(data.value)
    print(datax)
    lib.killmem(ctypes.c_char_p(datax))
    
    print(str(data.value,"ascii"))