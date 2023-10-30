#include "memtable.h"
#include <iostream>

RESULT MemTable::Put(const MemKey &key, const string &value)
{
    if (key.km_ == K_ADD)
    {
        memtable_[key] = value;
    }
    else if (key.km_ == K_DELETE)
    {
        memtable_[key] = "";
    }

    return RE_OK;
}

RESULT MemTable::Get(const MemKey &key, string &value)
{
    auto it = memtable_.find(key);
    if (it == memtable_.end())
    {
        return RE_NOTFOUND;
    }
    if (key.user_key_ == it->first.user_key_ && key.km_ != K_DELETE)
    {
        value = it->second;
    }
    return RE_OK;
}

RESULT MemTable::Lookup()
{
    for (auto it = memtable_.begin(); it != memtable_.end(); ++it)
    {
        std::cout << "|" << it->first.user_key_
                  << "|" << it->first.seq_
                  << "|" << km2str(it->first.km_)
                  << "|" << it->second
                  << "|" << std::endl;
    }
    return RE_OK;
}
