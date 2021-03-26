MmapFile is a class that uses mmap() to read files. It is a header only class that can be easily included in your projects.

When normally reading files from disk, kernel first copies from disk to buffer cache, then copies from buffer cache to user space program. It quickly slows down your program if you only need on-pass file read.

mmap() on the other hand, can avoid extra copies to the buffer cache, by mapping files to memory up front.
