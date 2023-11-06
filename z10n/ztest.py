import ctypes,os

libpath = os.path.dirname(os.path.abspath(__file__))+"/"+"libctest.so"


print(libpath)
lib = ctypes.CDLL(libpath)
print("library set")
lib.start_fx()
print("library loading")