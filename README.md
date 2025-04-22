# Class design

- Naming conventions: https://www.geeksforgeeks.org/naming-convention-in-c/
```
- The class name should be a noun.
- Use upper case letters as word separators, and lower case for the rest of the word in the class name.
- The first character in the class name must be in upper case.
- Each method/ function name should begin with a verb
- The private attribute name in class should be added with _
```
### Pos
- Position of items or chefs

- distanceWith
- isAdjacent
### Chef

#### Public Data Members
- **pos**: `Position`  
  Current grid coordinates of the chef.
- **item**: `Item`  
  What the chef is holding this turn (parsed from input).

#### Public Methods
- `void update(int x, int y, const string& itemStr)`  
  Reinitialize the chef’s state each turn.  
  - **x, y**: new position  
  - **itemStr**: e.g. `"DISH-APPLE-ICE_CREAM"`

- `bool isEmptyHanded() const noexcept`  
  Returns `true` if the chef’s inventory is empty.

- `string doActions(const string& action,  
                    const Position& target = {-1,-1},  
                    const string& comment = "") const`  
  Build the command string:  
  - If `action=="WAIT"`, returns `"WAIT"` or `"WAIT; comment"`.  
  - Otherwise returns `"MOVE x y; comment"` or `"USE x y; comment"`.

- `string dropItem(const Position& dropPos,  
                  const string& comment = "drop") const`  
  Issue a drop command:  
  - If empty‑handed, returns `"WAIT; nothing to drop"`.  
  - Otherwise calls  
    ```cpp
    Position dropPos = findClosestEmptyTable(); 
    return doActions("USE", dropPos, comment);
    ```

- `Position getPosition() const`  
  Returns the chef’s current `Position`.

- `const Item& getInventory() const`  
  Returns a const reference to the chef’s `Item` (parsed tokens).

#### Private Data Members
- `Position pos_`  
- `Item     item_`


### Items
- The contents of the item: vector str

- hasItems
- hasAllItems

### Table 
- Pos of items: Pos
- The items on the table: Items


### Kitchen
#### Public Member Functions
- `void initMap()` : Initialize the kitchen map
- `void setTableState()` : Update what's on the table
- `Position getClosestEmptyTable(const int &x, const int &y)`
- `vector<Position> getPosition(const string &name) ` : Get the position of sth

#### Private Data Members
- `vector<vector<char>> map_;` : kitchen map
- `unordered_map<string, Pos> equipment_;` : store the equipments
- `vector<Table> Table_;`
- `bool isInside(int x, int y)`
- `bool isTableOccupied(const Position& pos)`
### Customer
- item: Items
- award: int


