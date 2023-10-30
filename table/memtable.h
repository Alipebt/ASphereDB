#ifndef MEM_TABLE_H
#define MEM_TABLE_H

#include <string>
#include <map>
#include <result.h>

using namespace std;

enum KeyMode
{
    K_ADD,
    K_DELETE,
};

inline string km2str(KeyMode km)
{
    switch (km)
    {
    case K_ADD:
        return "ADD";
    case K_DELETE:
        return "DELETE";
    default:
        return "ERROR";
    }
}

struct MemKey
{
    string user_key_;
    int64_t seq_;
    KeyMode km_;

    MemKey(string uk, int64_t seq, KeyMode state)
        : user_key_(uk), seq_(seq), km_(K_ADD) {}

    bool operator<(const MemKey &other) const
    {
        if (user_key_ == other.user_key_)
        {
            return seq_ > other.seq_;
        }
        return user_key_ < other.user_key_;
    }
};

class MemTable
{
public:
    MemTable() = default;
    ~MemTable() = default;

    RESULT Put(const MemKey &key, const string &value);
    RESULT Get(const MemKey &key, string &value);
    RESULT Lookup();

private:
    map<MemKey, string> memtable_;
};

#endif