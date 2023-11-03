#ifndef OPTIONS_H_
#define OPTIONS_H_

struct Options {
    // 与重启点的间隔，前缀压缩时一次压缩的条目数量
    int block_restart_interval = 16;
};

#endif