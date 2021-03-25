#include "mmap_file.hh"
#include <iostream>
#include <fstream>

// usage: ./a.out <file_name> mmap/fstream
// A driver for comparing performance between using fstream and MmapFile.
// Open a file and read one byte at a time, and then copy to user buffer.
int main(int argc, char** argv) {
    if (argc <= 2) {
      std::cout << "invalid number of input arguments" << std::endl;
      return -1;
    }


    char buff[1];
    size_t total = 0;
    
    auto callback = [&total, &buff](const char c){
                          buff[0] = c;
                          total++;
                    };

    if (std::string(argv[2]) == std::string("mmap")) {
        nikki::MmapFile file{argv[1]};
   

        // read all bytes into buffer
        while(size_t n = file.read(buff, sizeof(buff))) {
            callback(buff[0]);
        };
        std::cout << "mmap read " << total << " bytes";
    } else if (std::string(argv[2]) == std::string("fstream")) {
        std::fstream file;
        file.open(argv[1]);

        std::for_each(std::istream_iterator<char>(file),
                      std::istream_iterator<char>(),
                      callback);           
    } else {
        std::cout << "unspecified command";
    }


    return 0;
}
