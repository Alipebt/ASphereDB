#ifndef DB_H_
#define DB_H_

#include <atomic>
#include <string>
#include "../include/status.h"
#include "../table/memtable.h"

class DB {
   public:
    DB(const std::string &dppath);
    ~DB();

    static STATUS Create(const std::string &dbname);
    static STATUS Open(const std::string &dbname, DB **dbprt);
    static STATUS Destory(const std::string &dbname);

    STATUS Lookup();
    STATUS Put(const std::string &key, const std::string &value);
    STATUS Delete(const std::string &key);
    STATUS Get(const std::string &key, std::string &value);
    STATUS Write(const std::string &key, const std::string &value,
                 const Table::KeyType &kt);

   private:
    std::string dbname_;
    Table::MemTable *mtable_;

    std::atomic<int64_t> seq_;
};

#endif