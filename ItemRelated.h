#include <vector>
#include <string>
#include <algorithm>
#include "position.h"

class Items {
public:
    std::vector<std::string> contents;

    Items() {}

    Items(const std::vector<std::string>& initContents) : contents(initContents) {}

    bool HasItem(const std::string& item) const {
        return std::find(contents.begin(), contents.end(), item) != contents.end();
    }

    bool HasAllItems(const Items& other) const {
        for (const auto& item : other.contents) {
            if (!HasItem(item)) return false;
        }
        return true;
    }

};