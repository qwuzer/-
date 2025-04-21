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

    // void AddItem(const std::string& item) {
    //     contents.push_back(item);
    // }

    // void ClearItems() {
    //     contents.clear();
    // }
};

class Table {
public:
    Pos pos;
    Items items;

    Table(const Pos& pos) : pos(pos) {}

};

class Customer {
public:
    Items item;
    int award;

    Customer(const Items& orderItem, int awardValue)
        : item(orderItem), award(awardValue) {}
};