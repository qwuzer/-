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
    int x, y;
    int distanceTo(const Position& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
    bool isAdjacentTo(const Position& other) const {
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



class Kitchen {
public:
    Kitchen() : map_(7, vector<char>(11, '.')) {}

    void initMap() {
        for (int i = 0; i < 7; i++) {
            string line;
            getline(cin, line);
            for (int j = 0; j < 11; j++) {
                map_[i][j] = line[j];

                if (map_[i][j] == '0' || map_[i][j] == '1') {
                    map_[i][j] = '.';  
                }

                if (line[j] != '.' && line[j] != '#') {
                    string name;
                    switch (line[j]) {
                        case 'D': name = "DISH"; break;
                        case 'W': name = "WINDOW"; break;
                        case 'B': name = "BLUEBERRIES"; break;
                        case 'I': name = "ICE_CREAM"; break;
                        case 'S': name = "STRAWBERRIES"; break;
                        case 'C': name = "CHOPPING_BOARD"; break;
                        case 'H': name = "DOUGH"; break;
                        case 'O': name = "OVEN"; break;
                        default: name = "UNKNOWN"; break;
                    }
                    if (name != "UNKNOWN")
                        equipment_[name] = Position{j, i};
                }
            }
        }
    }

    Position getClosestEmptyTable(const int &x, const int &y) {
        vector<vector<bool>> visited(7, vector<bool>(11, false));
        queue<pair<Position, int>> q;
        q.push({Position{x, y}, 0});
        visited[y][x] = true;

        vector<pair<int, int>> directions = {{0,1}, {1,0}, {0,-1}, {-1,0}};
        vector<pair<int, int>> neighbor8 = {
            {0,1}, {1,0}, {0,-1}, {-1,0},
            {1,1}, {1,-1}, {-1,1}, {-1,-1}
        };

        while (!q.empty()) {
            auto [current, dist] = q.front(); q.pop();

            for (auto [dx, dy] : neighbor8) {
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (isInside(nx, ny) && map_[ny][nx] == '#') {
                    if (!isTableOccupied(Position{nx, ny})) {  
                        return Position{nx, ny};
                    }
                }
            }

            for (auto [dx, dy] : directions) {
                int nx = current.x + dx;
                int ny = current.y + dy;
                if (isInside(nx, ny) && !visited[ny][nx] && map_[ny][nx] == '.') {
                    visited[ny][nx] = true;
                    q.push({Position{nx, ny}, dist + 1});
                }
            }
        }
        return Position{-1, -1};
    }

    void setTableState() {
        Table_.clear();  
    
        int num_tables_with_items = 0;
        cin >> num_tables_with_items; cin.ignore();
    
        for (int i = 0; i < num_tables_with_items; i++) {
            int table_x, table_y;
            string item;
            cin >> table_x >> table_y >> item; cin.ignore();
    
            Table_.push_back(Table{Position{table_x, table_y}, item});
        }
    }

    vector<Position> getPosition(const string &name) {
        vector<Position> result;
        for (const auto &entry : equipment_) {
            if (entry.first == name) {
                result.push_back(entry.second);
            }
        }
        for (const auto &table : Table_) {
            if (table.item == name) {
                result.push_back(table.pos);
            }
        }
        return result;
    }

    void printMap() {
        cerr<<"Map:"<<endl;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 11; j++) {
                cerr << map_[i][j];
            }
            cerr<<endl;
        }
    }

    void printEquipment() {
        cerr<<"Equipment:"<<endl;
        for (const auto& n : equipment_) {
            cerr << n.first << ":" << n.second.x <<","<< n.second.y<<endl;
        }
    }

    void printTable() {
        cerr << "Table:" << endl;
        for (const auto& t : Table_) {
            cerr << "Pos: (" << t.pos.x << ", " << t.pos.y << ")  Item: " << t.item << endl;
        }
    }

    vector<vector<char>> getMap() const { return map_; }
    unordered_map<string, Position> getEquipment() const { return equipment_; }
    vector<Table> getTable() const { return Table_; }

private:
    vector<vector<char>> map_;
    unordered_map<string, Position> equipment_;
    vector<Table> Table_;

    bool isInside(int x, int y) {
        return x >= 0 && y >= 0 && x < 11 && y < 7;
    }

    bool isTableOccupied(const Position& pos) {
        for (const auto& table : Table_) {
            if (table.pos.x == pos.x && table.pos.y == pos.y) {
                return true;
            }
        }
        return false;
    }
};