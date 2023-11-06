/*#include <iostream>
#include <fstream>
#include <vector>
*/
#include "zion.h"
#include "enc.h"
int main(int argc, char* argv[])
{
     int defno = 10;
     std::string arg = argv[1];
     defno = std::stoi(arg);

     cout<<endl<<"using def no of: "<<defno<<endl<<endl;
     cout.flush();
     cin.get();
    start_zion();

    int zind = zlisten();
    // Open the file in binary mode
    std::ifstream file("max.mp3", std::ios::binary);
    
    if (!file) {
        std::cout << "Error opening file." << std::endl;
        return 1;
    }
    
    // Determine the file size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Create a buffer to store the file contents
    std::vector<char> buffer(1065);
    
    // Read the file contents into the buffer
    std::streampos bytesRead = 0;
    int wr_index = 0;

    int rem_bytes = (1065*defno);
    //while (bytesRead < fileSize) {
    while (bytesRead < (1065*defno))
     {
        file.read(buffer.data(), 1065);
        
        // Get the actual number of bytes read
        std::streamsize bytesReadThisIteration = file.gcount();


        cout<<"bytes to read: "<<bytesReadThisIteration;
        string xb64_dta = base64_encode(buffer.data(),(1065));
        string b64_dta = to_string(rem_bytes)+':'+to_string(wr_index)+':'+xb64_dta;

        cout<<endl<<"base64 string: "<<b64_dta<<endl;
        cout.flush();
        //pushZexport(zind,buffer.data(),bytesReadThisIteration);
        pushZexport(zind,b64_dta.c_str(),b64_dta.length());
        // Process the bytes in the buffer here
        /*
        for (int i = 0; i < bytesReadThisIteration; ++i) {
            char byte = buffer[i];
            std::cout << static_cast<int>(byte) << " ";
        }
        */
        bytesRead += bytesReadThisIteration;
        wr_index = wr_index +1;
        //rem_bytes = rem_bytes-1065;
    }
    cout<<"\ntotal bytes read: "<<bytesRead<<endl<<endl;
    // Close the file
    file.close();
    
    std::cout << std::endl;
    
    return 0;
}