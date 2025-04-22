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
                items_.push_back(token);
            }
        }
    }

    bool hasItem(const std::string& item) const {
        return std::find(items_.begin(), items_.end(), item) != items_.end();
    }

    bool hasAllItems(const Items& other) const {
        for (const auto& item : other.items_) {
            if (!hasItem(item)) return false;
        }
        return true;
    }

    const std::vector<std::string>& getItems() const 
    {
        return items_;
    }

    bool isEmpty() const {
        return items_.empty();
    }

private:
    std::vector<std::string> items_;

    // void AddItem(const std::string& item) {
    //     contents_.push_back(item);
    // }

    // void ClearItems() {
    //     contents_.clear();
    // }
};

class Table {
public:
    Table(const Pos& pos) : pos_(pos) {}
private:
    Pos pos_;
    Items items_;
};

class Customer {
public:
    Customer(const Items& orderItem, int awardValue)
        : item_(orderItem), award_(awardValue) {}

private:
    Items item_;
    int award_;
};