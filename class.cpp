#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cmath>

class Position {
public: 
    int x, y;
    int distanceTo(const Pos& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
    bool isAdjacentTo(const Pos& other) const {
        return distanceTo(other) == 1 || (abs(x - other.x) == 1 && abs(y - other.y) == 1);
    }
    std::string str() const {
        return std::to_string(x) + " " + std::to_string(y);
    }
};


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


class Chef {
public:
    Chef () {}

    Chef(int x, int y, const std::string& itemStr) {
        update(x, y, itemStr);
    }


    void update(int x, int y, const std::string& itemStr) {
        pos_ = {x, y};
        item_ = Items(itemStr);
        initialized_ = true;
    }

    bool isEmptyHanded() const noexcept {
        ensureInitialized();
        return item_.isEmpty(); 
    }

    std::string doAction(const std::string& action, const Position& target = Position{-1,-1}, const std::string& comment = "") const {
        ensureInitialized();
        if (action == "WAIT") {
            return action + "; " + comment;
        } else {
            return action + target.str() + "; " + comment;
        }
    }

    std::string dropItem(const Position& dropPos, const std::string& comment = "drop") const {
        ensureInitialized();
        if (isEmptyHanded())
        return doAction("WAIT", Position{-1,-1}, "nothing to drop");
        return doAction("USE", dropPos, comment);
    }


    Position getPosition() const {
        ensureInitialized();
        return pos_; 
    }

    const std::vector<std::string>& getItems() const {
        ensureInitialized();
        return item_.getItems();
    }

private:
    Position pos_;
    Items item_;   
    bool initialized_{false};

    void ensureInitialized() const {
        assert(initialized_ && "Chef not initialized: call update() or use parameterized ctor");
    }
};


