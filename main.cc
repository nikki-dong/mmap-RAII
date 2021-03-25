#include "mmap_file.hh"
#include <iostream>

// usage: ./a.out <file_name>
int main(int argc, char** argv) {
    if (argc <= 1) {
      std::cout << "invalid number of input arguments" << std::endl;
      return -1;
    }
    nikki::MmapFile file{argv[1]};

    return 0;
}
