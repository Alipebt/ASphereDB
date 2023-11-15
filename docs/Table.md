## Memery Table

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

在 map 中顺序先以`user_key`字典序排序，遇到相同的`user_key`则通过`seq`从大到小进行排序，`seq`值唯一且递增。其中相同`user_key`时，`seq`值越大，`memkey`越小。在 `memtable` 中的值并不能被真正的删除，只能通过添加一个相同的 `user_key` 并将其 `key_mode` 设置为 `DELETE` 来模拟删除。整个`memkay`的大小通过重载`<`来进行排序。

`MemKey`的构造函数中只传入`user_key`，模拟了`Get()`只知道`user_key`的情况，此时构造一个值最小的`memkey`，以此值寻找第一个比`memkey`大的 key 即为最新的 key。如果 `memkey` 的 `key_mode` 为 `DELETE`，则说明该 key 已经被删除。


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

## SSTable

``SSTable`以`block`为单位存储。一般由一组数据block和一组元数据block组成。元数据block存储了`SSTable`数据block的描述信息，如索引、BloomFilter、压缩、统计等信息。因为`SSTable`是不可更改的，且是有序的。

它本质上是一个树形结构：

```sql
								  [Footer]
								  	  |
						   +----------+----------+
						   |  				     |
				   	 [Index Block]  	  [Meta Data Block]
				   	 	   |					 |
				+----------+----------+	   [Filter Block]
				|	       |          |
		 [Data Block] [Data Block] [Data Block]
```

`SSTable`的物理结构如下：

```sql
<beginning_of_file>
[	   data block 0	  	 ]
[	   data block 1		 ]
[	  	   ...			 ]
[	   data block N		 ]
[	   filter block		 ]
[	 meta index block	 ]
[       index block		 ]
[		  Footer 		 ]                              
<end_of_file>
```

- `meta index block`存储了`filter block`的偏移量
- `index block`存储了每一个`data block`的偏移量
- `Footer`存储了`meta index block`和`index block`的偏移量。

一个Table包含多个`Data Block`；

- `Block`内是严格按 key 排序的，没有重复的 key；
- `Block`间也是按 key 排序的，且两个`Block`没有交集。

## BlockBuilder

`BlockBuilder`是用于构建数据块`(block)`的类，它以特定的顺序将键值对添加到数据块中，并写入磁盘。

```c++
STATUS Add(const std::string &key, const std::string &value);
STATUS Reset();
STATUS Finish();
```

- `Add()`将键值对写入`BlockBuilder`的`buffer_`字段，将用于存储
- `Reset()`重置正在构建的`Block`块
- `Finish()`完成正在构建的`Block`块

`Block`结构为：

```sql
0   [ 		K-V 	  ]
1   [	    K-V 	  ]
2   [ 		K-V  	  ]
    [		...		  ]
16  [	    K-V		  ]
17  [		K-V		  ]
    [ 		...		  ]
    [ 	 offset 0->0  ]
    [	 offset 1->16 ]
    [	    ...  	  ]
    [	 Num Restart  ]
```

其中 `K-V`为任意长度的键值对，而`offset`是`restart_`(`重启点`) 记录的偏移量。其中每16个`K-V`为一组重启点。重启点偏移量以及重启点数量均占`sizeof(int)`大小。

## Data Block

在 `data block` 中存储的 `key-value` 是顺序存储的，所以会出现多个连续的 `key` 有部分前缀是相同的，所以在写入`block`中时需要将key压缩。它的存储格式为：

```sql
[key相同部分的大小][key中不同部分的大小][value大小][key中不同部分的值][value值]
```

记录大小的元素(`前三部分`)占内存均为`sizeof(int)`。

我们一般是将16条 K-V对进行一次前缀压缩，举个例子，以下是原数据：

```sql
0  [key_aaaa:value000]
1  [key_aabb:value001]
2  [key_bbbb:value002]
...
16 [key_cccc:value016]
17 [key_dddd:value017]
```

这是压缩后的数据：

```sql
0  [0][8][8][key_aaaa][value000]
1  [6][2][8][bb      ][value001]
2  [4][4][8][bbbb    ][value002]
...
16 [0][8][8][key_cccc][value016]
17 [4][4][8][dddd    ][value017]
```

从以上例子中可以看到第 0 条和第 16 条的 key 是完整保存在 block 中的，所以它们也是**重启点**指向的点。

最后在此基础上追加 `restart_`的全部内容以及它的数量。

这些数据以二进制的形式存储在类型为  `std::string` 的`buffer_`中，当我们需要将特定内容取出时就需要用到以下方法：

- `buffer_.substr(begin,end)`，其取出的值类型为`std::string`
- `Decode2Int()`可以将`std::string`类型的内容以二进制形式直接转为 int 类型。

## Filter Block

一个`Table`包含一个`FilterBlock`，`FilterBlock`内包含多个`filter`。每大约`2K`的`Data Block`对应一个`Filter`，它的作用是快速判断对应的`data block`中是否存在某个 key。（见 `Bloom Filter`）

构造`filter block`时，将`data block`中的 key 全部存起来，并且传入这个`data block`的大小，计算它需要多少个`bitmap`来映射 keys。理想状态下一次性传入的`data block`的大小为 2K ，但是现实情况并不是这样，它会将所有的 key 都映射在一个`bitmap`中。

假如`data block`大小为 `7K` ，在方法内会计算所需的`bitmap`个数为 `7K/2K=3`。但是除过第一个`bitmap`真正记录映射信息，其他`bitmap`并不存在，只在索引的时候指向第一个`bitmap`(`keys已全部被构造成bitmap，此时其余2个bitmap的偏移量也和第一个bitmap偏移量相同`)。

```c++
bitmaps_       : [01001011][00010100]
bitmap_size_   : 4
bitmap_offset_ : [8][8][8][16]
```

#### `std::string FilterBlockBuilder::Finish()`

这里面将构造最终的`filter block`

```c++
//       |       string       |    int32s   |    int32    |       char      |
//       |       bitmaps      |    offset   | bitmap size |  block size log |
result :  [01001011][00010100] [8][8][8][16]      [16]            [11]
```

