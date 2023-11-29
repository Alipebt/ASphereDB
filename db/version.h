#ifndef VERSION_H_
#define VERSION_H_

#include <stdint.h>
class Version {
   public:
    uint64_t NewFileNumber();

   private:
    int64_t next_file_number_;
};

#endif