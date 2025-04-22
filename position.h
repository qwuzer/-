#include<vector>

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