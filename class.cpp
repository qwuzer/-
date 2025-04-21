#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Chef {
public:
    void update(int x, int y, const string& itemStr) {
        pos_ = {x, y};
        item_ = Item(itemStr);
    }

    bool isEmptyHanded() const noexcept {
        return item_.isEmpty(); // TODO
    }

    string doActions(const string& action, cosnt Pos& target = {-1,-1}, const string& comment = "") const {
        if (action == "WAIT") {
            return action + comment;
        } else {
            return action + target.str() + "; " + commnet;
        }
    }

    string dropItem(const Position& dropPos, const string& comment = "drop") const {
        if (isEmptyHanded())
            return doAction("WAIT", {}, "nothing to drop");

        dropPos = findClosestEmptyTable() // TODO

        return doAction("USE", dropPos, comment);
    }

    Position getPosition() const {
        return pos_; 
    }

    const vector<Item>& getInventory() const { 
        return inventory_; 
    }

private:
    Position pos_;
    Item item_;   
}