#include<bits/stdc++.h>


using namespace std;

class LRUCache {
    private:
        // pair-> key, value
        unordered_map<int, int> cache;
        int max_cap;
        // contains the recent keys
        list<int> lru;
    public:
        LRUCache(int capacity) 
        {
            cache.reserve(capacity);
            max_cap = capacity;
        }
    
    int get(int key) 
    {
        if(cache.count(key) > 0)
        {
            lru.remove(key);
            lru.push_back(key);
            return cache[key];
        }
        return -1;
    }
    
    void put(int key, int value) 
    {
        if(cache.size() < max_cap)
        {
            if(cache.count(key) <= 0)
                lru.push_back(key);
            else
            {
                lru.remove(key);
                lru.push_back(key);
            }
            cache[key] = value;
        }
        else
        {
            if(cache.count(key) > 0)
            {
                lru.remove(key);
                lru.push_back(key);
                cache[key] = value;
            }
            else
            {
                int old_key = lru.front();
                lru.pop_front();
                cache.erase(old_key);
                lru.push_back(key);
            }
            cache[key] = value;
        }
        
    }
};

int main(int argc, char const *argv[])
{
    LRUCache cache = LRUCache( 2 /* capacity */ );

    cout << cache.get(2) << endl;
    cache.put(2,6);
    cout << cache.get(1) << endl;
    cache.put(1,5);
    cache.put(1,2);
    cout << cache.get(1) << endl;
    cout << cache.get(2) << endl;
    return 0;
}