#include "coding.h"
#include <cstring>

int Decode2Int(std::string str) { return *(int *)str.c_str(); }
int Decode2Int(std::string str, size_t begin, size_t n) {
    std::string result = str.substr(begin, n);
    return *(int *)result.c_str();
}