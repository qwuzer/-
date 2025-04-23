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
        setItems(rawString);
    }

    void setItems(const std::string& rawString) {
        items_ = rawString;
    }

    bool hasItem(const std::string& item) const {
        std::stringstream ss(items_);
        std::string token;
        while (std::getline(ss, token, '-')) {
            if (token == item) return true;
        }
        return false;
    }

    bool hasAllItems(const Items& other) const {
        std::stringstream ss(other.items_);
        std::string token;
        while (std::getline(ss, token, '-')) {
            if (!hasItem(token)) return false;
        }
        return true;
    }

    std::string getItemsString() const {
        return items_;
    }

    std::vector<std::string> getItems() const {
        std::vector<std::string> result;
        std::stringstream ss(items_);
        std::string token;
        while (std::getline(ss, token, '-')) {
            if (!token.empty()) {
                result.push_back(token);
            }
        }
        return result;
    }

    bool isEmpty() const {
        return items_.empty();
    }

private:
    std::string items_;
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

    Kitchen() : map_(kMaxHeight, vector<char>(kMaxWidth, '.')) {}

    void initMap() {
        for (int i = 0; i < kMaxHeight; i++) {
            string line;
            getline(cin, line);
            for (int j = 0; j < kMaxWidth; j++) {
                char ch = line[j];
                map_[i][j] = (ch == '0' || ch == '1') ? '.' : ch;

                if (ch != '.' && ch != '#' && ch != '0' && ch != '1') {
                    string name;
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
                            cerr << "Warning: Unknown equipment character: " << ch << endl;
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
        cin >> num_tables_with_items; cin.ignore();

        for (int i = 0; i < num_tables_with_items; i++) {
            int table_x, table_y;
            string item;
            cin >> table_x >> table_y >> item; cin.ignore();

            tables_.emplace_back(Position{table_x, table_y}, item);
        }
    }

    Position getClosestEmptyTable(const int& x, const int& y) const {
        vector<vector<bool>> visited(kMaxHeight, vector<bool>(kMaxWidth, false));
        queue<Position> q;
        q.push(Position{x, y});
        visited[y][x] = true;

        vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        vector<pair<int, int>> neighbor8 = {
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

    vector<Position> getPosition(const string& name) const {
        vector<Position> result;

        auto it = equipment_.find(name);
        if (it != equipment_.end()) {
            result.push_back(it->second);
        }

        for (const auto& table : tables_) {
            if (table.getItems().hasItem(name)) {
                result.push_back(table.getPosition());
            }
        }

        return result;
    }

    void printMap() const {
        cerr << "Map:" << endl;
        for (int i = 0; i < kMaxHeight; i++) {
            for (int j = 0; j < kMaxWidth; j++) {
                cerr << map_[i][j];
            }
            cerr << endl;
        }
    }

    void printEquipment() const {
        cerr << "Equipment:" << endl;
        for (const auto& n : equipment_) {
            cerr << n.first << ": " << n.second.x << "," << n.second.y << endl;
        }
    }

    void printTable() const {
        cerr << "Table:" << endl;
        for (const auto& t : tables_) {
            cerr << "Pos: (" << t.getPosition().x << ", " << t.getPosition().y << ")  Items: ";
            cerr << t.getItems().getItemsString() << " ";
            cerr << endl;
        }
    }

    const vector<vector<char>>& getMap() const { return map_; }
    const unordered_map<string, Position>& getEquipment() const { return equipment_; }
    const vector<Table>& getTable() const { return tables_; }

private:
    vector<vector<char>> map_;
    unordered_map<string, Position> equipment_;
    vector<Table> tables_;

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