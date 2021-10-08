//============================================================================
// Name        : VotingHashtable.cpp
// Author      : Willi Blanco
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Implementing Hash tables
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

const unsigned int DEFAULT_SIZE = 1000;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Response {
    string Resp_date; // Responce date
    string GEOID; //2020 Census Geography ID given by government
    double DRRALL; //Daily Self-Responce Rate - Overall
    double DRRINT; //Daily Self-Responce Rate - Internet
    double CRRALL; //Cumulative self-responce rate - overall
    double CRRINT; //Cumulative self-responce rate - internet
    string county; // County Geography
    double tract; //Tract - Geography
    string state; //state - Geography
	string uniqueID; //Unique Id. Unique Id is the date (year-month-day) and geological id given by

	Response(){
		DRRALL = 0.0;
		DRRINT = 0.0;
		CRRALL = 0.0;
		CRRINT = 0.0;
		tract = 0.0;
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
    // FIXME (1): Define structures to hold bids
	struct Node {
		Response censusResponse;
		unsigned key;
		Node* next;

		//default constructor
		Node() {
			key = UINT_MAX;
			next = nullptr;
		}

		//Initialize with a bid
		Node(Response aResponse) : Node() {
			censusResponse = aResponse;
		}

		//Initilize with a bid and a key
		Node (Response aResponse, unsigned aKey) : Node(aResponse) {
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned size);
    virtual ~HashTable();
    void Insert(Response response);
    void PrintAll();
    Response Search(string responseID);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    //Initializing the structures used to hold bids
	nodes.resize(tableSize);
}

HashTable::HashTable(unsigned size){
	this->tableSize = size;
	nodes.resize(tableSize);
}

/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (3): Implement logic to free storage when class is destroyed
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
    // FIXME (4): Implement logic to calculate a hash value
	return key % tableSize;
}


/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Response response) {
    // FIXME (5): Implement logic to insert a bid

	//calculate the key for this bid
	unsigned key = hash(atoi(response.uniqueID.c_str()));

	//try and retrieve node using the key
	Node* oldNode = & (nodes.at(key));

	//if no entry found for this key
	if(oldNode == nullptr){

		Node* newNode = new Node(response, key);
		nodes.insert(nodes.begin() + key, (*newNode));

	} else {
		//node found
		if (oldNode-> key == UINT_MAX) {

			oldNode-> key = key;
			oldNode-> censusResponse = response;
			oldNode-> next = nullptr;

		}

		else{
			//find the next open node (last one)
			while (oldNode-> next != nullptr){
				oldNode = oldNode-> next;
			}

			oldNode-> next = new Node (response, key);
		}
	}
}


/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all bids
	for (auto nodeSearch = nodes.begin(); nodeSearch != nodes.end(); ++nodeSearch) {

	//Set the conditional if not UNIT_MAX and output the key and bid data:

		if (nodeSearch->key != UINT_MAX) {

			cout << "Key " << nodeSearch->key << ": " << nodeSearch->censusResponse.Resp_date << " | "
				<< nodeSearch->censusResponse.GEOID << " | " << nodeSearch->censusResponse.DRRALL << " | "
	            << nodeSearch->censusResponse.DRRINT << " | " << nodeSearch->censusResponse.CRRALL << " | "
				<< nodeSearch->censusResponse.CRRINT << " | " << nodeSearch->censusResponse.county << " | "
				<< nodeSearch->censusResponse.tract << " | " << nodeSearch->censusResponse.state << " | "
				<< nodeSearch->censusResponse.uniqueID;

	//while node not equal to nullptr output the node data:
			Node* node = nodeSearch->next;
			while (node != nullptr) {
				cout << "    " << node->key << ": " << node->censusResponse.Resp_date << " | "
						<< node->censusResponse.GEOID << " | " << node->censusResponse.DRRALL << " | "
			            << node->censusResponse.DRRINT << " | " << node->censusResponse.CRRALL << " | "
						<< node->censusResponse.CRRINT << " | " << node->censusResponse.county << " | "
						<< node->censusResponse.tract << " | " << node->censusResponse.state << " | "
						<< node->censusResponse.uniqueID;
			node = node->next;
			}
		}
	}
}


/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Response HashTable::Search(string responseID) {
	Response censusResponse;

    // FIXME (8): Implement logic to search for and return a bid

    //calculate the key for this bid
    unsigned key = hash(atoi(responseID.c_str()));

	//try and retrieve node using the key
	Node* node = & (nodes.at(key));

	//if no entry found
	if(node == nullptr || node->key == UINT_MAX){
		cout << "returned nothing at 231";
		return censusResponse;
	}

	//if the node found matches the key
	if (node != nullptr && node->key != UINT_MAX
			&& node->censusResponse.uniqueID.compare(responseID) == true) {
		return node->censusResponse;

	}

	//walk the linked list to find the match
	while (node != nullptr){
		if (node->key!=UINT_MAX && node->censusResponse.uniqueID.compare(responseID) == true){
			return node->censusResponse;
		}
		node = node->next;
	}

    return censusResponse;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayResponse(Response aResponce) {
    cout << aResponce.Resp_date << " | "
			<< aResponce.GEOID << " | " << aResponce.DRRALL << " | " << aResponce.DRRINT << " | " << aResponce.CRRALL << " | "
			<< aResponce.CRRINT << " | " << aResponce.county << " | " << aResponce.tract << " | " << aResponce.state << " | "
			<< aResponce.uniqueID;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadResponse(string csvPath, HashTable* hashTable) {
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
            Response response;
            response.Resp_date = file[i][0];
            response.GEOID = file[i][1];
            response.DRRALL = strToDouble(file[i][2], '$');
            response.DRRINT = strToDouble(file[i][3], '$');
            response.CRRALL = strToDouble(file[i][4], '$');
            response.CRRINT = strToDouble(file[i][5], '$');
            response.county = file[i][6];
            response.tract = strToDouble(file[i][7], '$');
            response.state = file[i][8];
            response.uniqueID = file[i][10];


            // push this bid to the end
            hashTable->Insert(response);
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
    string csvPath, searchValue;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        searchValue = "2020040648453000102";
        break;
    case 3:
        csvPath = argv[1];
        searchValue = argv[2];
        break;
    default:
        csvPath = "census-2020-response-rates.csv";
        searchValue = "2020040648453000102";
    }

    // Define a hash table to hold all the bids
    HashTable* censusTable;

    Response response;

    int choice = 0;
    while (choice != 9) {
        cout << endl << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Response" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            censusTable = new HashTable();

            // Complete the method call to load the bids
            loadResponse(csvPath, censusTable);
            break;

        case 2:
            censusTable->PrintAll();
            break;

        case 3:
            response = censusTable->Search(searchValue);

            if (!response.uniqueID.empty()) {
                displayResponse(response);
            } else {
                cout << "Response Id " << searchValue << " not found." << endl;
            }

            break;

        }
    }

    cout << "Good bye." << endl;

    return 0;
}
