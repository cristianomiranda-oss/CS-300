//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Cristiano Miranda
// Version     : 1.0
// Description : ABCU Advising Program for handling course data.
//============================================================================


#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
using namespace std;

// Structure for the course object
struct Course {
    string number;
    string name;
    vector<string> preReqs;
};

// Structure for the tree nodes objects
struct Node {
    // Initializes the course var and the pointer vars
    Course course;
    Node* right;
    Node* left;

    // Default constructor
    Node() {
        // Point both vars to null
        right = nullptr;
        left = nullptr;
    }

    // Initializing with a newCourse
    Node(Course newCourse) :
        // Call the default constructor
        Node() {
        // Set course to the passed in course
        course = newCourse;
    }
};

void printCourse(Course course, bool showPreReqs) {
    // Displays the course number and name
    cout << course.number << ", " << course.name << endl;
    
    // Checks if the prerequisites should be displayed
    if (showPreReqs == true) {
        // Initializes a local var to store the size of the prerequisites vector
        int numPreReqs = course.preReqs.size();
        // Checks if the perquisites vector is empty
        if (numPreReqs != 0) {
            // Displays the initial prerequisites intro
            cout << "Prerequisites:";
            // Loops through preReqs vector
            for (int index = 0; index < numPreReqs; index++) {
                // Checks if the current index is the first or last element
                if (index == 0) {
                    // Displays without a leading comma
                    cout << " " << course.preReqs[index];
                }
                else {
                    // Displays with a leading comma
                    cout << ", " << course.preReqs[index];
                }
            }

            // Ends the line after listing the prerequisites
            cout << endl;
        }
        else {
            // Indicates that no prerequisites are available
            cout << "No Prerequisites." << endl;
        }
    }
}

class BinarySearchTree {

private:
    // Points to the first node of the tree
    Node* root;

    Node* removeNode(Node* node, string courseNumber);
    void addNode(Node* node, Course course);
    void inOrder(Node* node);
public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void Search(string courseNumber);
    void DisplayAll();
};

/**
 * Default constructor for the BinarySerachTree class
 */
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
};

/**
 * Default de-constructor for the BinarySerachTree class
 */
BinarySearchTree::~BinarySearchTree() {
    // Initializes a new pointer and points it to the root of the tree
    Node* currNode = nullptr;

    // Loops through all nodes within the tree
    while (currNode != nullptr) {
        // Calls the removeNode method and it returns the next node in line for the tree
            // currNode is set to the next node until all tree nodes are removed
        currNode = removeNode(currNode, currNode->course.number);
    }
};

/**
 * Inserts a new course into the Binary Search Tree
 *
 * @param course - New course to be inserted into the tree
 * @return void
 */
void BinarySearchTree::Insert (Course course) {
    // Check if the tree's root is pointing to null
    if (root == nullptr) {
        // Initialize a new node and point the root to it
        root = new Node(course);
    }
    else {
        // Call the addNode method to add the course to the tree
        addNode(root, course);
    }
};

/**
 * Searches through the Binary Search Tree until the desired node is found
 * 
 * @param courseNumber - Course number associated with the node to be found
 * @return void
 */
void BinarySearchTree::Search (string courseNumber) {
    // Initializes a pointer and points it to the root of the tree
    Node* currNode = root;

    // Converts the passed in string to uppercase, to compare letters not characters
    transform(courseNumber.begin(), courseNumber.end(), courseNumber.begin(), ::toupper);

    // Traverses the tree until the associated node is found or a null pointer is reached
    while (currNode != nullptr) {
        // Initializes a local var to hold the current course's number
        string currCourseNumber = currNode->course.number;

        // Converts the current course's number to uppercase, to ensure consistency when comparing
        transform(currCourseNumber.begin(), currCourseNumber.end(), currCourseNumber.begin(), ::toupper);

        // Compares the current node's course number to the passed in course number
        if (currCourseNumber == courseNumber) { // Indicates the associated node is found
            // Calls print course function with the flag to print prerequisites info
            printCourse(currNode->course, true);
            return;
        }
        else if (currNode->course.number > courseNumber) { // Indicates the associated node should be to the left of the current node
            // Navigates to the left child of the current node
            currNode = currNode->left;
        }
        else { // Indicates the associated node should be to the right of the current node
            // Navigates to the right child of the current node
            currNode = currNode->right;
        }
    }

    // Display a message indicating the passed in courseNumber is not associated with any course nodes
    cout << "Associated course not found!" << endl;
};

/**
 * Displays all elements in the tree starting from the root node
 * 
 * @return void
 */
void BinarySearchTree::DisplayAll() {
    // Call the inOrder method to start from the root of the tree
    inOrder(root);
};

/**
 * Recursive method to removes the node associated with the passed in courseNumber
 * and returns the to-be removed node's successive node, or null if there is no successive node.
 * 
 * @param node - Pointer to the current node 
 * @param courseNumber - String value to indicate which node is to be removed
 * @return Node* - Returns a pointer to the removed node's successive node, or null if none exists.
 */
Node* BinarySearchTree::removeNode(Node* node, string courseNumber) {
    // Checks if node is pointing to null or compare the current node's course number to the to-be removed course number
    if (node == nullptr) {
        // Return the null value
        return node;
    }
    else if (node->course.number > courseNumber) { // Indicates that the to-be removed node is positioned to the left of the current node.
        // Recursively call the method to search the left path
            // the successive node of the to-be removed node or null will be returned which will be set to the current node's left pointer
        node->left = removeNode(node->left, courseNumber);
    }
    else if (node->course.number < courseNumber) { // Indicates that the to-be removed node is positioned to the right of the current node.
        // Recursively call the method to search the right path
            // the successive node of the to-be removed node or null will be returned which will be set to the current node's left pointer
        node->right = removeNode(node->right, courseNumber);
    }
    else { // Indicates the current node is the to-be removed node
        // Check if the current node has any child nodes
        if (node->left == nullptr && node->right == nullptr) { // Indicates no child nodes
            // Deletes the current node and points the node pointer to null
            delete node;
            node = nullptr;
        }
        else if (node->left != nullptr && node->right == nullptr) { // Indicates there is a left child node
            // Initializes a pointer and points it to the node pointer
            Node* removedNode = node;
            // Points the node pointer to the current node's left child
            node = node->left;
            // Deletes the current node
            delete removedNode;
            // Points the current node to null
            removedNode = nullptr;
        }
        else if (node->left == nullptr && node->right != nullptr) { // Indicates there is a right child
            // Initializes a pointer and points it to the node pointer
            Node* removedNode = node;
            // Points the node pointer to the current node's right child
            node = node->right;
            // Deletes the current node
            delete removedNode;
            // Points the current node to null
            removedNode = nullptr;
        }
        else { // Indicates the current node has both child nodes
            // Initializes a pointer and points to the current node's right child
            Node* succNode = node->right;
            // Traverse's the right child's left path until the current node's successor is found
            while (succNode->left != nullptr) {
                succNode = succNode->left;
            }
            // Initializes a local var to store the successor's course data
            Course succNodeCourseData = succNode->course;
            // Sets the current node's course data to the successor's course data
            node->course = succNodeCourseData;
            // Recursively calls the function to remove the successor node from the tree, starting from the current node's right child
            removeNode(node->right, succNodeCourseData.number);
        }
    }

    // Returns the current node pointer
    return node;
};

/**
 * Traverses through the tree along the appropriate path relative to the course's course number.
 * The method recursively calls itself until an empty position in the tree is found
 *
 * @param node - Pointer to a node in the binary search tree
 * @param course - Course object that will be added to the tree
 * @return void
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // Compare the current node's course number to new course's number
    if (node->course.number > course.number) { // Indicates the new course should be inserted to the left of the current node
        // Check if the current node's left child is not occupied
        if (node->left == nullptr) {
            // Initializes a new node and points the current node's left pointer to it
            node->left = new Node(course);
        }
        else {
            // Recursively calls the method to insert the new course to the left path of the current node
            addNode(node->left, course);
        }
    }
    else { // Indicates the new course should be inserted to the right of the current node
        // Check if the current node's right child is not occupied
        if (node->right == nullptr) {
            // Initializes a new node and points the current node's left pointer to it
            node->right = new Node(course);
        }
        else {
            // Recursively calls the method to insert the new course to the right path of the current node
            addNode(node->right, course);
        }
    }
};

/**
 * Displays the course info in the current node after recursively calling the method for the left child
 * and before recursively calling the method for the right child
 *
 * @param node - Pointer to a node in the binary search tree
 * @return void
 */
void BinarySearchTree::inOrder(Node* node) {
    // Check if the node pointer is pointing to null
    if (node == nullptr) {
        // Exit the method
        return;
    }

    // Recursively call the method for the current node's left child
    inOrder(node->left);
    // Call the print course function, and disable the flag to show prerequisites
    printCourse(node->course, false);
    // Recursively call the method for the current node's right child
    inOrder(node->right);
};

/**
 * Parses file lines into separate strings.
 * Each string is split using a ','
 *
 * @param courseData - Data gathered from reading each line of the file
 * @return void
 */
void parseFileData(vector<vector<string>> courseData, BinarySearchTree* courses) {
    try {

        // Initializes a variable to store the number of rows from the courseData file
        int courseFileSize = courseData.size();

        // Iterates through the courseData vector
        for (int rowNum = 0; rowNum < courseFileSize; rowNum++) {
            // Initializes a variable to store the current row's number of parameters
            int rowLength = courseData[rowNum].size();

            // Checks if the current row has the minimum number of parameters
            if (rowLength < 2) {
                //Credit: https://www.geeksforgeeks.org/cpp/stringstream-c-applications/
                    // Utilize a string stream to combine strings and integers into a singular string

                // Initializes a string stream to house the error message
                stringstream errorMessage;
                // Inserts the error message to the string stream
                errorMessage << "File row #" << rowNum << " has insufficient parameters!";
                // Converts the error message to a string and throws the error message
                throw std::runtime_error(errorMessage.str());
            }

            // Initializes a new course object and assigns the number and name for the course
            Course newCourse;
            newCourse.number = courseData[rowNum][0];
            newCourse.name = courseData[rowNum][1];

            // Checks if the course has prerequisites
            if (rowLength > 2) {
                vector<string> currentPreReqs;

                // Iterates through the remaining parameters in the row, starting at the third parameter
                for (int paramNum = 2; paramNum < rowLength; paramNum++) {
                    // Initializes a boolean to indicate if the preReq is a valid course
                    bool isValid = false;

                    // Loops all rows in the courseData vector
                    for (int nestedRowNum = 0; nestedRowNum < courseFileSize; nestedRowNum++) {
                        // Checks if the prerequisites has an associated course number in the file
                        if (courseData[nestedRowNum][0] == courseData[rowNum][paramNum]) {
                            // Sets the isValid boolean to true
                            isValid = true;
                            // Exits the for loop
                            continue;
                        }
                    }

                    // Checks if the isValid boolean is still false, if so an error is thrown
                    if (!isValid) {
                        //Credit: https://www.geeksforgeeks.org/cpp/stringstream-c-applications/
                            // Utilize a string stream to combine strings and integers into a singular string

                        // Initializes a string stream to house the error message
                        stringstream errorMessage;
                        // Inserts the error message to the string stream
                        errorMessage << "File row #" << rowNum << " has prerequisites for non-listed courses!";
                        // Converts the error message to an string and throws the error message
                        throw std::runtime_error(errorMessage.str());
                    }

                    // Insert the pre prerequisites into the course object's preReqs vector
                    newCourse.preReqs.push_back(courseData[rowNum][paramNum]);
                }
            }

            // Calls the Binary Tree's insert method to insert the new course
            courses->Insert(newCourse);
        }
    }
    catch (const std::runtime_error& e) {
        // Throws the error to the upper try-catch block
        throw;
    }
};

/**
 * Opens a csv file and read each line.
 * Parses each read line and stores the values into separate vectors.
 *
 * @param csvFilePath - The path string to access the desired file
 * @return bool - Flag indicating if the data from the file was read and parsed correctly
 */
bool loadCourseData(string csvFilePath, BinarySearchTree* courses) {
    // Credit: https://cplusplus.com/reference/istream/istream/getline/
        // Access a csv file and read data line by line

    try {
        // Creates a stream to read the data for the passed in file path
        ifstream courseDataFile(csvFilePath);

        // Checks if the file failed to open
        if (!courseDataFile.is_open()) {
            // Initializes and throws a new error message if the file failed to open
            string errorMessage = "File failed to open!";
            throw std::runtime_error(errorMessage);
        }

        // Initializes a new vector to hold the parsed file data
        vector<vector<string>> courseData;
        // Initializes a local string var to hold the currentLine output
        string currentLine;

        while (std::getline(courseDataFile, currentLine)) {
            // Credit: https://www.geeksforgeeks.org/cpp/how-to-split-string-by-delimiter-in-cpp/#split-string-by-delimiter-in-c
                // Split the current file's line by a delimiter, in this case a ',', to separate the parameters for the line

            // Initializes a new vector to hold currentLine's course data
            vector<string> newCourseData;
            // Initializes a string stream of the currentLine
            istringstream newCourse(currentLine);
            // Initializes a local var to hold the currentParameter from the currentLine sting
            string currentParameter;

            // Iterates through the string stream,
                // The current string is pulled until a ',' character is reached
            while (std::getline(newCourse, currentParameter, ',')) {
                // Checks the currentParameter is a valid string
                if (currentParameter != "") {
                    // Pushes the split string to the newCourse Data vector
                    newCourseData.push_back(currentParameter);
                }
            }
            // Pushes the newCourseData vector to the courseData Vector
            courseData.push_back(newCourseData);
        }

        // Closes the file after reading the data
        courseDataFile.close();

        // Call the function to parse the file data into course object and append them into the binary search tree
        parseFileData(courseData, courses);

        // Signal that the data was properly loaded
        return true;
    }
    catch (const std::runtime_error& e) {
        // Prints the error message that caused the issue
        cout << e.what() << endl;

        // Signal that the data was not loaded
        return false;
    }
};

/**
 * Initializes the main data structure for storing courses and loads the menu for the program
 */
int main()
{
    // Initializes a pointer to a new binary search tree
    BinarySearchTree* courses = new BinarySearchTree();
    // Initializes two string vars to store the csv file path and the courseNumber entered by the user
    string csvPath;
    string courseNumber;
    // Initializes a var to signal if data has been loaded
    bool isDataLoaded = false;

    // Displays a welcome message
    cout << "Welcome to the course planner." << endl;

    // Initializes a var to store the user's menu choice, set to 0 by default
    int choice = 0;
    // Loops until the user enters '9'
    while (choice != 9) {
        // Displays the menu 
        cout << 
            "\n1. Load Course Data\n" <<
            "2. Display All Courses\n" <<
            "3. Display Course\n" <<
            "9. Exit\n\n" <<
            "What would you like to do? ";
        // Gathers the user's input
        cin >> choice;

        // Switches based on user's input
        switch (choice) {
        case 1: // Loads all file info
            // Indicates the user needs to enter a file path
            cout << "Enter csv file path: ";
            // Gathers the user's input
            cin >> csvPath;
            // Calls the function to load all course data from the given file path into the binary search tree
            isDataLoaded = loadCourseData(csvPath, courses);

            // Check if the data failed to load
            if (!isDataLoaded) {
                // Display a message to the user warning them of the failed loading
                cout << "Failed to load data from file!" << endl;
            }
            
            break;
        case 2: // Displays all courses in the binary search tree
            // Check if data is loaded
            if (isDataLoaded) {
                // Displays the initial message
                cout << "Here is a sample schedule:\n" << endl;
                // Calls the method to display all courses in the binary search tree in alphanumeric order
                courses->DisplayAll();
            }
            else { 
                // Display a warning message to the user
                cout << "Load data before displaying all courses!" << endl;
            }
            break;
        case 3: // Searches for a course in the binary search tree
            // Check if data is loaded
            if (isDataLoaded) {
                // Indicates the user needs to enter a course number
                cout << "\nWhat course do you want to know about? ";
                // Gathers the user's input
                cin >> courseNumber;

                // Calls the method to search for the specified course associated with the courseNumber
                courses->Search(courseNumber);
            }
            else {
                // Display a warning message to the user
                cout << "Load data before displaying all courses!" << endl;
            }
            break;
        case 9: // Exits the statement
            // Exits the statement if the exit button is pressed
            break;
        default: // User entered an invalid key
            // Displays an error message to the user
            cout << "'" << choice << "'" << " is not a valid option." << endl;
            // Clears the user input
            cin.clear();
            cin.ignore();
            // Sets the choice to 0
            choice = 0;
            break;
        }
    }
    
    // Displays an exit message
    cout << "Thank you for using the course planner!" << endl;
};
