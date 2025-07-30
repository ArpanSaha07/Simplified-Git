#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

// Node class to represent a node of the linked list.
class Node {
public:
    int version;        //version no. of the node
	int hash_data;      //hash number of the version
	string str_data;    //String content of the version
	Node* next;         //Pointer to the next node in the linkedlist

	// Default constructor
	Node()
	{
		hash_data = 0;
		str_data = "";
		next = NULL;
	}

	// Parameterised Constructor
	Node(int hash_data, string str_data, int version)
	{
		this->hash_data = hash_data;
		this->str_data = str_data;
        this->version = version;
		this->next = NULL;
	}
};

// Linkedlist class to implement a linked list.
class LinkedList {

public:
    Node* head;
    int list_size = 0;      // keeps track of the size of the list
    int data_version = 1;   // latest version added
    vector<int> available_versions;

	//Constructor
	LinkedList() { head = NULL;}	
};


class Git {

public:
    Git() {};
    void add(string content);
    void print(void);
    void load(int version);
    void compare(int version1, int version2);
    void remove(int version);
    void search(string keyword);

protected:
    LinkedList mylist;  //gets deconstructed automatically when out of scope

private:
    //a helper function that returns a hash integer for a given string
    size_t hash_it(string someString) {        
        return hash<string>{}(someString);
    }
};

class EnhancedGit: public Git
{
public:
    // Constructor
    EnhancedGit() {

        load_program();     //calls the load program when an object of this class is instantiated
    }

    void load_program() {
        ifstream infile("text_files/enhancedGit.txt");

        string versiontxt;
        string line;

        if (infile) {
            while(getline(infile, line)) {

                if (line.compare("$$$$$$$$$=========================================$$$$$$$$$$") == 0) {
                    versiontxt.erase(versiontxt.length()-1);    //removes the extra newline character
                    add(versiontxt);
                    versiontxt = "";
                }
                else {
                    versiontxt.append(line);
                    versiontxt.append("\n");
                }  
            }
        }
    }

    void quit() {
        Node *temp = mylist.head;
        
        ofstream outfile ("text_files/enhancedGit.txt");   //saves the versions in this file

        while (temp != NULL) {
            outfile << temp->str_data << endl;
            outfile << "$$$$$$$$$=========================================$$$$$$$$$$" << endl;
            
            int ver = temp->version;
            temp = temp->next;
            remove(ver);        //removes the node from the linkedlist before ending the program
        }
        outfile.close();
    }
};

int main()
{
    char input_char;
    EnhancedGit myGit;
    
    cout << "Welcome to the Comp file versioning system!\n" << endl;
    cout << "To add the content of your file to version control press 'a'" << endl;
    cout << "To remove a version press 'r'" << endl;
    cout << "To load a version press 'l'" << endl;
    cout << "To print to the screen the detailed list of all versions press 'p'" << endl;
    cout << "To compare any 2 versions press 'c'" << endl;
    cout << "To search versions for a keyword press 's'" << endl;
    cout << "To exit press 'e'" << endl;

    do {
        cin >> input_char;
        
        if(input_char == 'a') {
            // getting file content
            string content;
            string myfile = "./text_files/file.txt";

            fstream afile(myfile);
            ostringstream buf;

            buf << afile.rdbuf();
            content = buf.str(); 
            
            myGit.add(content);
        } 
        else if(input_char == 'p') {
            myGit.print();
        }
        else if(input_char == 'l') {
            int load_input;
            cout << "Which version would you like to load? ";
            cin >> load_input;

            myGit.load(load_input);
        }
        else if(input_char == 'c') {
            int compare_input1;
            int compare_input2;

            cout << "Please enter the number of the first version to compare: ";
            cin >> compare_input1;
            cout << endl;
            cout << "Please enter the number of the second version to compare: ";
            cin >> compare_input2;
            cout << endl;
            myGit.compare(compare_input1, compare_input2);
        }
        else if(input_char == 's') {
            string input_word;
            cout << "Please enter the keyword that you are looking for: ";
            cin >> input_word;
            myGit.search(input_word);
        }
        else if(input_char == 'r') {
            int remove_input;
            
            cout << "Enter the number of the version that you want to delete: ";
            cin >> remove_input;
            myGit.remove(remove_input);            
        }
    } while (input_char != 'e');

    myGit.quit();

    //myGit.print();  // this is to make sure the list doesn't contain any members after exit
}

void Git::add(string content) {

    int hash_data = hash_it(content);

    Node* newNode = new Node(hash_data, content, mylist.data_version);

	// Assign node to head if the arraylist is empty
	if (mylist.head == NULL) {
		mylist.head = newNode;
        
        mylist.available_versions.push_back(mylist.data_version);
		mylist.list_size += 1;  //****
		mylist.data_version += 1;

        cout << "Your content has been added successfully." << endl;
		return;
	}

	// Assign head to the temp pointer and traverse till end of list
	Node* temp = mylist.head;
    
	while (temp != NULL) {

        // checks if the data already exists
        if (temp->hash_data == hash_data) {
            cout << "git did not detect any change to your file and will not create a new version." << endl;
            return;
        }
        if (temp->next == NULL) {
            // Insert at the last.
            temp->next = newNode;

            mylist.available_versions.push_back(mylist.data_version);
            mylist.list_size += 1;
            mylist.data_version += 1;
            cout << "Your content has been added successfully." << endl;
            return;
        }
        temp = temp->next;  // Update temp pointer
	}
}

void Git::print() {

	// Check for empty list.
	if (mylist.head == NULL) {
		cout << "No versions found." << endl;
		return;
	}
    Node* temp = mylist.head;

    cout << "Number of versions: " << mylist.list_size << endl;
	// Traverse the list.
	while (temp != NULL) {
		
        cout << "Version number: " << temp->version << endl;
        cout << "Hash value: " << temp->hash_data << endl;
        cout << "Content: " << temp->str_data << endl;
        cout << endl;
        
		temp = temp->next;
	}
}

void Git::load(int version) {

    // Check if the version requested already exists
    if (!binary_search(mylist.available_versions.begin(), mylist.available_versions.end(), version)) {
        cout << "Please press 'l' and enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
        return;
    }

    ofstream myfile;
    myfile.open("./text_files/file.txt");

    Node *temp = mylist.head;

	// // Declare temp
	// temp = mylist.head;

    while (version != temp->version) {

		temp = temp->next;  // Update temp
	}

    myfile << temp->str_data;
    myfile.close();
    cout << "Version " << version << " loaded successfully. Please refresh your text editor to see the changes." << endl;
}

void Git::remove(int version)
{
    // Check if the version requested already exists
    if (!binary_search(mylist.available_versions.begin(), mylist.available_versions.end(), version)) {
        cout << "Please press 'r' and enter a valid version number. If you are not sure please press 'p' to list all valid version numbers." << endl;
        return;
    }

    Node* temp1 = mylist.head;
    Node* temp2 = NULL;
    int i = 0;

    while (temp1 != NULL && temp1->version != version) {
        temp2 = temp1;
        temp1 = temp1->next;
        i++;
    }

    // if the head needs to be removed, the head is assigned to the next element in the list
    if (temp1 == mylist.head) {     
        mylist.head = temp1->next;
    } else {
        temp2->next = temp1->next;
        temp1->next = NULL;
    }

    mylist.available_versions.erase(mylist.available_versions.begin() + i); //removes the version from the available_versions vector
	mylist.list_size -= 1;
    cout << "Version " << version << " deleted successfully." << endl;

    // delete temp1;
    // delete temp2;
}

void Git::compare(int version1, int version2) {
    
	if (mylist.head == NULL) {
        cout << "No versions found." << endl;
		return;
	}
    // check if the versions exist
	if (!binary_search(mylist.available_versions.begin(), mylist.available_versions.end(), version1)) {
        cout << "Version " << version1 << " does not exist." << endl;
        return;
    }

    if (!binary_search(mylist.available_versions.begin(), mylist.available_versions.end(), version2)) {
        cout << "Version " << version2 << " does not exist." << endl;
        return;
    }
    Node *tempNode = mylist.head, *tempNode1 = NULL, *tempNode2 = NULL;

    while (tempNode != NULL) {

		if (tempNode->version == version1) {
            tempNode1 = tempNode;
        }
        if (tempNode->version == version2) {
            tempNode2 = tempNode;
        }

		tempNode = tempNode->next;
	}

    string text1 = tempNode1->str_data;
    string text2 = tempNode2->str_data;

    string str_temp1, str_temp2;
    stringstream input_stringstream1(text1); 
    stringstream input_stringstream2(text2);
    
    
    bool end_of_version1 = true;
    bool end_of_version2 = true;
    int line_num = 0;
    
    while (end_of_version1 || end_of_version2) {
        line_num += 1;

        //if one of the versions have fewer lines than the other:
        if (!getline(input_stringstream1,str_temp1,'\n')) {
            end_of_version1 = false;
            str_temp1 = "";
        }
        if (!getline(input_stringstream2,str_temp2,'\n')) {
            end_of_version2 = false;
            str_temp2 = "";
        }
        
        size_t hash1 = hash_it(str_temp1);
        size_t hash2 = hash_it(str_temp2);
        
        if (!end_of_version1 && !end_of_version2) {
            continue;
        }
        else if (hash1 == hash2) {
            cout << "Line " << line_num << ": " << " <Identical> " << endl;
        }  
        else if(str_temp1 == "") {
            cout << "Line " << line_num << ": " << "<Empty line> <<>> ";
            cout << str_temp2 << endl;
        }
        else if(str_temp2 == "") {
            cout << "Line " << line_num << ": " << str_temp1 << " <<>> ";
            cout << "<Empty line>" << endl;
        }
        else {
            cout << "Line " << line_num << ": " << str_temp1 << " <<>> ";
            cout << str_temp2 << endl;
        }
    }   
}

void Git::search(string keyword) {

	// Check for empty list.
	if (mylist.head == NULL) {
		cout << "No versions found." << endl;
		return;
	}
    Node* temp = mylist.head;

    int found_counter = 0;  // keeps track of the total number of versions the word was found in

	// Traverse the list.
	while (temp != NULL) {
        int found = (temp->str_data).find(keyword);   //-1 if not found
        // if (temp->str_data[found+1] != ' ') { // this is to ensure the function only searches for words and not substrings
        //     found = -1;
        // }

        if (found != -1) {
            found_counter += 1;
            if (found_counter == 1) {
                cout << "The keyword \'" << keyword << "\' has been found in the following versions:" << endl;
            }
            cout << "Version number: " << temp->version << endl;
            cout << "Hash value: " << temp->hash_data << endl;
            cout << "Content: " << temp->str_data << endl;
            cout << endl;
        }
        
		temp = temp->next;      //updates the pointer
	}
    if (found_counter == 0) {
            cout << "Your keyword \'" << keyword << "\' was not found in any version." << endl;
    }
}
