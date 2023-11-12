# Util

## Bloom Filter 布隆过滤器

`BloomFilter` 继承了 `Filter`类，它是一种过滤策略。

`BloomFilter::CreateFilter()`方法是用于构建`bitmap`(位数组，多个 01 组成)。取多个哈希，并将`bitmap`对应位置为 1 。最佳哈希次数为通过公式计算可得：`k=n/m*ln2`，其中n/m是每个key所需的bit数，ln2 =~ 0.6。该方法里通过使用双重哈希算法来模拟多次哈希。

我们通过传入的 keys 的数量`( keys.size )`以及构造类时传入的`bits_per_key`计算得出 `bitmap_size`，然后将其按字节补全，使其长度为整字节的倍数。

此时`bitmap`大小已经确定，现在可以遍历 `keys` 然后让它进行多次`Hash`，并为`bitmap`赋值。

> 现在有两段 key 分别为 `"Hello"` 和 `"World!"`，假如它们都经过了两次哈希，并得出以下结果：
>
> ```c++
> "Hello"  : 3 5
> "World!" : 1 5
> ```
>
> 此时`bitmap`(假设长度为`8bit`) 为：
>
> ```c++
> [0][1][0][1][0][1][0][0]
> ```
>
> 当我们需要查找 `key = "World!"`这个 key 是否存在于与这个`Meta Block`相关的 `Data Block`中时，就会再进行一次 哈希，计算得出相应的哈希值，并与 `bitmap`对应位比较，如果全为1，则该 key ***有可能***存在

## Murmur3 哈希算法 

哈希函数是一种用于将任意数据映射为固定大小的值。而`MurmurHash3`是一种高速的非加密哈希函数，能够快速生成哈希值，尤其在大量数据的情况下效率显著。

`MurmurHash3`正如它的名字一样，它运算操作主要在于 `Mutiply 和 Rotate (乘，旋转)` 。它首先通过对输入数据进行分块处理（一般为4字节，32位），然后将每一块内容进行倒置，然后处理。

`MurmurHash3` 有32位和128位版本，以下以32位版本为例。

首先我们需要几个常量来参与这个哈希运算：

```c++
// 用于Mutiply运算
const uint32_t mu1 = 0xcc9e2d51;
const uint32_t mu2 = 0x1b873593;
// 用于Rotate运算
const uint32_t r1 = 15;
const uint32_t r2 = 13;
// 用于数据块处理最后的运算
const uint32_t m = 5;
const uint32_t n = 0xe6546b64;
```

这些常量是`MurmurHash3` 算法中使用的特定值，用于操作数据块以生成哈希值。它们的选择基于实验和数学分析，目的是使其产生更好的散列效果和均匀性，最大程度地减少不同输入映射到相同哈希值的情况（哈希冲突）。当然，好的取值不止以上这一种。

我们还需要一个种子`seed`，它的值是由用户传入的任意32位整数。哈希的结果是在`seed`的基础上进行计算并返回的。

```c++
uint32_t h = seed;
```

接下来可以进行对数据的分块了，这里假设输入的数据为`std::string`类型的字符串（如果想计算任意数据的哈希值，则可以将其二进制值作为输入来进行哈希）。

```c++
// 计算输入数据str的总字节数
const uint32_t len = str.size();
for (uint32_t i = 0; i <= len - 4; i += 4) {
    // 4字节内容倒置
    uint32_t k = ((uint32_t)str.at(i) << 24) |
                     ((uint32_t)str.at(i + 1) << 16) |
                     ((uint32_t)str.at(i + 2) << 8) | (uint32_t)str.at(i + 3);

    k *= mu1; // Mutiply
    k = (k << r1) | (k >> (32 - r1)); // Rotate 向左旋转 r1 位
    k *= mu2; // Mutiply

    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
}
```

此时只剩不足4位的数据了：

```c++
uint32_t k = 0;
// 倒置剩余的字节
switch (len % 4) {
    case 3:
        k ^= ((uint32_t)str.at(len - 1) << 16);
    case 2:
        k ^= ((uint32_t)str.at(len - 2) << 8);
    case 1:
        k ^= (uint32_t)str.at(len - 3);
    default:
        break;
}

k *= mu1;
k = (k << r1) | (k >> (32 - r1));
k *= mu2;
// 对seed进行计算
h ^= k;
h ^= len;
```

最后一步也是前人的总结：

```c++
h ^= h >> 16;
h *= 0x85ebca6b;
h ^= h >> 13;
h *= 0xc2b2ae35;
h ^= h >> 16;
```

`h`即为计算出的哈希值。