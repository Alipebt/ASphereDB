#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

#include <string>
#include "status.h"

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

#endif