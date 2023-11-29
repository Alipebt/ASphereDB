#include "memtable.h"
#include <iostream>
#include "../util/file_name.h"
#include "sstable.h"

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

// TODO
STATUS Table::MemTable::BuildSSTable(const std::string &dbname,
                                     FileMetaData *meta, const Options &op) {
    std::string fname = SSTableFileName(dbname, meta->number);
    TempFile *temp_file = nullptr;
    FileManager::OpenTempFile(&temp_file);
    auto ssbuilder = new Table::SSTableBuilder(dbname, temp_file, op);
    for (auto it = memtable_.begin(); it != memtable_.end(); ++it) {
        MemKey memkey = it->first;
        std::string key = memkey.Encode2Key();
        ssbuilder->Add(key, it->second);
    }

    // finish
    ssbuilder->Finish();
    delete ssbuilder;

    return S_OK;
}

void MemTable::lookup() {
    for (auto it = memtable_.begin(); it != memtable_.end(); ++it) {
        std::cout << "|" << it->first.user_key_;
        std::cout << "|" << it->first.seq_;
        std::cout << "|" << kt2str(it->first.kt_);
        std::cout << "|" << it->second;
        std::cout << "|" << std::endl;
    }
    return;
}
