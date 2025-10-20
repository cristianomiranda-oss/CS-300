//============================================================================
// Name        : HashTable.cpp
// Author      : Cristiano Miranda
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
    size_t Size();
};

/**
 * Default constructor
 */
HashTable::HashTable() {   
    // Resizes the nodes array to the current tableSize var
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // Updates the tableSize var
    this->tableSize = size;
    // Resizes the nodes array to the passed in size var
    nodes.resize(size);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // Removes all elements in the node starting from the beginning of the vector
    nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // Calculates the hash key using the current tableSize value
    return key % this->tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // Calls the hash function to create the hash key using the bid's id
        // The bid Id string is parsed into a integer and for use in the hash function
    unsigned int nodeKey = hash(atoi(bid.bidId.c_str()));
    
    // Points to the address of the element located at the nodeKey index
    Node* currNode = &(nodes[nodeKey]);

    // Checks if the currNode is pointing to a null value
    if (currNode == nullptr) {
        // Point to a new Node
        currNode = new Node(bid, nodeKey);
    }
    // Checks if the node is empty after a removal based on its key value
    else if (currNode->key == UINT_MAX) {
        // Set's currNode's bid and key property to their new values
        currNode->bid = bid;
        currNode->key = nodeKey;
    }
    else {
        // Iterates through the singularly linked list until an null pointer is reached
        while (currNode->next != nullptr) {
            // Moves to the next element in the singularly linked list
            currNode = currNode->next;
        }

        // Points the last element's next pointer to a new Node instantiated with the passed in bid and calculated key value
        currNode->next = new Node(bid, nodeKey);
    }
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // Iterates through the nodes vector
    for (int i = 0; i < nodes.size(); i++) {
        // Points to the Node located at the nodes vector's index
        Node* currNode = &(nodes[i]);

        // Checks that the node is not empty
        if (currNode != nullptr && currNode->key != UINT_MAX) {
            // Loops through the singularly linked list until a null pointer is reached
            while (currNode != nullptr) {
                // Sets the node's bid to a local var
                Bid bid = currNode->bid;
                // Prints the information associated with the bid object to the console
                cout << currNode->key << " | " << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
                    << bid.fund << endl;
                // Points to the next element in the singularly linked list
                currNode = currNode->next;
            }
        }
    }
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // Calls the hash function to create the hash key using the bid's id
        // The bid Id string is parsed into a integer and for use in the hash function 
    unsigned int nodeKey = hash(atoi(bidId.c_str()));

    // Initializes a pointer and points to the element located at the hash index
    Node* currNode = &(nodes[nodeKey]);

    // Checks if the pointer is not pointing to a null value and is not empty from deletion node
    if (currNode != nullptr && currNode->key != UINT_MAX) {
        // Checks if the next node is pointing to a null value and the node's bid id is equal to the bidId,
            // indicating that this node is the only element in the singularly linked list and it is the node to be deleted
        if (currNode->next == nullptr && currNode->bid.bidId == bidId) {
            // Set's the current node's key to UINT_MAX indicating it is empty from deletion
            currNode->key = UINT_MAX;
            // Exits the function
            return;
        }
        else {
            // Initialize a new prevNode pointer to point to the previous node in the singly linked list
            Node* prevNode = currNode;
            // Iterates the currNode to the next node in the singly linked list
            currNode = currNode->next;

            // Loops through the singularly linked list until the current node's pointer is null
            while (currNode != nullptr) {
                // Checks if the currNode's bidId is equal to the passed in bidId
                if (currNode->bid.bidId == bidId) {
                    // Initializes a local bid var
                    Bid bid = currNode->bid;
                    // Displays the soon to be removed bid's details to the screen
                    cout << currNode->key << " | " << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
                        << bid.fund << endl;
                    // Points the prevNode's next pointer to the currNode's next pointer
                    prevNode->next = currNode->next;
                    // Removes the currNode from the singly linked list
                    delete currNode;
                    // Displays a message indicating that the bid was removed
                    cout << "Removed Bid" << endl;
                    // Exits the function
                    return;
                }

                prevNode = currNode;
                currNode = currNode->next;
            }
        }
    }

    // Displays a message indicating the node was not found
    cout << "Associated node not found!" << endl;
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    // Initializes an local bid with an empty bid
    Bid bid = Bid();

    // Calls the hash function to create the hash key using the bid's id
        // The bid Id string is parsed into a integer and for use in the hash function 
    unsigned int nodeKey = hash(atoi(bidId.c_str()));

    // Initializes a pointer and points to the element located at the hash index
    Node* currNode = &(nodes[nodeKey]);

    // Checks if the currNode is not pointing to a null value and is not empty from deletion node
    if (currNode != nullptr && currNode->key != UINT_MAX) {
        // Loops through the singularly linked list until a null value is reached
        while (currNode != nullptr) {
            // Checks if the currNode's bidId is equal to the passed in bidId
            if (currNode->bid.bidId == bidId) {
                // Returns the node's bid
                return currNode->bid;
            }

            // Points to the next element in the singularly linked list
            currNode = currNode->next;
        }
    }

    // Returns the empty bid if the associated bid is not found.
    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98223";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable;

    Bid bid;
    bidTable = new HashTable();
    
    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable->Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            bidTable->Remove(bidKey);
            break;
        case 9:
            // breaks the switch statement if the exit value is entered
            break;
        default:
            // // Solution acquired from https://www.reddit.com/r/cpp_questions/comments/lt4g3r/stdcin_doesnt_work_after_entering_the_wrong_input/, 
            // // User Competitive-Sea-3788

            // Alerts the user an invalid input was entered
            cout << "Invalid Input" << endl;
            // Resets choice var
            choice = 0;
            // Clears the user input and ignores the last invalid input
            cin.clear();
            cin.ignore();
            break;
        }
        
    }

    cout << "Good bye." << endl;

    return 0;
}
