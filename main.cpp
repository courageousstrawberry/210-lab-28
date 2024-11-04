#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <random>
#include <numeric>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();

int main() {
    srand(time(0));
    bool again;

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++]);
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++]);
    fin1.close();

    // create & populate a trip of Goats using std::list of random size 8-15
    int tripSize = rand() % 8 + 8;
    list<Goat> trip;
    int age;
    string name, color;
    for (int i = 0; i < tripSize; i++) {
        age = rand() % MAX_AGE;  // defined in Goat.h
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        Goat tmp(name, age, color);
        trip.push_back(tmp);
    }
    
    // Goat Manager 3001 Engine
    int sel = main_menu();
    while (sel != 12) {
        switch (sel) {
            case 1:
                cout << "Adding a goat.\n";
                add_goat(trip, names, colors);
                break;
            case 2:    
                cout << "Removing a goat.\n";
                delete_goat(trip);
                break;
            case 3:    
                cout << "Displaying goat data.\n";
                display_trip(trip);
                break;
            case 4: {
                // Count all pink goats
                cout << "Counting goats with color 'Pink':\n";
                // Use count_if and iterate from the beginning to the end of the list.
                int count = count_if(trip.begin(), trip.end(), [](const Goat& g) {
                    return g.get_color() == "Pink";
                });
                // Display the number of pink goats.
                cout << "Count: " << count << endl;
                break;
            }
            case 5: {
                // Prompt user for a color.
                string color;
                cout << "Enter a color to search for: ";
                cin >> color;
                cout << "Searching for Goats with " << color <<  " color..." << endl;
                // Use find_if to search for goats with that specific color.
                auto it = find_if(trip.begin(), trip.end(), [&color](const Goat& v){ return v.get_color() == color; });
                // Display what goat was found.
                if (it != trip.end())
                    cout << it->get_name() << " found with color " << it->get_color() << endl;
                else
                    cout << "None found." << endl;
                break;
            }
            case 6: {
                // Remove a goat based on a name the user enters.
                string name;
                cout << "Enter a name to remove: ";
                cin >> name;
                // Use erase and remove_if to iterate through the whole list and get rid of necessary goats.
                trip.erase(remove_if(trip.begin(), trip.end(), [&name](const Goat& v){ return v.get_name() == name; }), trip.end());
                break;
            }
            case 7: {
                // Use accumulate to add up all the goat's ages.
                int totalAge = accumulate(trip.begin(), trip.end(), 0, [](int sum, const Goat& v){ return sum + v.get_age(); });
                cout << "Total ages: " << totalAge << endl;
                break;
            }
            case 8: {
                // Use max_element to find and display the oldest goat.
                cout << "Finding the oldest goat..." << endl;
                auto max_it = max_element(trip.begin(), trip.end(), [](const Goat& a, const Goat& b) {
                    return a.get_age() < b.get_age();
                });
                if (max_it != trip.end())
                    cout << "Oldest goat: " << max_it->get_name() << " (" << max_it->get_age() << " years old)" << endl;
                else
                    cout << "No goats found." << endl;
                break;
            }
            case 9: {
                // Use min_element to find and display the youngest goat.
                cout << "Finding the youngest goat..." << endl;
                auto max_it = min_element(trip.begin(), trip.end(), [](const Goat& a, const Goat& b) {
                    return a.get_age() < b.get_age();
                });
                if (max_it != trip.end())
                    cout << "Youngest goat: " << max_it->get_name() << " (" << max_it->get_age() << " years old)" << endl;
                else
                    cout << "No goats found." << endl;
                break;
            }
            case 10:
                // Reverse the list of goats
                cout << "Reversing the order of the goats..." << endl;
                reverse(trip.begin(), trip.end());
                cout << "Goats are reversed!" << endl;
                break;
            case 11:
                // Increase the age of all goats by 1
                cout << "Increasing all goats' ages by one..." << endl;
                // Use trasform to loop through each goat in the list and increment its age.
                transform(trip.begin(), trip.end(), trip.begin(), [](Goat& g) { 
                    g.set_age(g.get_age() + 1); 
                    return g;
                });
                cout << "Ages successfully incremented." << endl;
                break;
            default:
                cout << "Invalid selection.\n";
                break;
        }
        sel = main_menu();
    }
    

    return 0;
}

int main_menu() {
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] Count pink goats\n";
    cout << "[5] Find a goat by color\n";
    cout << "[6] Remove goats by name\n";
    cout << "[7] Sum of goat ages\n";
    cout << "[8] Find the oldest goat\n";
    cout << "[9] Find the youngest goat\n";
    cout << "[10] Reverse order of goats\n";
    cout << "[11] Increment all goats' ages by one\n";
    cout << "[12] Quit\n";
    cout << "Choice --> ";
    int choice;
    cin >> choice;
    while (choice < 1 || choice > 12) {
        cout << "Invalid, again --> ";
        cin >> choice;
    }
    return choice;
}

void delete_goat(list<Goat> &trip) {
    cout << "DELETE A GOAT\n";
    int index = select_goat(trip);
    auto it = trip.begin();
    advance(it, index-1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << endl;
}

void add_goat(list<Goat> &trip, string nms[], string cls[]) {
    cout << "ADD A GOAT\n";
    int age = rand() % MAX_AGE;
    string nm = nms[rand() % SZ_NAMES];
    string cl = cls[rand() % SZ_COLORS];
    Goat tmp(nm, age, cl);
    trip.push_back(tmp);
    cout << "Goat added. New trip size: " << trip.size() << endl;
}

void display_trip(list<Goat> trp) {
    int i = 1;
    for (auto gt: trp)
        cout << "\t" 
             << "[" << i++ << "] "
             << gt.get_name() 
             << " (" << gt.get_age() 
             << ", " << gt.get_color() << ")\n";
}

int select_goat(list<Goat> trp) {
    int input;
    cout << "Make a selection:\n";
    display_trip(trp);
    cout << "Choice --> ";
    cin >> input;
    while (input < 1 or input > trp.size()) {
        cout << "Invalid choice, again --> ";
        cin >> input;
    }
    return input;
}