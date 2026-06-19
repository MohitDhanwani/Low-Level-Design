#include<iostream>
#include<vector>
#include"./Modals/Menu_Items.h"
using namespace std;

class Restaurant {

    private:
    static int restaurantId;
    string restaurantName;
    string restaurantLocation;
    vector<MenuItems> restaurantMenu;

    public:
    Restaurant(string& name, string& location){
        this->restaurantLocation = location;
        this->restaurantName = name;
        this->restaurantId = ++restaurantId;
    }

    //Getter and Setter
     string getName() const {
        return restaurantName;
    }

    void setName(const string &n) {
        restaurantName = n;
    }

    string getLocation() const {
        return restaurantLocation;
    }

    void setLocation(const string &loc) {
        restaurantLocation = loc;
    }

    void addMenuItem(const MenuItems &item) {
        restaurantMenu.push_back(item);
    }

    const vector<MenuItems>& getMenu() const {
        return restaurantMenu;
    }
};

int Restaurant::restaurantId = 0;