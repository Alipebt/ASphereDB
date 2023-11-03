# Memery Table

`memtable`是内存中的一张表，用户每次写入`Key/Value`都是向`memtable`中写。当`memtable`所占的内存大于一定值时，它会转为`Immutable`。`Immutable`内容无法再更改，等待转为`SStable`。

`memtable`底层用`std::map<key,value>`存储(一般为`skipList`)

目前 `memtable` 需要有两个操作：

- `Add`: 在 map 中添加新内容
- `Get`: 获取 map 中的内容

其中 key 类型为 `MemKey`，其结构如下:

```
|user_key | seq | key_mode |
```

- `user_key`: 用户输入的 key 值
- `seq`: 依次递增的标号
- `key_mode`: key的状态，如 `ADD`, `DELETE`

在 map 中顺序先以`user_key`字典序排序，遇到相同的`user_key`则通过`seq`从大到小进行排序，`seq`值唯一且递增。其中相同`user_key`时，`seq`值越大，`memkey`越小。

`MemKey`的构造函数中只传入`user_key`，模拟了`Get()`时只知道`user_key`的情况，此时构造的`memkey`值最小，以此值寻找第一个比`memkey`大的 key 即为最新的 key。


```c++
STATUS Add(const MemKey &key, const string &value);
```
其中`key`，`value` 均为传入参数，`value`为传出参数。

`Add()`所添加的新内容分为两种：`ADD`，`DELETE`。两种`key_mode`本质上都是向 `map` 中添加新内容，而`DELETE`仅仅作为已删除的标记，其`value`的值无效，为空即可。

```c++
STATUS MemTable::Get(const MemKey &key, string &value);
```

其中`key`为传入参数，`value`为传出参数。

按照以上所述的 map 顺序，同一`user_key`中`Get()`取到的是最大 `seq`，即为最新的`value`。

`Lookup()`打印`memtable`中所有内容，目前只作为测试用。

# SSTable

`SSTable`以`block`为单位存储。其中`block`类型有多种，如`Data Block`,`Filter Block`,`Meta Index Block`,`Index Block`等。



## BlockBuilder

`BlockBuilder`是用于构建数据块`(block)`的类，它以特定的顺序将键值对添加到数据块中，并写入磁盘。

```c++
STATUS Add(const std::string &key, const std::string &value);
STATUS Reset();
STATUS Finish();
```

- `Add()`将键值对写入`BlockBuilder`的`buffet_`字段，将用于存储
- `Reset()`重置正在构建的`Block`块
- `Finish()`完成正在构建的`Block`块

在 `block` 中存储的 `key-value` 是顺序存储的，所以会出现多个连续的 `key` 有部分前缀是相同的，所以在写入`block`中时需要将key压缩。它的存储格式为：

```
[key相同部分的大小][key中不同部分的大小][value大小][key中不同部分的值][value值]
```

我们一般是将16条 K-V对进行一次前缀压缩，举个例子，以下是原数据：

```
0  [key_aaaa:value000]
1  [key_aabb:value001]
2  [key_bbbb:value002]
...
16 [key_cccc:value016]
17 [key_dddd:value017]
```

这是压缩后的数据：

```
0  [0][8][8][key_aaaa][value000]
1  [6][2][8][bb      ][value001]
2  [4][4][8][bbbb    ][value002]
...
16 [0][8][8][key_cccc][value016]
17 [4][4][8][dddd    ][value017]
```

从以上例子中可以看到第 0 条和第 16 条的 key 是完整保存在 block 中的，所以它们也是**重启点**指向的点。

这些数据以二进制的形式存储在类型为  `std::string` 的`buffer_`中，当我们需要将特定内容取出时就需要用到以下方法：

- `buffer_.substr(begin,end)`，其取出的值类型为`std::string`
- `Decode2Int()`可以将`std::string`类型的内容以二进制形式直接转为 int 类型。
