#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"

//==============================================================================================================================

struct Product
{
    string name;
    unsigned int price;
    unsigned int inventory;
    unsigned int sold = 0;
};

//==============================================================================================================================

class Warehouse
{
    vector<Product> stuff;

    //O(log n)
    bool binarySearch(const string& name, size_t& pos) const
    {
        int left = 0;
        int right = (int)stuff.size() - 1;

        while (left <= right)
        {
            int mid = left + (right - left) / 2;

            if (stuff[mid].name == name)
            {
                pos = mid;
                return true;
            }
            else if (stuff[mid].name < name)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }

        pos = left;
        return false;
    }

    //------------------------------------------------------------------------------------------------------------------------

public:

    //O(n)
    bool record(const Product& p)
    {
        size_t pos;

        if (binarySearch(p.name, pos))
        {
            cout << RED << "Failed: '" << p.name << "' already exists!" << RESET << endl;
            return false;
        }

        stuff.insert(stuff.begin() + pos, p);
        cout << GREEN << "Recorded: '" << p.name << "'" << RESET << endl;
        return true;
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(n)
    void eliminate(string name)
    {
        size_t pos;
        if (!binarySearch(name, pos))
        {
            cout << RED << "Failed: '" << name << "' not found!" << RESET << endl;
            return;
        }

        stuff.erase(stuff.begin() + pos);
        cout << GREEN << "Deleted: '" << name << "'" << RESET << endl;
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(log n)
    void probe(const string& name)
    {
        size_t pos;
        if (!binarySearch(name, pos))
        {
            cout << RED << "Failed: '" << name << "' not found!" << RESET << endl;
            return;
        }

        cout << BLUE << "---- Product Info ----" << RESET << endl;
        cout << "Name: " << stuff[pos].name << endl;
        cout << "Price: " << stuff[pos].price << endl;
        cout << "Inventory: " << stuff[pos].inventory << endl;
        cout << "Sold: " << stuff[pos].sold << endl;
    }
    //------------------------------------------------------------------------------------------------------------------------

    //O(log n)
    void change(string name)
    {
        size_t pos;
        if (!binarySearch(name, pos))
        {
            cout << RED << "Failed: '" << name << "' not found!" << RESET << endl;
            return;
        }

        cout << GREEN << "Product found: '" << stuff[pos].name << "'" << RESET << endl;

        cout << "Enter new price: ";
        cin >> stuff[pos].price;

        cout << "Enter new inventory: ";
        cin >> stuff[pos].inventory;

        cout << GREEN << "Updated successfully!" << RESET << endl;
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(log n)
    bool hasProduct(const string& name) const
    {
        size_t pos;
        return binarySearch(name, pos);
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(log n)
    bool deliver(const string& name, unsigned int qty)
    {
        size_t pos;
        if (!binarySearch(name, pos))
            return false;

        if (stuff[pos].inventory < qty)
            return false;

        stuff[pos].inventory -= qty;
        stuff[pos].sold += 1;
        return true;
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(n)
    void makeProductsCopy(vector<Product>& copy) const
    {
        copy.clear();
        copy.reserve(stuff.size());

        for (const auto& p : stuff)
        {
            copy.push_back(p);
        }
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(n)
    void show()
    {
        if (stuff.empty())
        {
            cout << YELLOW << "Warehouse is empty!" << RESET << endl;
            return;
        }

        cout << BLUE << "---- All Products ----" << RESET << endl;

        for (const auto& p : stuff)
        {
            cout << "Name: " << p.name<< " | Inventory: " << p.inventory << endl;
        }
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(n)
    void filter()
    {
        if (stuff.empty())
        {
            cout << YELLOW << "Warehouse is empty!" << RESET << endl;
            return;
        }

        unsigned int minPrice, maxPrice;

        cout << "Enter min price: ";
        cin >> minPrice;

        cout << "Enter max price: ";
        cin >> maxPrice;

        if (minPrice > maxPrice)
        {
            cout << RED << "Invalid range! minPrice must be <= maxPrice" << RESET << endl;
            return;
        }

        cout << BLUE << "---- Filtered Products (" << minPrice << " to " << maxPrice << ") ----" << RESET << endl;

        bool found = false;

        for (const auto& p : stuff)
        {
            if (p.price >= minPrice && p.price <= maxPrice)
            {
                cout << "Name: " << p.name<< " | Price: " << p.price<< " | Inventory: " << p.inventory << endl;
                found = true;
            }
        }

        if (found == false)
        {
            cout << YELLOW << "No products found in this price range." << RESET << endl;
        }
    }
};

//==============================================================================================================================

class Admin
{
private:
    Warehouse warehouse;

public:

    //O(1)
    Warehouse& getWarehouse()
    {
        return warehouse;
    }

    //------------------------------------------------------------------------------------------------------------------------

    bool record(const Product& p)
    {
        return warehouse.record(p);
    }

    //------------------------------------------------------------------------------------------------------------------------

    void change(const string& name)
    {
        warehouse.change(name);
    }

    //------------------------------------------------------------------------------------------------------------------------

    void eliminate(const string& name)
    {
        warehouse.eliminate(name);
    }

    //------------------------------------------------------------------------------------------------------------------------

    void probe(const string& name)
    {
        warehouse.probe(name);
    }

    //------------------------------------------------------------------------------------------------------------------------

    void filter()
    {
        warehouse.filter();
    }

    //------------------------------------------------------------------------------------------------------------------------

    void show()
    {
        warehouse.show();
    }
};

//==============================================================================================================================

struct OrderItem
{
    string name;
    unsigned int quantity;
};

//==============================================================================================================================

class Order
{
    queue <OrderItem> orders;

public:

    //O(log n)
    void orderRegistration(Warehouse& wh)
    {
        OrderItem item;

        cout << "Enter product name: "<< endl;
        cin >> item.name;

        cout << "Enter quantity: "<< endl;
        cin >> item.quantity;

        if (!wh.deliver(item.name, item.quantity))
        {
            cout << RED << "Failed: Not enough stock OR product not found!"<< RESET << endl;
            return;
        }

        orders.push(item);

        cout << GREEN << "Order registered successfully: "<< item.name << " x" << item.quantity<< RESET << endl;
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(m)
    void processOrder(Warehouse& wh)
    {
        if (orders.empty())
        {
            cout << YELLOW << "No orders to process!" << RESET << endl;
            return;
        }

        while (!orders.empty())
        {
            OrderItem item = orders.front();
            orders.pop();

            cout << GREEN << "Delivered: " << item.name<< " x" << item.quantity << RESET << endl;
        }
    }

    //------------------------------------------------------------------------------------------------------------------------

    //O(n^2)
    void showFavoriteProduct(const Warehouse& wh)
    {
        vector<Product> products;
        wh.makeProductsCopy(products);

        if (products.empty())
        {
            cout << YELLOW << "Warehouse is empty!" << RESET << endl;
            return;
        }

        for (int i = 0; i < (int)products.size(); i++)
        {
            for (int j = 0; j < (int)products.size() - i - 1; j++)
            {
                if (products[j].sold < products[j + 1].sold)
                {
                    Product temp = products[j];
                    products[j] = products[j + 1];
                    products[j + 1] = temp;
                }
            }
        }

        cout << BLUE << "---- Favorite Products (SOLD) ----" << RESET << endl;

        for (const auto& p : products)
        {
            cout << "Name: " << p.name << " | Sold: " << p.sold << endl;
        }
    }


};

//==============================================================================================================================

int main()
{
    Admin admin;
    Order orderSystem;

    while (true)
    {
        string mode;
        cout << BLUE
             << "\nSelect Mode:\n"
             << "1) admin\n"
             << "2) user\n"
             << "3) exit\n"
             << RESET;
        cin >> mode;

        if (mode == "admin" || mode == "1")
        {
            string password, cmd;
            cout << "enter your password :" << endl;
            cin >> password;

            if (password != "admin123")
            {
                cout << RED << "Wrong password! Returning to main menu.\n" << RESET;
                continue;
            }

            while (true)
            {
                cout << BLUE
                     << "\n(Admin) what your order ? :\n"
                     << "1) record\n"
                     << "2) eliminate\n"
                     << "3) change\n"
                     << "4) deliver\n"
                     << "5) show\n"
                     << "6) filter\n"
                     << "7) back\n"
                     << RESET;
                cin >> cmd;

                if (cmd == "record" || cmd == "1")
                {
                    Product p;
                    cout << "Enter name: ";
                    cin >> p.name;
                    cout << "Enter price: ";
                    cin >> p.price;
                    cout << "Enter inventory: ";
                    cin >> p.inventory;

                    admin.record(p);
                }
                else if (cmd == "eliminate" || cmd=="2")
                {
                    string name;
                    cout << "Enter product name to delete: ";
                    cin >> name;
                    admin.eliminate(name);
                }
                else if (cmd == "change" || cmd=="3")
                {
                    string name;
                    cout << "Enter product name to change: ";
                    cin >> name;
                    admin.change(name);
                }
                else if (cmd == "deliver" || cmd=="4")
                {
                    orderSystem.processOrder(admin.getWarehouse());
                }
                else if (cmd == "show" || cmd=="5")
                {
                    admin.show();
                }
                else if (cmd == "filter" || cmd=="6")
                {
                    admin.filter();
                }
                else if (cmd == "back" || cmd=="7")
                {
                    cout << YELLOW << "Returning to main menu...\n" << RESET;
                    break;
                }
                else
                {
                    cout << YELLOW << "Unknown command!\n" << RESET;
                }
            }
        }
        //------------------------------------------------------------------------------------------------------------------------
        else if (mode == "user" || mode == "2")
        {
            string cmd;

            while (true)
            {
                cout << BLUE
                     << "\n(User) what your order ? :\n"
                     << "1) order\n"
                     << "2) deliver\n"
                     << "3) favorite\n"
                     << "4) back\n"
                     << RESET;
                cin >> cmd;

                if (cmd == "order" || cmd=="1")
                {
                    orderSystem.orderRegistration(admin.getWarehouse());
                }
                else if (cmd == "deliver" || cmd=="2")
                {
                    orderSystem.processOrder(admin.getWarehouse());
                }
                else if (cmd == "favorite" || cmd=="3")
                {
                    orderSystem.showFavoriteProduct(admin.getWarehouse());
                }
                else if (cmd == "back" || cmd=="5")
                {
                    cout << YELLOW << "Returning to main menu...\n" << RESET;
                    break; // ✅ برگشت به منوی اصلی
                }
                else
                {
                    cout << YELLOW << "Unknown command!\n" << RESET;
                }
            }
        }
        //------------------------------------------------------------------------------------------------------------------------
        else if (mode == "exit" || mode == "3")
        {
            cout << GREEN << "have good day !" << RESET << endl;
            break;
        }
        else
        {
            cout << YELLOW << "Invalid mode! Try again.\n" << RESET;
        }
    }

    return 0;
}

