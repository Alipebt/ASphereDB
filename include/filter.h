#ifndef FILTER_H_
#define FILTER_H_

#include <string>
#include <vector>

class Filter {
   public:
    virtual std::string FilterName() const = 0;
    virtual void BuildBitmap(const std::vector<std::string> &keys,
                             std::string &bitmap) const = 0;
    virtual bool KeyMayMatch(const std::string &key,
                             const std::string &bitmap) const = 0;
};

#endif