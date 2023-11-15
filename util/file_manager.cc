#include "file_manager.h"
#include <string.h>
#include <unistd.h>
#include <filesystem>
namespace fs = std::filesystem;

STATUS FileManager::Create(const std::string &dbpath, FileType ft) {
    if (ft == T_DIR) {
        if (!fs::exists(dbpath)) {
            fs::create_directory(dbpath);
        } else {
            return S_EXISTS;
        }
    } else if (ft == T_FILE) {
        // TODO
        return S_ERROR;
    }
    return S_OK;
}

STATUS FileManager::Delete(const std::string &dbpath) {
    if (fs::exists(dbpath) && fs::is_directory(dbpath)) {
        fs::remove_all(dbpath);
    } else {
        return S_NOTFOUND;
    }
    return S_OK;
}

STATUS FileManager::Exists(const std::string &dbpath) {
    if (fs::exists(dbpath) && fs::is_directory(dbpath)) {
        return S_OK;
    } else {
        return S_NOTFOUND;
    }
    return S_ERROR;
}

FileWriter::FileWriter(int fd) : fd_(fd), closed_(false), pos_(0) {}

FileWriter::~FileWriter() {
    if (!closed_) {
        Close();
    }
}

STATUS FileWriter::Append(const std::string &data) {
    size_t data_size = data.size();
    if (data_size + pos_ < kWriterBufSize) {
        memcpy(buf_ + pos_, data.data(), data_size);
        pos_ += data_size;
        return S_OK;
    }
    // 超过缓冲区
    size_t write_size = kWriterBufSize - pos_;
    memcpy(buf_ + pos_, data.data(), write_size);
    pos_ += write_size;
    size_t size_left = data_size - write_size;

    STATUS st = FlushBuf();
    if (st != S_OK) {
        return st;
    }

    // 小数据写入缓冲区，大数据直接写入文件。
    if (size_left < kWriterBufSize) {
        memcpy(buf_, data.data() + write_size, size_left);
        pos_ += size_left;
        return S_OK;
    }
    return Write(data.data() + write_size, size_left);
}

STATUS FileWriter::FlushBuf() {
    if (pos_ > 0) {
        ssize_t ret = Write(buf_, pos_);
        if (ret < 0) {
            return S_IOERROR;
        }
        pos_ = 0;
    }
    return S_OK;
}

STATUS FileWriter::Close() {
    if (pos_ > 0) {
        STATUS st = FlushBuf();
        if (st != S_OK) {
            return st;
        }
    }
    if (fd_ != -1) {
        int ret = close(fd_);
        if (ret < 0) {
            return S_CLOSEERROR;
        }
    }
    closed_ = true;
    return S_OK;
}

STATUS FileWriter::Write(const char *data, size_t size) {
    ssize_t ret = 0;
    while (size > 0) {
        ret = write(fd_, data, size);
        if (ret < 0) {
            if (errno == EINTR) {
                continue;
            }
            return S_IOERROR;
        }
        size -= ret;
        data = (char *)data + ret;
    }
    return S_OK;
}

FileReader::FileReader(int fd) :fd_(fd){}
