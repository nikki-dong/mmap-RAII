#pragma once

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <algorithm>

namespace nikki {
// A wrapper class that opens a file using mmap,
// and automatically cleans itself up (RAII).
class MmapFile {
public:
    // TODO: support writing as well.
    MmapFile(const char* file_name) : file_name_(file_name) {
        fd_ = ::open(file_name, O_RDONLY, 0);
        assert(fd_ != -1);

        file_size_ = get_file_size(file_name);

        mmap_start_ = static_cast<char*>(::mmap(NULL, 
                                                file_size_,
                                                PROT_READ,
                                                MAP_PRIVATE,
                                                fd_,
                                                0));
        assert(mmap_start_ != MAP_FAILED);
        
        // set cursor and boundary.
        cur_ = mmap_start_;
        end_ = cur_ + file_size_;
    }

    // Cleanup when goes out of scope
    ~MmapFile() {
        int rc = ::munmap(mmap_start_, file_size_);
        assert(rc == 0);
        rc = ::close(fd_);
        assert(rc == 0);
    }

    // copy bytes to destination and advance cursor.
    // return bytes copied.
    size_t read(char* dest, size_t n) {
        n = std::min(n, static_cast<size_t>(end_ - cur_));
        ::memcpy(dest, cur_, n);
        cur_ += n;
        return n;
    }

    // seek to a position within the file.
    // return false if seek out of bounds.
    bool seek(size_t offset_from_begin) {
        if (mmap_start_ + offset_from_begin >= end_) {
            return false;
        }
        cur_ = mmap_start_ + offset_from_begin;
        return true;
    }

    // return the size of the file.
    size_t size() const {
        return file_size_;
    }

private:
    static size_t get_file_size(const char* filename) {
        struct stat st;
        ::stat(filename, &st);
        return st.st_size;
    }

    const char* file_name_;
    char* mmap_start_ = nullptr;
    int fd_ = -1;
    size_t file_size_ = 0;
    
    // current cursor.
    char* cur_ = nullptr;
    // end of file.
    char* end_ = nullptr;
};

} // namespace nikki
