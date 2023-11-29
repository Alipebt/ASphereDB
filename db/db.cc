#include "db.h"
#include <memory>
#include "../util/file_manager.h"

DB::DB(const std::string &dbpath)
    : dbname_(dbpath), mtable_(new Table::MemTable), versions_(new Version()) {}

DB::~DB() {
    delete mtable_;
    delete versions_;
}

STATUS DB::Create(const std::string &dbname) {
    return FileManager::Create(dbname, T_DIR);
}

STATUS DB::Destory(const std::string &dbname) {
    return FileManager::Delete(dbname);
}

STATUS DB::Open(const std::string &dbname, DB **dbptr) {
    if (S_OK != FileManager::Exists(dbname)) {
        return S_NOTFOUND;
    }
    DB *db = new DB(dbname);
    *dbptr = db;
    return S_OK;
}

STATUS DB::Write(const std::string &key, const std::string &value,
                 const Table::KeyType &kt) {
    Table::MemKey mk(key, seq_.fetch_add(1), kt);
    mtable_->Add(mk, value);
    return S_OK;
}

STATUS DB::Put(const std::string &key, const std::string &value) {
    return DB::Write(key, value, Table::K_ADD);
}

STATUS DB::Delete(const std::string &key) {
    return DB::Write(key, "", Table::K_DELETE);
}

STATUS DB::Get(const std::string &key, std::string &value) {
    return mtable_->Get(key, value);
}

void DB::lookup() { mtable_->lookup(); }

STATUS DB::WriteLevel0Table(Table::MemTable *mem) {
    FileMetaData meta;
    meta.number = versions_->NewFileNumber();

    mem->BuildSSTable(dbname_, &meta, op_);

    // TODO

    return S_OK;
}