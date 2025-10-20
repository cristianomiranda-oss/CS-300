//============================================================================
// Name        : VectorSorting.cpp
// Author      : Cristiano Miranda
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Vector Sorting Algorithms
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
 * Prompt user for bid information using console (std::in)
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
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    // Initializes two variables to store positions
    int lowIndex = begin;
    int highIndex = end;

    // Declares variable with the calculated middle position index
    int midpoint = lowIndex + (highIndex - lowIndex) / 2;
    
    // Declares variable with Bid's title string located at midpoint index
    string pivotValue = bids[midpoint].title;

    // Declares a variable that determines if the while loop finishes executing
    bool done = false;
    // Loops until the done boolean is set to true
    while (!done) {
        // Increment the lowIndex while the bid[lowIndex].title is less than the pivotValue
        while (bids[lowIndex].title < pivotValue) {
            lowIndex++;
        }

        // Increment the highIndex while the bid[highIndex].title is greater than the pivotValue
        while (bids[highIndex].title > pivotValue) {
            highIndex--;
        }

        // Checks if there are one or less elements remaining
        if (lowIndex >= highIndex) {
            // The while loop finishes executing if so
            done = true;
        }
        else {
            // Swaps the elements at index and nestedIndex
            std::swap(bids[lowIndex], bids[highIndex]);

            // Increments the lowIndex and highIndex and continues running the while loop
            lowIndex++;
            highIndex--;
        }
    }

    // Returns the highIndex value
    return highIndex;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    // Checks if the bids vector has at least one element
    if (bids.size() != 0) {
        // Initializes a variable to store a position value
        int midIndex = 0;

        // Checks if begin is greater than or equal to end
        if (begin >= end) {
            // If so, the partition is sorted and the function returns
            return;
        }
        
        midIndex = partition(bids, begin, end);

        // Recursively calls quickSort to sort the lower partition
        quickSort(bids, begin, midIndex);

        // Recursively calls quickSort to sort the higher partition
        quickSort(bids, midIndex + 1, end);
    }
    else {
        // If there are no element in the bids vector, a message is displayed to the user
        cout << "Load bids before sorting!" << endl;
    }
}

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    // Checks if the bids vector has at least one element
    if (bids.size() != 0) {
        // Initializes a variable to store the size of the bids vector
        int bidsSize = bids.size();
        // Loops through the bids vector
        for (int index = 0; index < bidsSize - 1; index++) {
            // initializes variable to hold the smallest value's index
            int minIndex = index;

            // Loops through the bids vector starting from the position after the outer loop's current index
            for (int nestedIndex = index + 1; nestedIndex < bidsSize; nestedIndex++) {
                // Checks if the bid's title at nestedIndex is smaller than the  bid's title at index
                if (bids[nestedIndex].title < bids[index].title) {
                    // If so, sets the minIndex variable equal to the nestedIndex
                    minIndex = nestedIndex;
                }
            }

            // Swaps the elements at index and nestedIndex
            std::swap(bids[index], bids[minIndex]);
        }
    }
    else {
        // If there are no element in the bids vector, a message is displayed to the user
        cout << "Load bids before sorting!" << endl;
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
    string csvPath;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    default:
        csvPath = "eBid_Monthly_Sales.csv";
    }

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Selection Sort All Bids" << endl;
        cout << "  4. Quick Sort All Bids" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            bids = loadBids(csvPath);

            // Displays the amount of bids read
            cout << bids.size() << " bids read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 2:
            // Loop and display the bids read
            for (int i = 0; i < bids.size(); ++i) {
                displayBid(bids[i]);
            }
            cout << endl;

            break;

        case 3: 
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Calls the selectionSort method and passes in the bids vector
            selectionSort(bids);
            
            // Displays the amount of bids sorted
            cout << bids.size() << " bids sorted" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        case 4:
            // Initialize a timer variable before loading bids
            ticks = clock();

            // Calls the quickSort method and passes in the bids vector, the starting index, and the last index in the vector
            quickSort(bids, 0, bids.size() - 1);

            // Displays the amount of bids sorted
            cout << bids.size() << " bids sorted" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
