rm ../libzion.so
g++ -std=c++17 -lcrypto -lpthread -fPIC zion.cpp --shared -o libzion.so -fpermissive
cp libzion.so ../libzion.so 
