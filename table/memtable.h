#ifndef MEM_TABLE_H
#define MEM_TABLE_H

#include <cstdint>
#include <map>
#include <string>
#include "../include/options.h"
#include "../include/status.h"
#include "../util/file_manager.h"

namespace Table {

using namespace std;

enum KeyType {
    K_ADD,
    K_DELETE,
};

inline string kt2str(KeyType kt) {
    switch (kt) {
        case K_ADD:
            return "ADD";
        case K_DELETE:
            return "DELETE";
        default:
            return "ERROR";
    }
}

struct MemKey {
    string user_key_;
    int64_t seq_;
    KeyType kt_;

    MemKey(string uk, int64_t seq, KeyType kt)
        : user_key_(uk), seq_(seq), kt_(kt) {}

    MemKey(const string &key) : user_key_(key), seq_(INT64_MAX), kt_(K_ADD) {}

    bool operator<(const MemKey &other) const {
        if (user_key_ == other.user_key_) {
            return seq_ > other.seq_;
        }
        return user_key_ < other.user_key_;
    }

    std::string Encode2Key() {
        std::string key(user_key_);
        key.append(reinterpret_cast<char *>(&seq_), sizeof(int));
        key.append(reinterpret_cast<char *>(&kt_), 1);
        return key;
    }
};

class MemTable {
   public:
    MemTable() = default;
    ~MemTable() = default;

    STATUS Add(const MemKey &key, const string &value);
    STATUS Get(const string &key, string &value);
    STATUS BuildSSTable(const std::string &dbname, FileMetaData *meta,
                        const Options &op);

    // TEST
    void lookup();

   private:
    map<MemKey, string> memtable_;
};
}  // namespace Table

#endif