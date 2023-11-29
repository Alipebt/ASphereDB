#ifndef FILE_NAME_H_
#define FILE_NAME_H_

#include <stdint.h>
#include <string>

static std::string MakeFileName(const std::string &dbname, uint64_t number,
                                const char *suffix) {
    char buf[100];
    std::snprintf(buf, sizeof(buf), "/%06llu.%s",
                  static_cast<unsigned long long>(number), suffix);
    return dbname + buf;
}

std::string SSTableFileName(const std::string &dbname, uint64_t number) {
    return MakeFileName(dbname, number, "sst");
}

#endif