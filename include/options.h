#ifndef OPTIONS_H_
#define OPTIONS_H_

struct Options {
    // 与重启点的间隔，前缀压缩时一次压缩的条目数量
    int block_restart_interval = 16;
    // 每一个key在fliter中占的位数
    int bits_per_key = 10;
    // 过滤器
    const Filter *filter = nullptr;
    // 每一个 block 的大致大小
    size_t block_size = 4 * 1024;
};

#endif