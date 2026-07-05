#include <iostream>
#include <unordered_map>
using namespace std;

class Node {
    public:
    int key;
    int value;
    Node* next;
    Node* previous;

    Node(int keyData, int keyValue){
        this->key = keyData;
        this->value = keyValue;
        this->next = NULL;
        this->previous = NULL;
    }

    ~Node(){}
};

void printCache(Node* head){
    cout << "Printing cache" << endl;
    Node* temp = head;
    while(temp != NULL){
        cout << "{" << temp->key << ", " << temp->value << "}" << " ";
        temp = temp->next;
    }
    cout << endl;
}

class LRU_Cache {
    private: 
    int cacheSize;
    Node* head;
    Node* tail;
    unordered_map<int, Node*> cacheStorage;

    public:
    LRU_Cache(int size){
        this->cacheSize = size;
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->previous = head;
    }

    int getCacheSize(){
        return cacheSize;
    }

    void addNodeToHead(Node* node){
        node->next = head->next;
        node->previous = head;
        head->next->previous = node;
        head->next = node;
    }

    void removeNode(Node* node){
        node->previous->next = node->next;
        node->next->previous = node->previous;
    }

    void moveToHead(Node* node){
        removeNode(node);
        addNodeToHead(node);
    }

    int GET(int key){
        auto checkIfKeyPresent = cacheStorage.find(key);
        if(checkIfKeyPresent == cacheStorage.end()){
            return -1;
        }
        else {
            Node* valueFromCache = cacheStorage[key];
            moveToHead(valueFromCache);
            return valueFromCache->value;
        }
    }

    void PUT(int key, int value){
        auto checkIfKeyPresent = cacheStorage.find(key);

        if(checkIfKeyPresent == cacheStorage.end()){
            if(cacheStorage.size() == cacheSize){
                Node* lru = tail->previous;

                removeNode(lru);
                cacheStorage.erase(lru->key);
                delete lru;
            }
            Node* newNode = new Node(key, value);
            cacheStorage[key] = newNode;
            addNodeToHead(newNode);
        }
        else{
            Node* existingNode = checkIfKeyPresent->second;
            existingNode->value = value;
            moveToHead(existingNode);
        }
    }
};

int main() {
    LRU_Cache* cache = new LRU_Cache(2);

    cout << "--- Running Tests ---" << endl;
    cache->PUT(1, 10);
    cache->PUT(2, 20);
    cout << "Get 1 (Expected: 10): " << cache->GET(1) << endl; 

    cache->PUT(3, 30); // Evicts key 2
    cout << "Get 2 (Expected: -1): " << cache->GET(2) << endl; 
    cout << "Get 1 (Expected: 10): " << cache->GET(1) << endl; 

    cache->PUT(1, 100); // Updates key 1
    cache->PUT(4, 40);  // Evicts key 3
    cout << "Get 3 (Expected: -1): " << cache->GET(3) << endl; 
    cout << "Get 1 (Expected: 100): " << cache->GET(1) << endl; 
    cout << "Get 4 (Expected: 40): " << cache->GET(4) << endl; 
    
    return 0;
}