#include <iostream>
#include <string>
#include <vector>

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
        if (isEmptyHanded()) {
            return doAction("WAIT", Position{-1,-1}, "nothing to drop");
        }

        return doAction("USE", dropPos , comment);
    }

    Position getPosition() const {
        ensureInitialized();
        return pos_; 
    }

    const std::vector<std::string>& getItems() const {
        ensureInitialized();
        return item_.getItems();
    }

    bool hasItem(const string item) {
        ensureInitialized();
        return item_.hasItem(item);
    }

    bool canServeCustomer(const Customer& customer) const {
        ensureInitialized();
        return chef.hasAllItems(customer.getItems());
    }



private:
    Position pos_;
    Items item_;   
    bool initialized_{false};

    void ensureInitialized() const {
        assert(initialized_ && "Chef not initialized: call update() or use parameterized ctor");
    }
};