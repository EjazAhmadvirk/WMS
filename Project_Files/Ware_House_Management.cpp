#include <iostream>
#include <string>
#include <vector>
#include <fstream>      // For file handling
#include <unistd.h> 
using namespace std;

// -------------------- Item Class --------------------
class Item {
public:
    int itemID;
    string name;
    string category;
    int quantity;
    double price;

    Item(int id = 0, string n = "", string cat = "", int qty = 0, double p = 0.0) {
        itemID = id;
        name = n;
        category = cat;
        quantity = qty;
        price = p;
    }

    void displayItem() {
        cout << "\t\t\t\t\t\t\tID: " << itemID << ", Name: " << name
             << ", Category: " << category << ", Quantity: " << quantity
             << ", Price: $" << price << endl;
        // system("PAUSE");
        // system("CLS"); use this for clear console screen. this is built in function
    }
};

//  Warehouse Class 
class Warehouse {
public:
    int warehouseID;
    string location;
    vector<Item> items;

    Warehouse(int id = 0, string loc = "") {
        warehouseID = id;
        location = loc;
    }

    void addItem(const Item& item) {
        items.push_back(item);
    }

    bool deleteItem(int itemID) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->itemID == itemID) {
                items.erase(it);
                return true;
            }
        }
        return false;
    }

    Item* searchItem(int itemID) {
        for (auto& item : items) {
            if (item.itemID == itemID) {
                return &item;
            }
        }
        return nullptr;
    }

    void displayWarehouse() {
        cout << "\t\t\t\t\t\t\tWarehouse ID: " << warehouseID << ", Location: " << location << endl;
        cout << "\t\t\t\t\t\t\tItems in this warehouse:\n";
        for (auto& item : items) {
            item.displayItem();
        }
        // system("PAUSE");
        // system("CLS");
    }
};

// Node Linked List 
class WarehouseNode {
public:
    Warehouse data;
    WarehouseNode* next;

    WarehouseNode(const Warehouse& w) {
        data = w;
        next = nullptr;
    }
};

class WarehouseLinkedList {
public:
    WarehouseNode* head;

    WarehouseLinkedList() {
        head = nullptr;
    }

    ~WarehouseLinkedList() {
        WarehouseNode* curr = head;
        while (curr != nullptr) {
            WarehouseNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }

    void addWarehouse(const Warehouse& w) {
        WarehouseNode* newNode = new WarehouseNode(w);
        if (!head) {
            head = newNode;
        } else {
            WarehouseNode* curr = head;
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newNode;
        }
    }

    WarehouseNode* findWarehouse(int id) {
        WarehouseNode* curr = head;
        while (curr) {
            if (curr->data.warehouseID == id)
                return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    bool deleteWarehouse(int id) {
        if (!head) return false;
        if (head->data.warehouseID == id) {
            WarehouseNode* temp = head;
            head = head->next;
            delete temp;
            return true;
        }
        WarehouseNode* prev = head;
        WarehouseNode* curr = head->next;
        while (curr) {
            if (curr->data.warehouseID == id) {
                prev->next = curr->next;
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void displayWarehouses() {
        if (!head) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tNo warehouses available.\n";
            sleep(5);
            system("CLS");
            system("PAUSE");
            return;
        }
        WarehouseNode* curr = head;
        while (curr) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            curr->data.displayWarehouse();
            cout << "\t\t\t\t\t\t\t--------------------------\n";
            curr = curr->next;
        }
        sleep(5); 
        
        system("CLS");
        system("PAUSE");
    }

    // File Handling: Save all warehouses to file
    void saveToFile(const string& filename) {
        ofstream fout(filename);
        if (!fout) {
            cout << "Error opening file for writing!\n";
            return;
        }
        WarehouseNode* curr = head;
        while (curr) {
            fout << curr->data.warehouseID << "|" << curr->data.location << endl;
            curr = curr->next;
        }
        fout.close();
    }

    // File Handling: Load all warehouses from file
    void loadFromFile(const string& filename) {
        ifstream fin(filename);
        if (!fin) {
            // File might not exist (first run), that's okay
            return;
        }
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t sep = line.find('|');
            if (sep == string::npos) continue;
            int id = stoi(line.substr(0, sep));
            string loc = line.substr(sep+1);
            addWarehouse(Warehouse(id, loc));
        }
        fin.close();
    }
};

// -------------------- WMS Class --------------------
class WMS {
private:
    WarehouseLinkedList warehouses;
    const string warehouseFile = "warehouse.txt"; // your file name

public:
    WMS() {
        warehouses.loadFromFile(warehouseFile);
    }

    ~WMS() {
        warehouses.saveToFile(warehouseFile);
    }

    void addWarehouse(int id, string location) {
        if (warehouses.findWarehouse(id)) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tWarehouse ID already exists.\n";
            sleep(4);
            system("CLS");
            system("PAUSE");
            return;
        }
        warehouses.addWarehouse(Warehouse(id, location));
        warehouses.saveToFile(warehouseFile);
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\t\t\t\t\t\t\tWarehouse added successfully.\n";
        
        sleep(4); 
        system("CLS");
        system("PAUSE");
    }

    void addItemToWarehouse(int warehouseID, Item item) {
        WarehouseNode* node = warehouses.findWarehouse(warehouseID);
        if (!node) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tWarehouse not found.\n";
            sleep(3); 
            system("CLS");
            system("PAUSE");
            return;
        }
        node->data.addItem(item);
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\t\t\t\t\t\t\tItem added to Warehouse " << warehouseID << ".\n";
        sleep(3); 
        system("CLS");
        system("PAUSE");
    }

    void displayWarehouses() {
        warehouses.displayWarehouses();
    }

    void searchItem(int itemID) {
        WarehouseNode* curr = warehouses.head;
        while (curr) {
            Item* item = curr->data.searchItem(itemID);
            if (item) {
                cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
                cout << "\t\t\t\t\t\t\tItem found in Warehouse ID: " << curr->data.warehouseID << endl;
                item->displayItem();
                sleep(3);
                system("CLS");
                system("PAUSE");
                return;
            }
            curr = curr->next;
        }
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\t\t\t\t\t\t\tItem not found.\n";
        sleep(3);         
        system("CLS");
        system("PAUSE");
    }

    void deleteItem(int warehouseID, int itemID) {
        WarehouseNode* node = warehouses.findWarehouse(warehouseID);
        if (!node) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tWarehouse not found.\n";
            sleep(3); 
            system("CLS");
            system("PAUSE");
            return;
        }
        bool success = node->data.deleteItem(itemID);
        if (success) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tItem deleted successfully.\n";
            sleep(3); 
            
            system("CLS");
            system("PAUSE");
        } else {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tItem not found in the specified warehouse.\n";
            sleep(3); 
            system("CLS");
            system("PAUSE");
        }
    }

    void deleteWarehouse(int id) {
        bool success = warehouses.deleteWarehouse(id);
        if (success) {
            warehouses.saveToFile(warehouseFile);
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tWarehouse deleted successfully.\n";
            sleep(3); 
            system("CLS");
            system("PAUSE");
        } else {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tWarehouse not found.\n";
            sleep(3); 
            system("CLS");
            system("PAUSE");
        }
    }

    void transferItem(int fromWarehouseID, int toWarehouseID, int itemID, int quantity) {
        WarehouseNode* fromNode = warehouses.findWarehouse(fromWarehouseID);
        WarehouseNode* toNode = warehouses.findWarehouse(toWarehouseID);

        if (!fromNode || !toNode) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tOne or both warehouses not found.\n";
            sleep(5); 
            system("CLS");
            system("PAUSE");
            return;
        }

        Item* item = nullptr;
        for (auto& i : fromNode->data.items) {
            if (i.itemID == itemID) {
                item = &i;
                break;
            }
        }

        if (!item || item->quantity < quantity) {
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tItem not found or insufficient quantity in source warehouse.\n";
            sleep(3); 
           
            system("CLS");
            system("PAUSE");
            return;
        }

        item->quantity -= quantity;
        Item newItem(itemID, item->name, item->category, quantity, item->price);
        toNode->data.addItem(newItem);
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\t\t\t\t\t\t\tItem transferred successfully.\n";
        sleep(3); 
        system("CLS");
        system("PAUSE");
    }
};

// -------------------- Main Function --------------------
int main() {
    WMS wms;
    int choice;

    do {
        cout << "\n\n";
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\t\t\t\t\t\t\t|                                                          |\n";
        cout << "\t\t\t\t\t\t\t|               WAREHOUSE MANAGEMENT SYSTEM                |\n";
        cout << "\t\t\t\t\t\t\t|                                                          |\n";
        cout << "\t\t\t\t\t\t\t|----------------------------------------------------------|\n";
        cout << "\t\t\t\t\t\t\t|                                                          |\n";
        cout << "\t\t\t\t\t\t\t|\t1. Add Warehouse                                   |\n";
        cout << "\t\t\t\t\t\t\t|\t2. Add Item to Warehouse                           |\n";
        cout << "\t\t\t\t\t\t\t|\t3. Display Warehouses                              |\n";
        cout << "\t\t\t\t\t\t\t|\t4. Search Item                                     |\n";
        cout << "\t\t\t\t\t\t\t|\t5. Delete Item                                     |\n";
        cout << "\t\t\t\t\t\t\t|\t6. Transfer Item Between Warehouses                |\n";
        cout << "\t\t\t\t\t\t\t|\t7. Delete Warehouse                                |\n";
        cout << "\t\t\t\t\t\t\t|\t8. Exit                                            |\n";
        cout << "\t\t\t\t\t\t\t|                                                          |\n";
        cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
        cout << "\n\n";
        cout << "\t\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;
    
        system("CLS");
        switch (choice) {
        case 1: {
            
            int id;
            string location;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t|Enter Warehouse ID:         ";
            cin >> id;
            cout << "\t\t\t\t\t\t\t|Enter Warehouse Location:   ";
            cin.ignore();
            getline(cin, location);
            wms.addWarehouse(id, location);
           
            break;
        }
        case 2: {
            int warehouseID, itemID, quantity;
            string name, category;
            double price;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tEnter Warehouse ID: ";
            cin >> warehouseID;
            cout << "\t\t\t\t\t\t\tEnter Item ID: ";
            cin >> itemID;
            cout << "\t\t\t\t\t\t\tEnter Item Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "\t\t\t\t\t\t\tEnter Item Category: ";
            getline(cin, category);
            cout << "\t\t\t\t\t\t\tEnter Item Quantity: ";
            cin >> quantity;
            cout << "\t\t\t\t\t\t\tEnter Item Price: ";
            cin >> price;
            Item newItem(itemID, name, category, quantity, price);
            wms.addItemToWarehouse(warehouseID, newItem);
          
            break;
        }
        case 3:
            wms.displayWarehouses();
            break;
        case 4: {
            int itemID;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tEnter Item ID to Search: ";
            cin >> itemID;
            wms.searchItem(itemID);
            break;
        }
        case 5: {
            int warehouseID, itemID;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "t\t\t\t\t\t\tEnter Warehouse ID: ";
            cin >> warehouseID;
            cout << "t\t\t\t\t\t\tEnter Item ID to Delete: ";
            cin >> itemID;
            wms.deleteItem(warehouseID, itemID);
            break;
        }
        case 6: {
            int fromID, toID, itemID, quantity;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\tEnter Source Warehouse ID: ";
            cin >> fromID;
            cout << "\t\t\t\t\t\tEnter Destination Warehouse ID: ";
            cin >> toID;
            cout << "\t\t\t\t\t\tEnter Item ID: ";
            cin >> itemID;
            cout << "\t\t\t\t\t\tEnter Quantity to Transfer: ";
            cin >> quantity;
            wms.transferItem(fromID, toID, itemID, quantity);
            
            break;
        }
        case 7: {
            int warehouseID;
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tEnter Warehouse ID to Delete: ";
            cin >> warehouseID;
            wms.deleteWarehouse(warehouseID);
            break;
        }
        case 8:
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tExiting system. Goodbye!\n";
            break;
        default:
            cout << "\n\n";
            cout << "\t\t\t\t\t\t\t////////////////////////////////////////////////////////////\n";
            cout << "\t\t\t\t\t\t\tInvalid choice. Please try again.\n";
            // system("PAUSE");
           
        }
    } while (choice != 8);
    

    return 0;
}
