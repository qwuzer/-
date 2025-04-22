#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

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

    void setItems(const std::string& rawString) {
        items_.clear();
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
    Table(const Position& pos) : pos_(pos) {}
private:
    Position pos_;
    Items items_;
};

class Customer {
public:
    Customer() {}
    
    Customer(const std::string& itemStr, int awardValue)
        : item_(Items(itemStr)), award_(awardValue) {}

    void setItems(const std::string& itemStr) {
        item_.setItems(itemStr);
    }
    
    void setAward(int awardValue) {
        award_ = awardValue;
    }

    const Items& getItems() const {
        return item_;
    }

    const int& getAward() const {
        return award_;
    }

private:
    Items item_;
    int award_;
};