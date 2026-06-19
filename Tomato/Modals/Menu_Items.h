#include<iostream>
#include<vector>
using namespace std;

class MenuItems {
    private:
    string code;
    string name;
    int price;

    public:
    MenuItems(string& code, string& name, int& price){
        this->code = code;
        this->name = name;
        this->price = price;
    }

    //Getter
    string getCode(){
        return code;
    }

    string getName(){
        return name;
    }

    int getPrice(){
        return price;
    }

    //Setter
    void setCode(string& code){
        code = code;
    }

    void setName(string& name){
        name = name;
    }

    void setPrice(int& price){
        price = price;
    }
};