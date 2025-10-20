//============================================================================
// Name        : LinkedList.cpp
// Author      : Cristiano Miranda
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Lab 3-2 Lists and Searching
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

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
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
    //Internal structure for list entries, housekeeping variables
    struct Node {
        Bid bid;
        Node *next;

        // default constructor
        Node() {
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) {
            bid = aBid;
            next = nullptr;
        }
    };

    Node* head;
    Node* tail;
    int size = 0;

public:
    LinkedList();
    virtual ~LinkedList();
    void Append(Bid bid);
    void Prepend(Bid bid);
    void PrintList();
    void Remove(string bidId);
    Bid Search(string bidId);
    int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
    // Points the head and tail of the list to null values
    head = nullptr;
    tail = nullptr;
}   

/**
 * Destructor
 */
LinkedList::~LinkedList() {
    // start at the head
    Node* current = head;
    Node* temp;

    // loop over each node, detach from list then delete
    while (current != nullptr) {
        temp = current; // hang on to current node
        current = current->next; // make current the next node
        delete temp; // delete the orphan node
    }
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
    // Initializes a new node pointer and sets it value to the passed in bid
    Node* newNode = new Node(bid);
    
    //Checks if the list is not populated
    if (head == nullptr) {
        // Sets the newNode as the head and tail of the list
        head = newNode;
        tail = newNode;
    }
    else {
        // Sets the current tail to point to the new node
        tail->next = newNode;
        // Sets the new node as the list's tail
        tail = newNode;
    }

    // Increments the size var
    size++;
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
    // Initializes a new node pointer and sets it value to the passed in bid
    Node* newNode = new Node(bid);

    //Checks if the list is not populated
    if (head == nullptr) {
        // Sets the newNode as the head and tail of the list
        head = newNode;
        tail = newNode;
    }
    else {
        // Sets the current head node to the new node's next pointer
        newNode->next = head;
        // Sets the new node as the head
        head = newNode;
    }

    // Increments the size var
    size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
    // Initializes a node pointer to the start of the list
    Node* currNode = head;

    // Loops until the currNode points to a null pointer
    while (currNode != nullptr) {
        // Initialize a var to store the current node's bid object
        Bid currBid = currNode->bid;

        // Prints the various bid properties to the console
        cout << currBid.bidId << ": " << currBid.title << " | " << currBid.amount
            << " | " << currBid.fund << endl;

        // Sets the current node pointer to the next node in the list
        currNode = currNode->next;
    }
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
    // Initializes a new pointer and sets it to the list's head
    Node* currNode = head;

    // Checks if the currNode is not null and if the head's bid id value equals the passed in bid id
    if (currNode != nullptr && currNode->bid.bidId == bidId) {
        // Initializes a pointer to temporarily store the next list item
        Node* sucNode = head->next;

        // Calls the function to remove the current head node from memory
        delete currNode;

        // Sets the head of the list to the next list item
        head = sucNode;

        // Checks if the next list item is null,
        // which indicates that the list is now empty
        if (sucNode == nullptr) {
            // Sets the tail to null
            tail = nullptr;
        }

        // Decrements the size variable
        size--;
        // Exit the method
        return;
    }

    // Loops through the list until a null value is reached
    while (currNode != nullptr) {
        // Checks if the next item is not null and if the next item's bid id is equal to the passed in bid id
        if (currNode->next != nullptr && currNode->next->bid.bidId == bidId) {
            // Initializes a pointer to temporarily store the next node after the next node
            Node* sucNode = currNode->next->next;

            // Calls the function to remove the next node from memory
            delete currNode->next;
            
            // Sets the current node's next pointer to point to the next node after the next node
            currNode->next = sucNode;

            // Checks if the next node is null,
            // which would indicate that the current node is now the tail of the list
            if (sucNode == nullptr) {
                // Sets the tail to the currNode
                tail = currNode;
            }

            // Decrements the size variable
            size--;
            // Exits the method
            return;
        }

        // Iterates through the list by moving to the next node
        currNode = currNode->next;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
    // Initializes a new pointer and sets it to the list's head
    Node* currNode = head;

    // Checks if the head is not null and if the head's bid id value equals the passed in bid id
    if (currNode != nullptr && currNode->bid.bidId == bidId) {
        // Returns the associated bid
        return currNode->bid;
    }

    // Loops through the list until a null value is reached
    while (currNode != nullptr) {
        // Checks if the current node's bid id is equal to the passed in id
        if (currNode->bid.bidId == bidId) {
            // Returns if the current bid
            return currNode->bid;
        }
        else {
            // Iterates through the list by moving to the next node
            currNode = currNode->next;
        }
    }

    // Initializes a new empty bid
    Bid emptyBid = Bid();
    // Returns the emptyBid if the bidId is not found
    return emptyBid;
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
    return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount
         << " | " << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    cout << "Enter title: ";
    getline(cin, bid.title);

    cout << "Enter fund: ";
    cin >> bid.fund;

    cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // initialize a bid using data from current row (i)
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

            // add this bid to the end
            list->Append(bid);
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
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
        bidKey = "98109";
    }

    clock_t ticks;

    LinkedList bidList;

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Enter a Bid" << endl;
        cout << "  2. Load Bids" << endl;
        cout << "  3. Display All Bids" << endl;
        cout << "  4. Find Bid" << endl;
        cout << "  5. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bid = getBid();
            bidList.Append(bid);
            displayBid(bid);

            break;

        case 2:
            ticks = clock();

            loadBids(csvPath, &bidList);

            cout << bidList.Size() << " bids read" << endl;

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " milliseconds" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 3:
            bidList.PrintList();

            break;

        case 4:
            ticks = clock();

            bid = bidList.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            }
            else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 5:
            bidList.Remove(bidKey);

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
