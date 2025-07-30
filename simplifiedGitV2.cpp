#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

void add(string content);
void print(void);
void load(int version);
void compare(int version1, int version2);
void remove(int version);   //Function to delete node at the given position
void search(string keyword);

//returns a hash integer for a given string
size_t hash_it(string someString) {
    
    std::size_t hash_num = std::hash<std::string>{}(someString);
    
    return hash_num;
}

// Node class to represent a node of the linked list.
class Node {
public:
    int version;
	int hash_data;
	string str_data;
	Node* next;

	// Default constructor
	Node()
	{
		hash_data = 0;
		str_data = "";
		next = NULL;
	}

	// Parameterised Constructor
	Node(int hash_data, string str_data)
	{
		this->hash_data = hash_data;
		this->str_data = str_data;
		this->next = NULL;
	}
};

// Linkedlist class to implement a linked list.
class Linkedlist {

public:
    Node* head;
    int list_size = 0;
    int data_version = 1;
    vector<int> available_versions;

	// Default constructor
	Linkedlist() { head = NULL; }	
};

//Global variable
Linkedlist LIST;  //gets deconstructed automatically at the end

int main()
{
    char input_char;
    
    cout << "Welcome to the Comp322 file versioning system!\n" << endl;
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
            string myfile = "./file.txt";

            fstream afile(myfile);
            ostringstream buf;

            buf << afile.rdbuf();
            content = buf.str(); 
            
            add(content);
            
        } 
        else if(input_char == 'p') {
            print();
        }
        else if(input_char == 'l') {
            int load_input;
            cout << "Which version would you like to load? ";
            cin >> load_input;

            bool found_element = false;

            while(!found_element) {
                
                cin >> load_input;
                for (auto it = LIST.available_versions.begin(); it != LIST.available_versions.end(); ) {
                    if (*it == load_input) {
                        // Found the element, erase it
                        found_element = true;
                        it = LIST.available_versions.erase(it);
                        load(load_input);
                        break;
                    } else {
                        // Move to next element
                        ++it;
                    }
                }
                if(!found_element) {
                    cout << "Please enter a valid version number. I you are unsure then enter 'p' to check." << endl;
                }
            }
            load(load_input);
            
        }
        else if(input_char == 'c') {
            int compare_input1, compare_input2;

            cout << "Please enter the number of the first version to compare: ";
            cin >> compare_input1;
            cout << endl;
            cout << "Please enter the number of the second version to compare: ";
            cin >> compare_input2;
            cout << endl;
            compare(compare_input1, compare_input2);
        }
        else if(input_char == 's') {
            string input_word;
            cout << "Please enter the keyword that you are looking for: " << endl;
            cin >> input_word;
            search(input_word);
        }
        else if(input_char == 'r') {
            int remove_input;
            bool found_element = false;
            cout << "Enter the number of the version that you want to delete: ";

            while(!found_element) {
                
                cin >> remove_input;
                for (auto it = LIST.available_versions.begin(); it != LIST.available_versions.end(); ) {
                    if (*it == remove_input) {
                        // Found the element, erase it
                        found_element = true;
                        it = LIST.available_versions.erase(it);
                        remove(remove_input);
                        break;
                    } else {
                        // Move to next element
                        ++it;
                    }
                }
                if(!found_element) {
                    cout << "Please enter a valid version number." << endl;
                }
            }
        }
        else if (input_char == 'e') {
            while (!LIST.available_versions.empty()) {
                 vector<int>::iterator it = LIST.available_versions.begin();
                remove(*it); // removes remaining elements at the end
            }
        }

    } while (input_char != 'e');

    print();
	return 0;
}

void add(string content) {

    int hash_data = hash_it(content);

    Node* newNode = new Node(hash_data, content);
	
	newNode->version = LIST.data_version;

	// Assign to head
	if (LIST.head == NULL) {
		LIST.head = newNode;
        LIST.available_versions.push_back(LIST.data_version);
		LIST.list_size += 1;
		LIST.data_version += 1;
        cout << "Your content has been added successfully." << endl;
		return;
	}

	// Traverse till end of list
	Node* temp = LIST.head;
    
	while (temp != NULL) {

        if (temp->hash_data == hash_data) {
            cout << "git322 did not detect any change to your file and will not create a new version." << endl;

            return;
        }
		// Update temp
		
        if (temp->next == NULL) {
            // Insert at the last.
            temp->next = newNode;

            LIST.available_versions.push_back(LIST.data_version);
            LIST.list_size += 1;
            LIST.data_version += 1;
            cout << "Your content has been added successfully." << endl;
            return;
        }
        temp = temp->next;
	}
    
}

void print() {
    Node* temp = LIST.head;

	// Check for empty list.
	if (LIST.head == NULL) {
		cout << "No versions found." << endl;
		return;
	}

    cout << "Number of versions: " << LIST.list_size << endl;
	// Traverse the list.
	while (temp != NULL) {
		
        cout << "Version number: " << temp->version << endl;
        cout << "Hash value: " << temp->hash_data << endl;
        cout << "Content: " << temp->str_data << endl;
        cout << endl;
        
		temp = temp->next;
	}
}

void load(int version) {
    int temp_version = version;

    ofstream myfile;
    myfile.open("./file.txt");

    Node *temp = LIST.head;

	// Declare temp
	temp = LIST.head;

    while (version != temp->version) {

		// Update temp
		temp = temp->next;
	}

    myfile << temp->str_data;
    myfile.close();
    cout << "Version " << temp_version << " loaded successfully. Please refresh your text editor to see the changes." << endl;

}


// Function to delete a node based on its version.
void remove(int version)
{
    Node* temp1 = LIST.head;
    Node* temp2 = NULL;

    while (temp1 != NULL && temp1->version != version) {
        temp2 = temp1;
        temp1 = temp1->next;
    }


    if (temp1 == LIST.head) {
        LIST.head = temp1->next;
    } else {
        temp2->next = temp1->next;
    }

    LIST.available_versions.erase(remove(LIST.available_versions.begin(), LIST.available_versions.end(), version), LIST.available_versions.end());
	LIST.list_size -= 1;
    cout << "Version " << version << " deleted successfully." << endl;
    delete temp1;
}

void compare(int version1, int version2) {
    Node *tempNode = LIST.head, *tempNode1 = NULL, *tempNode2 = NULL;
	int ListLen = LIST.list_size;

	if (LIST.head == NULL) {
		return;
	}

	if (ListLen < version1 || ListLen < version2) {
		return;
	}
	// Declare tempNode variable
	tempNode = LIST.head;
    int max_version = max(version1, version2);  //finds the larger version number among the two

    while (max_version-- >= 1) {

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

    string temp1, temp2;
    stringstream input_stringstream1(text1); 
    stringstream input_stringstream2(text2);
    
    
    bool end1 = true;
    bool end2 = true;
    int line_num = 0;
    
    while (end1 || end2) {
        line_num += 1;
        if (!getline(input_stringstream1,temp1,'\n')) {
            end1 = false;
            temp1 = "";
        }
        if (!getline(input_stringstream2,temp2,'\n')) {
            end2 = false;
            temp2 = "";
        }
        
        size_t hash1 = hash_it(temp1);
        size_t hash2 = hash_it(temp2);
        
        if (!end1 && !end2) {
            continue;
        }
        else if (hash1 == hash2) {
            cout << "Line " << line_num << ": " << " <Identical> " << endl;
        }  
        else if(temp1 == "") {
            cout << "Line" << line_num << ": " << "<Empty line> <<>> ";
            cout << temp2 << endl;
        }
        else if(temp2 == "") {
            cout << "Line" << line_num << ": " << temp1 << " <<>> ";
            cout << "<Empty line>" << endl;
        }
        else {
            cout << "Line" << line_num << ": " << temp1 << " <<>> ";
            cout << temp2 << endl;
        }
    }
    
}


void search(string keyword) {
    Node* temp = LIST.head;

	// Check for empty list.
	if (LIST.head == NULL) {
		cout << "No versions found." << endl;
		return;
	}

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
        
		temp = temp->next;
	}
    if (found_counter == 0) {
            cout << "Your keyword \'" << keyword << "\' was not found in any version." << endl;
    }
}

