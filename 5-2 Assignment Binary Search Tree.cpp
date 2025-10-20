//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Cristiano Miranda
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 5-2 Binary Search Tree
//============================================================================

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

// Internal structure for tree node
struct Node {
    Bid bid;
    Node *left;
    Node *right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a bid
    Node(Bid aBid) :
            Node() {
        bid = aBid;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Bid bid);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    Node* removeNode(Node* node, string bidId);
    void printNode(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void PostOrder();
    void PreOrder();
    void Insert(Bid bid);
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Points the start of the tree, the root, to a null value
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {

    Node* currNode = root;

    while (currNode != nullptr) {
        currNode = removeNode(currNode, currNode->bid.bidId);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // Call the inOrder recursive function and pass in the root to begin traversal
    inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // Call the postOrder recursive function and pass in the root to begin traversal
    postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // Call the preOrder recursive function and pass in the root to begin traversal
    preOrder(root);
}



/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
    // Check if the tree's root is empty
    if (root == nullptr) {
        // Initialize a new node and assign it to the tree's root
        root = new Node(bid);
    }
    else {
        // Call the addNode recursive function and pass in the root and new bid to-be added
        addNode(root, bid);
    }
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
    // Call the removeNode recursive function and pass in the root and id of node to-be removed
    removeNode(root, bidId);
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
    // Initialize a new pointer pointing to the tree's root
    Node* currNode = root;

    // Traverse the tree until the currNode is null
    while (currNode != nullptr) {
        // Check if the currNode's bid matches the passed in bid id
        if (currNode->bid.bidId == bidId) { // Indicates the desired bid was found
            // Exit the method and return the currNode's bid
            return currNode->bid;
        }
        // Check if the currNode's bid id is greater than the passed in bid id
        else if (currNode->bid.bidId > bidId) { // Indicates the left child should be searched
            // Navigate to the left child
            currNode = currNode->left;
        }
        else { // Indicates the right child should be searched=
            // Navigate to the right child
            currNode = currNode->right;
        }
    }
    
    // Initialize an empty bid
    Bid bid;
    // Exit the function and return the empty bid, indicating the bid id has no associated node in the tree
    return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
    // Checks if the current node's bid id is greater than the passed in bid's id
    if (node->bid.bidId > bid.bidId) { // Indicates the new bid's id is less than the current node's bid id and needs to be assigne to the left
        // Checks if the current node's left child is unoccupied
        if (node->left == nullptr) {
            // Initializes a new node using the passed in bid and assigns it to the left child node
            node->left = new Node(bid);
        }
        else { // Indicates that the left child is occupied
            // Recursively call the addNode method to traverse to the left child
            addNode(node->left, bid);
        }
    }
    else { // Indicates new bid's id is greater than or equal to the current node's bid id and needs be assigned to the right
        // Checks if the current node's right child is unoccupied
        if (node->right == nullptr) {
            // Initializes a new node using the passed in bid and assigns it to the right child node
            node->right = new Node(bid);
        }
        else {
            // Recursively call the addNode method to traverse to the right child
            addNode(node->right, bid);
        }
    }
}

void BinarySearchTree::inOrder(Node* node) {
    // Exit the method once a null value is reached
    if (node == nullptr) {
        return;
    }

    // Recursively call to visit the node's left child
    inOrder(node->left);
    // Call the display function to output the node's bid info
    printNode(node);
    // Recursively call to visit the node's right child
    inOrder(node->right);
}

void BinarySearchTree::postOrder(Node* node) {
    // Exit the method once a null value is reached
    if (node == nullptr) {
        return;
    }

    // Recursively call to visit the node's left child
    postOrder(node->left);
    // Recursively call to visit the node's right child
    postOrder(node->right);
    // Call the display function to output the node's bid info
    printNode(node);
}

void BinarySearchTree::preOrder(Node* node) {
    // Exit the method once a null value is reached
    if (node == nullptr) {
        return;
    }

    // Call the display function to output the node's bid info
    printNode(node);
    // Recursively call to visit the node's left child
    preOrder(node->left);
    // Recursively call to visit the node's right child
    preOrder(node->right);
}

/**
 * Remove a bid from some node (recursive)
 */
Node* BinarySearchTree::removeNode(Node* node, string bidId) {
    // Check if the passed in pointer is null
    if (node == nullptr) {
        // Exit the function as the bid id was not found
        return node;
    }
    // Check if the current node's bid id is less than the passed in bidId
    else if (node->bid.bidId > bidId) { // Indicates that the left child needs to be searched
        // Recursively call the removeNode method with the current node's left child
            // The returned node will become the current node's new left child upon finding the to-be removed node
        node->left = removeNode(node->left, bidId);
    }
    // Check if the currNode's bid id is greater than the passed in bidId
    else if (node->bid.bidId < bidId) { // Indicates that the right child needs to be searched
        // Recursively call the removeNode method with the current node's right child
            // The returned node will become the current node's new right child upon finding the to-be removed node
        node->right = removeNode(node->right, bidId);
    }
    else { // Indicates that the current node is to be removed
        // Check if the node has any child nodes 
        if (node->left == nullptr && node->right == nullptr) { // Indicates that it is a leaf node and can be removed
            // Delete the node
            delete node;
            // Point node to null
            node = nullptr;
        }
        // Check if the left child is occupied
        else if (node->left != nullptr && node->right == nullptr) { // Indicates the left child can be assigned to the current node
            // Initializes a temporary var to hold to-be removed node
            Node* removedNode = node;
            // Assigns the current node's left child to the current node
            node = node->left;
            // Frees the to-be removed node from memory
            delete removedNode;
            // Points the temporary var to null
            removedNode = nullptr;
        }
        // Check if the right child is occupied
        else if (node->left == nullptr && node->right != nullptr) { // Indicates the right child can be assigned to the current node
            // Initializes a temporary var to hold to-be removed node
            Node* removedNode = node;
            // Assigns the current node's left child to the current node
            node = node->right;
            // Frees the to-be removed node from memory
            delete removedNode;
            // Points the temporary var to null
            removedNode = nullptr;
        }
        else { // Indicates both children are occupied, and the successive node must be found
            // Initializes a pointer pointing to the current node's right child
            Node* succNode = node->right;
            // Traverses the tree until the current node's successive node is found
            while (succNode->left != nullptr) {
                succNode = succNode->left;
            }
            // Initialize a temp var to hold the succNode's bid data
            Bid succNodeBidData = succNode->bid;
            // Assign the current node's bid data to the successor's bid data
            node->bid = succNodeBidData;
            // Recursively call the removeNode method to remove the successive node from the tree
                //Passing in the current node's right node and the succNode's bid id
            node->right = removeNode(node->right, succNodeBidData.bidId);
        }
    }
    // Exit the method and return the current node
    return node;
}

void BinarySearchTree::printNode(Node* node) {
    // Initialize a temp var to hold the bid info
    Bid bid = node->bid;
    // Display the bid info
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    // Exit method
    return;
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
void loadBids(string csvPath, BinarySearchTree* bst) {
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
            bst->Insert(bid);
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

    // Define a binary search tree to hold all bids
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Bid bid;

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
            loadBids(csvPath, bst);

            //cout << bst->Size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bst->InOrder();
            //bst->PostOrder();
            //bst->PreOrder();
            break;

        case 3:
            ticks = clock();

            bid = bst->Search(bidKey);

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
            bst->Remove(bidKey);
            break;
        case 9:
            break;
        default:
            cout << "Invalid Input!" << endl;
            cin.clear();
            cin.ignore();
            choice = 0;
            break;
        }
    }

    cout << "Good bye." << endl;

	return 0;
}
