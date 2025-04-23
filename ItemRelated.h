#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

class Items {
public:
    Items() {}
    Items(const std::string& rawString);

    void setItems(const std::string& rawString);

    bool hasItem(const std::string& item) const;
    bool hasAllItems(const Items& other) const;
    bool isEmpty() const;

    const std::vector<std::string>& getItems() const;
    
private:
    std::vector<std::string> items_;
};


class Table {
public:
    Table() {}
    Table(const Position& pos) : pos_(pos) {}

    void setPosition(const Position& pos) {
        pos_ = pos;
    }

    void setItems(const std::string& itemStr) {
        items_.setItems(itemStr);
    }

    Position getPosition() const {
        return pos_;
    }

    Items getItems() const {
        return items_;
    }
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