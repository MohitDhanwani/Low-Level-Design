#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

/*
user => userid, userSubscriptionid, subscriptonDate
plan => planId, monthylPrice
*/

class SubscriptionPlanDetails {
private:
    static int nextPlanId;
    int planId;
    int subscriptionPrice;
    int subscriptionDuration;
    
public:
    SubscriptionPlanDetails() {
        planId = -1;
        subscriptionPrice = 0;
        subscriptionDuration = 0;
    }
    
    SubscriptionPlanDetails(int price, int duration){
        planId = ++nextPlanId;
        this->subscriptionPrice = price;
        this->subscriptionDuration = duration;
        cout << "Subscription created for time " << subscriptionDuration << " days and price is " << subscriptionPrice << endl;
    }
    
    //getters
    int getPlanId(){
        return this->planId;
    }
    
    int getSubscriptionPrice(){
        return this->subscriptionPrice;
    }
    
    int getSubscriptionDuration(){
        return this->subscriptionDuration;
    }
    
    
};

class UserDetails {
private:
    static int nextUserId;
    int userId;
    bool hasAnyActiveSubscription;
    int userPlanSubscriptionId;
    string subscriptionStartDate;

public:
    UserDetails() {
        userId = ++nextUserId;
        this->hasAnyActiveSubscription = false;
        this->userPlanSubscriptionId = -1;
        this->subscriptionStartDate = "";
    }

    // getters
    int getUserId() {
        return this->userId;
    }

    bool subscriptonOwningStatus() {
        return this->hasAnyActiveSubscription;
    }

    int getUserSubscriptionId() {
        return this->userPlanSubscriptionId;
    }

    string getSubscriptionStartDate() {
        return this->subscriptionStartDate;
    }
};

class UserSubscriptionManager { // todo -> make this singleton so that we do not end up creating its instance everytime
private:
    unordered_map<int, SubscriptionPlanDetails> UserSubscriptinInformation;
    
public:
    UserSubscriptionManager(){
        cout << "Created a new user subscription manager and info size is " << UserSubscriptinInformation.size() << endl;
    }
    
    //getters 
    SubscriptionPlanDetails getUserSubscriptionStatus(int userId){
        auto it = UserSubscriptinInformation.find(userId);
        
        if (it == UserSubscriptinInformation.end()) {
            cout << "No active subscription present for the user." << endl;
            return SubscriptionPlanDetails(); // Returns a default object
        }
    
        // If found, print the active subscription details here
        cout << "User subscription details -> Plan ID: " << it->second.getPlanId() 
             << ", Price: " << it->second.getSubscriptionPrice() 
             << ", Duration: " << it->second.getSubscriptionDuration() << " days" << endl;
             
        return it->second;
    }
    
    //setters
    void setUserSubscriptionInfo(int userId, SubscriptionPlanDetails temp){
        
        if(UserSubscriptinInformation.find(userId) != UserSubscriptinInformation.end()){
            cout << "User currently has an active subscription running and its info is " 
            << UserSubscriptinInformation[userId].getPlanId() << " "
            << UserSubscriptinInformation[userId].getSubscriptionPrice() << " "
            << UserSubscriptinInformation[userId].getSubscriptionDuration() << endl;
        }
        else {
            cout << "No active subscritpion present for the user, creating a new one" << endl;
            UserSubscriptinInformation[userId] = temp;
            cout << "Subscription created!" << endl;
        }
    }
};

int SubscriptionPlanDetails::nextPlanId = 0;
int UserDetails::nextUserId = 0;

int main() {
    UserDetails* user1 = new UserDetails();
    UserDetails* user2 = new UserDetails();
    UserDetails* user3 = new UserDetails();
    
    SubscriptionPlanDetails* sp1 = new SubscriptionPlanDetails(99, 7);
    SubscriptionPlanDetails* sp2 = new SubscriptionPlanDetails(199, 30);
    SubscriptionPlanDetails* sp3 = new SubscriptionPlanDetails(499, 180);
    
    UserSubscriptionManager* manager = new UserSubscriptionManager();
    
    cout << "finding status for user 1 " << endl;
    manager->getUserSubscriptionStatus(user1->getUserId());
    
    cout << "setting status for user 1 " << endl;
    manager->setUserSubscriptionInfo(user1->getUserId(), *sp1);
    
    cout << "finding status for user 1 after subscription" << endl;
    manager->getUserSubscriptionStatus(user1->getUserId());
    
    cout << "setting status for user 1 " << endl;
    manager->setUserSubscriptionInfo(user1->getUserId(), *sp1);
    
    cout << "finding status for user 2 " << endl;
    manager->getUserSubscriptionStatus(user2->getUserId());
    
    cout << "setting status for user 2 " << endl;
    manager->setUserSubscriptionInfo(user2->getUserId(), *sp2);
    
    cout << "finding status for user 2 after subscription" << endl;
    manager->getUserSubscriptionStatus(user2->getUserId());

    return 0;
}