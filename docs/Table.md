# Memery Table

`memtable`是内存中的一张表，用户每次写入`Key/Value`都是向`memtable`中写。当`memtable`所占的内存大于一定值时，它会转为`Immutable`。`Immutable`内容无法再更改，等待转为`SStable`。

`memtable`底层用`std::map<key,value>`存储(一般为`skipList`)

目前 `memtable` 需要有两个操作：

- `Add`: 在 map 中添加新内容
- `Get`: 获取 map 中的内容

## MemKey

其中 key 类型为 `MemKey`，其结构如下:

```
|user_key | seq | key_mode |
```

- `user_key`: 用户输入的 key 值
- `seq`: 依次递增的标号
- `key_mode`: key的状态，如 `ADD`, `DELETE`

在 map 中顺序先以`user_key`字典序排序，遇到相同的`user_key`则通过`seq`从大到小进行排序，`seq`值唯一且递增。其中相同`user_key`时，`seq`值越大，`memkey`越小。

`MemKey`的构造函数中只传入`user_key`，模拟了`Get()`时只知道`user_key`的情况，此时构造的`memkey`值最小，以此值寻找第一个比`memkey`大的 key 即为最新的 key。

## Add()

`Add()` 函数原型：

```c++
STATUS Add(const MemKey &key, const string &value);
```
其中`key`，`value` 均为传入参数，`value`为传出参数。

`Add()`所添加的新内容分为两种：`ADD`，`DELETE`。两种`key_mode`本质上都是向 `map` 中添加新内容，而`DELETE`仅仅作为已删除的标记，其`value`的值无效，为空即可。

## Get()

`Get()` 函数原型：

```c++
STATUS MemTable::Get(const MemKey &key, string &value);
```

其中`key`为传入参数，`value`为传出参数。

按照以上所述的 map 顺序，同一`user_key`中`Get()`取到的是最大 `seq`，即为最新的`value`。

## Lookup()

`Lookup()`打印`memtable`中所有内容，目前只作为测试用。
