#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <queue>

using namespace std;


class Position {
public: 
    int x = -1;
    int y = -1;

    int distanceTo(const Position& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
    bool isAdjacentTo(const Position& other) const {
        return (distanceTo(other) == 1) || 
               (abs(x - other.x) == 1 && abs(y - other.y) == 1);
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

    void addItem(const std::string& item) {
        items_.push_back(item);
    }

    void removeItem(const std::string& item) {
        auto it = std::remove(items_.begin(), items_.end(), item);
        if (it != items_.end()) {
            items_.erase(it, items_.end());
        }
    }

    void clear() {
        items_.clear();
    }

    bool hasOnlyItem(const std::string& item) const {
        return items_.size() == 1 && items_[0] == item;
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

    bool isEmpty() const {
        return items_.empty();
    }

    std::string getItemsString() const {
        std::string result;
        for (size_t i = 0; i < items_.size(); ++i) {
            result += items_[i];
            if (i < items_.size() - 1) {
                result += "-";
            }
        }
        return result;
    }

    const std::vector<std::string>& getItems() const 
    {
        return items_;
    }

private:
    std::vector<std::string> items_;
};



class Table {
public:
    Table() {}
    Table(const Position& pos, const std::string& itemStr) : pos_(pos) {
        items_.setItems(itemStr);
    }

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

    bool isEmptyHanded() const {
        ensureInitialized();
        return item_.isEmpty(); 
    }

    std::string doAction(const std::string& action, const Position& target = Position{-1,-1}, const std::string& comment = "") const {
        ensureInitialized();
        if (action == "WAIT") {
            return action + "; " + comment;
        } else {
            return action + " " + target.str() + "; " + comment;
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

    bool hasItem(const string& item) const {
        ensureInitialized();
        return item_.hasItem(item);
    }

    bool canServeCustomer(const Customer& customer) const {
        ensureInitialized();
        return item_.hasAllItems(customer.getItems());
    }

private:
    Position pos_;
    Items item_;   
    bool initialized_{false};

    void ensureInitialized() const {
        assert(initialized_ && "Chef not initialized: call update() or use parameterized ctor");
    }
};


class Kitchen {
public:
    static constexpr int kMaxHeight = 7;
    static constexpr int kMaxWidth = 11;

    Kitchen() : map_(kMaxHeight, std::vector<char>(kMaxWidth, '.')) {}

    void initMap() {
        for (int i = 0; i < kMaxHeight; i++) {
            std::string line;
            getline(std::cin, line);
            for (int j = 0; j < kMaxWidth; j++) {
                char ch = line[j];
                map_[i][j] = (ch == '0' || ch == '1') ? '.' : ch;

                if (ch != '.' && ch != '#' && ch != '0' && ch != '1') {
                    std::string name;
                    switch (ch) {
                        case 'D': name = "DISH"; break;
                        case 'W': name = "WINDOW"; break;
                        case 'B': name = "BLUEBERRIES"; break;
                        case 'I': name = "ICE_CREAM"; break;
                        case 'S': name = "STRAWBERRIES"; break;
                        case 'C': name = "CHOPPING_BOARD"; break;
                        case 'H': name = "DOUGH"; break;
                        case 'O': name = "OVEN"; break;
                        default:
                            std::cerr << "Warning: Unknown equipment character: " << ch << std::endl;
                            continue;
                    }
                    equipment_[name] = Position{j, i};
                }
            }
        }
    }

    void setTableState() {
        tables_.clear();

        int num_tables_with_items;
        std::cin >> num_tables_with_items; std::cin.ignore();

        for (int i = 0; i < num_tables_with_items; i++) {
            int table_x, table_y;
            std::string item;
            std::cin >> table_x >> table_y >> item; std::cin.ignore();

            tables_.emplace_back(Position{table_x, table_y}, item);
        }
    }

    Position getClosestEmptyTable(const int& x, const int& y) const {
        std::vector<std::vector<bool>> visited(kMaxHeight, std::vector<bool>(kMaxWidth, false));
        std::queue<Position> q;
        q.push(Position{x, y});
        visited[y][x] = true;

        std::vector<std::pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        std::vector<std::pair<int, int>> neighbor8 = {
            {0,1}, {1,0}, {0,-1}, {-1,0},
            {1,1}, {1,-1}, {-1,1}, {-1,-1}
        };

        while (!q.empty()) {
            Position current = q.front(); q.pop();

            for (const auto& [dx, dy] : neighbor8) {
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (isInside(nx, ny) && map_[ny][nx] == '#' &&
                    !isTableOccupied(Position{nx, ny})) {
                    return Position{nx, ny};
                }
            }

            for (const auto& [dx, dy] : directions) {
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (isInside(nx, ny) && !visited[ny][nx] && map_[ny][nx] == '.') {
                    visited[ny][nx] = true;
                    q.push(Position{nx, ny});
                }
            }
        }

        return Position{-1, -1};
    }

    std::vector<Position> getPosition(const std::string& name) const {
        std::vector<Position> result;

        auto it = equipment_.find(name);
        if (it != equipment_.end()) {
            result.push_back(it->second);
        }

        for (const auto& table : tables_) {
            if (table.getItems().hasOnlyItem(name)) {
                result.push_back(table.getPosition());
            }
        }

        return result;
    }

    void printMap() const {
        std::cerr << "Map:" << std::endl;
        for (int i = 0; i < kMaxHeight; i++) {
            for (int j = 0; j < kMaxWidth; j++) {
                std::cerr << map_[i][j];
            }
            std::cerr << std::endl;
        }
    }

    void printEquipment() const {
        std::cerr << "Equipment:" << std::endl;
        for (const auto& n : equipment_) {
            std::cerr << n.first << ": " << n.second.x << "," << n.second.y << std::endl;
        }
    }

    void printTable() const {
        std::cerr << "Table:" << std::endl;
        for (const auto& t : tables_) {
            std::cerr << "Pos: (" << t.getPosition().x << ", " << t.getPosition().y << ")  Items: ";
            std::cerr << t.getItems().getItemsString() << " ";
            std::cerr << std::endl;
        }
    }

    const std::vector<std::vector<char>>& getMap() const { return map_; }
    const std::unordered_map<std::string, Position>& getEquipment() const { return equipment_; }
    const std::vector<Table>& getTable() const { return tables_; }

private:
    std::vector<std::vector<char>> map_;
    std::unordered_map<std::string, Position> equipment_;
    std::vector<Table> tables_;

    bool isInside(int x, int y) const {
        return x >= 0 && y >= 0 && x < kMaxWidth && y < kMaxHeight;
    }

    bool isTableOccupied(const Position& pos) const {
        for (const auto& table : tables_) {
            if (table.getPosition().x == pos.x && table.getPosition().y == pos.y) {
                return true;
            }
        }
        return false;
    }
};




int main() {
    // --- Position tests ---
    Position p1{2, 3}, p2{5, 3};
    cout << "p1: " << p1.str()
         << ", p2: " << p2.str() << "\n";
    cout << "Distance p1→p2: " << p1.distanceTo(p2) << "\n";
    cout << "Adjacent? " << boolalpha << p1.isAdjacentTo(p2) << "\n\n";

    // --- Items tests ---
    Items its("DISH-DOUGH-ICE_CREAM");
    cout << "Items string: " << its.getItemsString() << "\n";
    cout << "Parsed tokens:";
    for (auto& tok : its.getItems())
        cout << " [" << tok << "]";
    cout << "\n";
    cout << "Has 'DOUGH'? " << its.hasItem("DOUGH") << "\n";
    cout << "Has only 'DOUGH'? " << its.hasOnlyItem("DOUGH") << "\n";
    its.addItem("BLUEBERRIES");
    cout << "After add, has 'BLUEBERRIES'? " << its.hasItem("BLUEBERRIES") << "\n";
    its.removeItem("ICE_CREAM");
    cout << "After remove, has 'ICE_CREAM'? " << its.hasItem("ICE_CREAM") << "\n";
    cout << "Is empty? " << its.isEmpty() << "\n";
    its.clear();
    cout << "After clear, is empty? " << its.isEmpty() << "\n\n";

    // --- Table tests ---
    Table tbl({4,2}, "DISH-DOUGH");
    cout << "Table at " << tbl.getPosition().str()
         << " holds: " << tbl.getItems().getItemsString() << "\n";
    tbl.setPosition({6,1});
    tbl.setItems("TART-ICE_CREAM");
    cout << "Moved to " << tbl.getPosition().str()
         << ", now holds: " << tbl.getItems().getItemsString() << "\n\n";

    // --- Customer tests ---
    Customer cust("DISH-DOUGH", 150);
    cout << "Customer wants: " << cust.getItems().getItemsString()
         << ", award: " << cust.getAward() << "\n";
    cust.setItems("TART-DOUGH");
    cust.setAward(200);
    cout << "After update wants: " << cust.getItems().getItemsString()
         << ", award: " << cust.getAward() << "\n\n";

    // --- Chef tests ---
    Chef chef; 
    chef.update(1,1,"NONE");
    cout << "Chef empty? " << chef.isEmptyHanded() << "\n";
    chef.update(1,1,"DISH-DOUGH");
    cout << "Chef holds:";
    for (auto& tok : chef.getItems()) cout << " " << tok;
    cout << "\n";
    cout << chef.doAction("MOVE", {2,2}, "go chop") << "\n";
    cout << chef.doAction("USE", {4,5}, "pick up") << "\n";
    cout << chef.dropItem({3,3}, "drop here") << "\n";
    cout << "Can serve cust? " << chef.canServeCustomer(cust) << "\n\n";

    // --- Kitchen tests ---
    Kitchen kitchen;
    cout << "Initial map:\n";
    kitchen.printMap();
    cout << "Equipment (should be empty):\n";
    kitchen.printEquipment();
    cout << "Tables (should be none):\n";
    kitchen.printTable();
    auto emptyTable = kitchen.getClosestEmptyTable(0,0);
    cout << "Closest empty table from (0,0): "
         << emptyTable.str() << "\n";
    auto eqPos = kitchen.getPosition("DISH");
    cout << "'DISH' positions found: " << eqPos.size() << "\n";

    return 0;
}