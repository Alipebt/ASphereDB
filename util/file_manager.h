#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <string>
#include "status.h"

// 每次写文件的缓冲区 65526
static constexpr size_t kWriterBuflg = 16;
static constexpr size_t kWriterBufSize = 1 << 16;

enum FileType {
    T_DIR,
    T_FILE,
};

class FileManager {
   public:
    static STATUS Create(const std::string &path, FileType ft);
    static STATUS Delete(const std::string &path);
    static STATUS Exists(const std::string &path);

   private:
    std::string path_;
};

class FileWriter {
   public:
    FileWriter(int fd);
    ~FileWriter();

    // 将数据写入文件
    STATUS Append(const std::string &data);
    STATUS FlushBuf();
    STATUS Close();

   private:
    STATUS Write(const char *buf, size_t size);

   private:
    int fd_;
    bool closed_;

    char buf_[kWriterBufSize];
    size_t pos_;
};

// TODO
class FileReader {
   public:
    FileReader(int fd);
    STATUS Read(uint64_t offset, size_t n);

   private:
    int fd_;
};

#endif