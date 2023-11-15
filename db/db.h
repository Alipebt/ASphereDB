#ifndef DB_H_
#define DB_H_

#include <atomic>
#include <string>
#include "../include/status.h"
#include "../table/memtable.h"

class DB {
   public:
    DB() = delete;
    DB(const std::string &dbpath);
    ~DB();

    static STATUS Create(const std::string &dbname);
    static STATUS Open(const std::string &dbname, DB **dbprt);
    static STATUS Destory(const std::string &dbname);

    STATUS Put(const std::string &key, const std::string &value);
    STATUS Delete(const std::string &key);
    STATUS Get(const std::string &key, std::string &value);
    STATUS Write(const std::string &key, const std::string &value,
                 const Table::KeyType &kt);

    // TEST
    void lookup();

   private:
    std::string dbname_;
    Table::MemTable *mtable_;

    std::atomic<int64_t> seq_;
};

#endif