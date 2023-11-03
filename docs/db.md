## DB

`db`是数据库类，用于向用户提供操作APIs

```c++
static STATUS Create(const std::string &dbname);
static STATUS Open(const std::string &dbname, DB **dbprt);
static STATUS Destory(const std::string &dbname);
```

使用`static`向用户提供创建、打开、销毁的方法，其中`Open()`第二个参数需要传入`DB`指针的地址以保存方法内部创建的数据库。目前`Create()`和`Destory()`单纯的调用`File_Manager`相应方法，为数据库创建了一个目录。

## File_Manager

`file_manager`是一个用来对磁盘中文件夹操作的一个类，用于创建数据库的根目录

```c++
static STATUS Create(const std::string &path, FileType ft);
static STATUS Delete(const std::string &path);
static STATUS Exists(const std::string &path);
```

文件目录操作。
