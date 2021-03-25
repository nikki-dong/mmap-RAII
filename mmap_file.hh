#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

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

        mmap_start_ = ::mmap(NULL, file_size_, PROT_READ, MAP_PRIVATE, fd_, 0);
        assert(mmap_start_ != MAP_FAILED);
    }

    // Cleanup when goes out of scope
    ~MmapFile() {
        int rc = ::munmap(mmap_start_, file_size_);
        assert(rc == 0);
        rc = ::close(fd_);
        assert(rc == 0);
    }

private:
    static size_t get_file_size(const char* filename) {
        struct stat st;
        ::stat(filename, &st);
        return st.st_size;
    }

    const char* file_name_;
    void* mmap_start_ = nullptr;
    int fd_ = -1;
    size_t file_size_ = 0;
};

} // namespace nikki
