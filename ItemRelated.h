#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "position.h"

class Items {
public:
    Items() {}

    Items(const std::string& rawString) {
        std::stringstream ss(rawString);
        std::string token;
        while (std::getline(ss, token, '-')) {
            if (!token.empty()) {
                contents_.push_back(token);
            }
        }
    }

    bool hasItem(const std::string& item) const {
        return std::find(contents_.begin(), contents_.end(), item) != contents_.end();
    }

    bool hasAllItems(const Items& other) const {
        for (const auto& item : other.contents_) {
            if (!hasItem(item)) return false;
        }
        return true;
    }

    bool isEmpty() const {
        return contents_.empty();
    }

private:
    std::vector<std::string> contents_;

    // void AddItem(const std::string& item) {
    //     contents_.push_back(item);
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