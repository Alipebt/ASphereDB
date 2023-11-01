# DBImpl

## db

`db`是数据库类，用于向用户提供操作APIs

## file_manager

`file_manager`是一个用来对磁盘中文件夹操作的一个类，用于创建数据库的根目录

## writer_batch

`writer_batch`是 DB 与 memtable 连接的通道，用户通过调用 DB 的读写 API，然后 DB 将信息传入 `writer_batch`, 通过整合 MemKey 和 Value向 memtable中写入内容
