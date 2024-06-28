//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Dat Nguyen
// Version     : 1.0
// Description :  ABCU Advising Assistance Program
//============================================================================

#include <iostream>      // For standard input and output
#include <fstream>       // For file handling
#include <sstream>       // For string stream operations
#include <vector>        // For using the vector container
#include <unordered_map> // For using the unordered_map container
#include <algorithm>     // For using algorithms like sort and transform
#include <cctype>        // For character handling functions

using namespace std;

// Course structure to hold course details
struct Course
{
    string courseNumber;          // Course number
    string courseName;            // Course name
    vector<string> prerequisites; // List of prerequisite courses
};

// Hash Table class definition to manage courses
class HashTable
{
private:
    unordered_map<string, Course> table; // Unordered map to store courses
public:
    void Insert(const Course &course);               // Method to insert a course
    void PrintAll() const;                           // Method to print all courses
    Course Search(const string &courseNumber) const; // Method to search for a course
};

/**
 * Insert a course into the hash table
 *
 * @param course The course to insert
 */
void HashTable::Insert(const Course &course)
{
    table[course.courseNumber] = course; // Insert course using course number as key
}

/**
 * Print all courses in the hash table in sorted order
 */
void HashTable::PrintAll() const
{
    vector<string> keys; // Vector to store course numbers (keys)
    for (const auto &entry : table)
    {
        keys.push_back(entry.first); // Collect all course numbers
    }
    sort(keys.begin(), keys.end()); // Sort course numbers alphabetically
    for (const auto &key : keys)
    {
        const Course &course = table.at(key);                             // Get course using course number
        cout << course.courseNumber << ", " << course.courseName << endl; // Print course details
    }
}

/**
 * Search for a course by course number
 *
 * @param courseNumber The course number to search for
 * @return The course object if found, otherwise an empty course
 */
Course HashTable::Search(const string &courseNumber) const
{
    string upperCaseCourseNumber = courseNumber;      // Copy course number to modify
    transform(upperCaseCourseNumber.begin(), upperCaseCourseNumber.end(), upperCaseCourseNumber.begin(), ::toupper); // Convert to uppercase
    auto i = table.find(upperCaseCourseNumber);      // Search for course in the table
    if (i != table.end())
    {
        return i->second; // Return the course if found
    }
    return Course(); // Return an empty course if not found
}

/**
 * Load course data from a file
 *
 * @param fileName The name of the file to load data from
 * @return A vector of Course objects containing the loaded courses
 */
vector<Course> loadCourseData(const string &fileName)
{
    ifstream file(fileName);   // Open the file
    vector<Course> courseList; // Vector to store courses
    if (!file.is_open())
    {
        cout << "Error: File could not be opened." << endl; // Print error if file cannot be opened
        return courseList;
    }

    string line; // Variable to hold each line from the file
    while (getline(file, line))
    {
        stringstream ss(line);                 // Create a string stream from the line
        string token;                          // Variable to hold each token from the line
        Course course;                         // Create a course object
        getline(ss, course.courseNumber, ','); // Read course number
        getline(ss, course.courseName, ',');   // Read course name

        // Read prerequisites
        while (getline(ss, token, ','))
        {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end()); // Remove spaces
            if (!token.empty())
            {
                course.prerequisites.push_back(token); // Add prerequisite to the course
            }
        }

        courseList.push_back(course); // Add course to the course list
    }

    file.close();      // Close the file
    return courseList; // Return the course list
}

/**
 * Print course information
 *
 * @param course The course object containing the information to print
 */
void printCourseInfo(const Course &course)
{
    cout << course.courseNumber << ", " << course.courseName << endl; // Print course number and name
    if (course.prerequisites.empty())
    {
        cout << "Prerequisites: None" << endl; // Print 'None' if no prerequisites
    }
    else
    {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i)
        {
            cout << course.prerequisites[i]; // Print each prerequisite
            if (i < course.prerequisites.size() - 1)
            {
                cout << ", "; // Add comma between prerequisites
            }
        }
        cout << endl;
    }
}

/**
 * Display the menu
 */
void displayMenu()
{
    cout << endl
         << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << endl
         << "What would you like to do? ";
}

/**
 * Main function to drive the program
 *
 * @return 0 to indicate successful execution
 */
int main()
{
    HashTable courseTable;     // Create a HashTable object
    vector<Course> courseList; // Vector to store loaded courses
    int choice;                // Variable to store user choice

    cout << "Welcome to the course planner." << endl;

    do
    {
        displayMenu(); // Display the menu
        cin >> choice; // Get user choice

        cout << endl;

        switch (choice)
        {
        case 1:
        {
            char defaultChoice;
            cout << "Load default file: ABCU_Advising_Program_Input.csv? y/n: ";
            cin >> defaultChoice;

            string fileName;
            if (defaultChoice == 'y' || defaultChoice == 'Y')
            {
                fileName = "ABCU_Advising_Program_Input.csv"; // Use default file name
            }
            else if (defaultChoice == 'n' || defaultChoice == 'N')
            {
                cout << "Enter the file name to load: ";
                cin >> fileName; // Get file name from user
            }
            else
            {
                cout << "Invalid choice. Returning to menu." << endl;
                break;
            }

            courseList = loadCourseData(fileName); // Load course data from file
            if (!courseList.empty())
            {
                for (const auto &course : courseList)
                {
                    courseTable.Insert(course); // Insert each course into the hash table
                }
                cout << "Courses loaded." << endl;
            }
            break;
        }
        case 2:
        {
            courseTable.PrintAll(); // Print all courses
            break;
        }
        case 3:
        {
            string courseNumber;
            cout << "Enter course number: ";
            cin >> courseNumber;
            Course course = courseTable.Search(courseNumber); // Search for the course
            if (!course.courseNumber.empty())
            {
                printCourseInfo(course); // Print course information if found
            }
            else
            {
                cout << "Course not found." << endl;
            }
            break;
        }
        case 9:
            cout << "Thank you for using the course planner!" << endl; // Exit message
            break;
        default:
            cout << choice << " is not a valid option." << endl; // Invalid choice message
            break;
        }
    } while (choice != 9);

    return 0; // Return from main
}
