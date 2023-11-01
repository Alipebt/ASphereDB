#include "memtable.h"
#include <iostream>

using namespace Table;

STATUS MemTable::Add(const MemKey &key, const string &value) {
    if (key.kt_ == K_ADD) {
        memtable_[key] = value;
    } else if (key.kt_ == K_DELETE) {
        memtable_[key] = "";
    }

    return S_OK;
}

STATUS MemTable::Get(const string &key, string &value) {
    MemKey mk(key);
    auto it = memtable_.upper_bound(mk);
    if (it == memtable_.end()) {
        return S_NOTFOUND;
    }
    if (mk.user_key_ != it->first.user_key_) {
        return S_ERROR;
    } else if (mk.kt_ == K_DELETE) {
        return S_NOTFOUND;
    } else {
        value = it->second;
        return S_OK;
    }
    return S_ERROR;
}

STATUS MemTable::Lookup() {
    for (auto it = memtable_.begin(); it != memtable_.end(); ++it) {
        std::cout << "|" << it->first.user_key_;
        std::cout << "|" << it->first.seq_;
        std::cout << "|" << kt2str(it->first.kt_);
        std::cout << "|" << it->second;
        std::cout << "|" << std::endl;
    }
    return S_OK;
}
