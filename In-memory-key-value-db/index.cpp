#include <iostream>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <string>
#include <memory>
using namespace std;

// 1. INTERFACE LAYER (Open/Closed Principle - OCP)
class IStorageEngine {
public:
    virtual pair<bool, string> GET(const string& key) = 0;
    virtual bool PUT(const string& key, const string& value) = 0;
    virtual bool DELETE(const string& key) = 0;
    virtual bool PATCH(const string& key, const string& newValue) = 0;
    virtual unordered_map<string, string> GETALL() = 0;
    virtual ~IStorageEngine() = default;
};

// 2. DATA STORAGE LAYER (Single Responsibility Principle - SRP)
// No console outputs (cout) here. Pure raw database operations.
class InMemoryStorageEngine : public IStorageEngine {
private:
    unordered_map<string, string> keyToValue;
    mutex mtx; // Thread safety lock

public:
    InMemoryStorageEngine() {
        cout << "In Memory Storage Engine Created..." << endl;
    }

    pair<bool, string> GET(const string& key) override {
        lock_guard<mutex> lock(mtx); // automatically releases the lock once function returns the value
        auto it = keyToValue.find(key);
        if (it != keyToValue.end()) {
            return {true, it->second};
        }
        return {false, ""};
    }

    bool PUT(const string& key, const string& value) override {
        lock_guard<mutex> lock(mtx);
        if (keyToValue.find(key) != keyToValue.end()) {
            return false; // Key already exists
        }
        keyToValue[key] = value;
        return true;
    }

    bool DELETE(const string& key) override {
        lock_guard<mutex> lock(mtx);
        auto it = keyToValue.find(key);
        if (it == keyToValue.end()) {
            return false; // Key not found
        }
        keyToValue.erase(it);
        return true;
    }

    bool PATCH(const string& key, const string& newValue) override {
        lock_guard<mutex> lock(mtx);
        auto it = keyToValue.find(key);
        if (it == keyToValue.end()) {
            return false; // Key not found to update
        }
        keyToValue[key] = newValue;
        return true;
    }

    unordered_map<string, string> GETALL() override {
        lock_guard<mutex> lock(mtx);
        return keyToValue; // Returns a thread-safe snapshot copy
    }
};

// 3. SERVICE / BUSINESS LOGIC LAYER (SRP & Dependency Injection)
// Handles validations, formatting, and console communication.
class KeyValueService {
private:

    // Smart Pointer Similar to = StorageEngine* redis = new StorageEngine(); BUT it automatically frees the memory we dont have to
    shared_ptr<IStorageEngine> storage; // Loose coupling through interface

public:
    KeyValueService(shared_ptr<IStorageEngine> engine) : storage(engine) {
        cout << "Key-Value Service Initialized with Storage Engine." << endl;
    }

    void handleGet(const string& key) {
        auto result = storage->GET(key);
        if (result.first) {
            cout << "GET Success | " << key << " -> " << result.second << endl;
        } else {
            cout << "GET Error   | Key '" << key << "' does not exist." << endl;
        }
    }

    void handlePut(const string& key, const string& value) {
        if (storage->PUT(key, value)) {
            cout << "PUT Success | " << key << " inserted cleanly." << endl;
        } else {
            cout << "PUT Error   | Key '" << key << "' already exists." << endl;
        }
    }

    void handleDel(const string& key) {
        if (storage->DELETE(key)) {
            cout << "DEL Success | Key '" << key << "' deleted." << endl;
        } else {
            cout << "DEL Error   | Cannot delete non-existent key '" << key << "'." << endl;
        }
    }

    void handlePatch(const string& key, const string& newValue) {
        if (storage->PATCH(key, newValue)) {
            cout << "PATCH Success | Key '" << key << "' updated securely." << endl;
        } else {
            cout << "PATCH Error   | Cannot update non-existent key '" << key << "'." << endl;
        }
    }

    void printAll() {
        auto dataSnapshot = storage->GETALL();
        cout << "Current DB Snapshot" << endl;
        if (dataSnapshot.empty()) {
            cout << "[Empty Database]" << endl;
        } else {
            for (const auto& pair : dataSnapshot) {
                cout << pair.first << " => " << pair.second << endl;
            }
        }
        cout << endl;
    }
};

int main() {
    // Injecting Concrete storage into Service via Interface Pointer
    shared_ptr<IStorageEngine> engine = make_shared<InMemoryStorageEngine>();
    shared_ptr<KeyValueService> redis = make_shared<KeyValueService>(engine);

    cout << "STARTING BASIC FUNCTIONALITY TESTS" << endl;
    redis->handlePut("user1", "{name: mohit, age:18}");
    redis->handlePut("user1", "{duplicate entry attempt}"); // Should fail 
    redis->handleGet("user1");
    redis->handlePatch("user1", "{name: mohit, age:19}");
    redis->handleGet("user1");
    redis->handleDel("user1");
    redis->handleGet("user1");
    
    cout << "SETTING UP CONCURRENCY TESTS" << endl;
    redis->handlePut("user_test", "{name: initial_state}");
    
    // Test 1: Simultaneous Writes
    cout << "TEST 1: Simultaneous Writes" << endl;
    thread t1(redis, "user_t1", "{data: dynamic_1}", "Thread_1");
    thread t2(redis, "user_t2", "{data: dynamic_2}", "Thread_2");
    t1.join();
    t2.join();

    // Test 2: Simultaneous Patches
    cout << "TEST 2: Simultaneous Updates (Same Key)" << endl;
    thread t3(redis, "user_test", "{name: altered_by_T3}", "Thread_3");
    thread t4(redis, "user_test", "{name: altered_by_T4}", "Thread_4");
    t3.join();
    t4.join();

    // Test 3: Simultaneous Read & Patch
    cout << "TEST 3: Simultaneous Read and Update" << endl;
    thread t5(redis, "user_test", "Thread_5_Reader");
    thread t6(redis, "user_test", "{name: complete_final_value}", "Thread_6_Patcher");
    t5.join();
    t6.join();

    cout << "VERIFYING ABSOLUTE FINAL DB STATE" << endl;
    redis->printAll();

    return 0;
}