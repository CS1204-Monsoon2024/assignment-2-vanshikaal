#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

double LOAD_FACTOR_THRESHOLD = 0.8;

// Hash table class
class HashTable {
public:
    int* table;
    bool* deleted;
    int capacity;
    int size;

    // Helper function to check if a number is prime
    bool isPrime(int n) {
        if (n <= 1) return false;
        if (n == 2 || n == 3) return true;
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i * i <= n; i += 6) {
            if (n % i == 0 || n % (i + 2) == 0)
                return false;
        }
        return true;
    }

    // Get the next prime number greater than n
    int nextPrime(int n) {
        while (!isPrime(n)) {
            n++;
        }
        return n;
    }

    // Hash function
    int hash(int key) {
        return key % capacity;
    }

    // Resize the table when the load factor exceeds the threshold
    void resize() {
        int newCapacity = nextPrime(2 * capacity);
        int* newTable = new int[newCapacity];
        bool* newDeleted = new bool[newCapacity];

        for (int i = 0; i < newCapacity; i++) {
            newTable[i] = -1;
            newDeleted[i] = false;
        }

        for (int i = 0; i < capacity; i++) {
            if (table[i] != -1 && !deleted[i]) {
                int newHash = table[i] % newCapacity;
                int j = 0;
                while (newTable[(newHash + j * j) % newCapacity] != -1) {
                    j++;
                }
                newTable[(newHash + j * j) % newCapacity] = table[i];
            }
        }

        delete[] table;
        delete[] deleted;
        table = newTable;
        deleted = newDeleted;
        capacity = newCapacity;
    }

public:
    // Constructor
    HashTable(int initialCapacity) {
        capacity = nextPrime(initialCapacity);
        table = new int[capacity];
        deleted = new bool[capacity];
        size = 0;

        for (int i = 0; i < capacity; i++) {
            table[i] = -1;
            deleted[i] = false;
        }
    }

    // Insert a key into the hash table
    void insert(int key) {
        if (search(key) != -1) {
            std::cout << "Duplicate key insertion is not allowed" << endl;
            return;
        }

        if ((double)size / capacity >= LOAD_FACTOR_THRESHOLD) {
            resize();
        }

        int index = hash(key);
        int i = 0;
        while (table[(index + i * i) % capacity] != -1 && !deleted[(index + i * i) % capacity]) {
            i++;
            if (i >= capacity) {
                std::cout << "Max probing limit reached!" << endl;
                return;
            }
        }

        table[(index + i * i) % capacity] = key;
        deleted[(index + i * i) % capacity] = false;
        size++;
    }

    // Search for a key in the hash table
    int search(int key) {
        int index = hash(key);
        int i = 0;
        while (table[(index + i * i) % capacity] != -1 || deleted[(index + i * i) % capacity]) {
            if (table[(index + i * i) % capacity] == key) {
                return (index + i * i) % capacity;
            }
            i++;
            if (i >= capacity) return -1;
        }
        return -1;
    }

    // Remove a key from the hash table
    void remove(int key) {
        int index = search(key);
        if (index == -1) {
            std::cout << "Element not found" << endl;
        } else {
            table[index] = -1;
            deleted[index] = true;
            size--;
        }
    }

    // Print the hash table
    void printTable() {
        for (int i = 0; i < capacity; i++) {
            if (table[i] != -1 && !deleted[i]) {
                cout << table[i] << " ";
            } else {
                cout << "- ";
            }
        }
        cout << endl;
    }
};
