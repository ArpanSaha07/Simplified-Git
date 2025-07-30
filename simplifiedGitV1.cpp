#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

//Q1 string equality
bool word_diff(string word1, string word2) {
    return word1 == word2;
}

//Q2
bool classical_file_diff(string file1, string file2){
    ifstream filestream1(file1, ifstream::binary|ifstream::ate);
    ifstream filestream2(file2, ifstream::binary|ifstream::ate);
    string word1, word2;

    //some checks I thought were useful
    if (filestream1.fail() || filestream2.fail()) {
        cerr << "Error opening file: " << file1 << " " << file2 << endl;
        return false; //file problem
    }

    if (filestream1.tellg() != filestream2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and compare contents
    filestream1.seekg(0, ifstream::beg);
    filestream2.seekg(0, ifstream::beg);

    filestream1.open(file1);
    filestream2.open(file2);
    //use Q1's helper
    while (filestream1 >> word1 && filestream2 >> word2) {
        if(!word_diff(word1,word2)){
            return false;
        }
    }
    return true; 
}
//Q3 pregiven hash function
size_t hash_it(string someString){
    return hash<string>()(someString);
}

// helper function to hash entire file for Q4
string hashFile(string file) {
    ifstream filestream(file, ios::binary);
    if (filestream.fail()) {
        cerr << "Error opening file: " << file << endl;
        return ""; //file problem
    }

    //get the whole content and hash it
    string content((istreambuf_iterator<char>(filestream)), istreambuf_iterator<char>());
    size_t hashValue = hash_it(content); //hash value

    //return as a string
    return to_string(hashValue);
}

//Q4
bool enhanced_file_diff(string file1, string file2){

    //hash files and check if they are identical
    string hash1 = hashFile(file1);
    string hash2 = hashFile(file2);

    return word_diff(hash1, hash2); //can use Q1 helper as hash is string
}

//helper to get all lines of a file
vector<string> getFileLines(string file) {
    ifstream filestream(file);
    if (filestream.fail()) {
        cerr << "Error opening file: " << file << endl; //file problem
    }
    vector<string> lines;
    string line;
    //loop through, add lines to vector
    while (getline(filestream, line)) {
        lines.push_back(line);
    }
    return lines;
}

//helper to get all words of a line
vector<string> getWordsFromLine(string line) {
    vector<string> words;
    stringstream linestream(line);
    string word;
    //loop through, add words to vector
    while (linestream >> word) {
        words.push_back(word);
    }
    return words;
}

//helper for Q5
void recursive_line_comparison(vector<string> lines1, vector<string> lines2, int start, int end, vector<int>& mismatched_lines){
    if (end >= start){
        //cant use vectors for hashing so convert them into strings
        string substr1;
        string substr2;

        //slice the file depending on if hashes were equal or not previously
        //if same hash, no need to look further down that path
        //if not, find which line(s) is/are causing the trouble
        vector<string> sub1 = vector<string>(lines1.begin() + start, lines1.begin() + end);
        vector<string> sub2 = vector<string>(lines2.begin() + start, lines2.begin() + end);

        //convert vectors (slices) into strings
        for (vector<string>::const_iterator i = sub1.begin(); i != sub1.end(); ++i) {
            substr1 += *i;
        }
        for (vector<string>::const_iterator i = sub2.begin(); i != sub2.end(); ++i) {
            substr2 += *i;
        }

        //hash lines
        size_t hash1 = hash_it(substr1);
        size_t hash2 = hash_it(substr2);

        //if not equal, slice into half to keep searching
        if(hash1 != hash2){
            if(end != start + 1){
                int mid = (start+end) / 2;
                recursive_line_comparison(lines1, lines2, start, mid,mismatched_lines);
                recursive_line_comparison(lines1, lines2, mid, end,mismatched_lines);
            }
            //we found the line (for example 0-1 indicates line 0)
            else{
                mismatched_lines.push_back(start);
            }
        }
    }
}

//Q5
void list_mismatched_lines(string file1, string file2){
    //vector to store line indices
    vector<int> mismatched_lines;

    //use helper to get lines as a vector
    vector<string> lines1 = getFileLines(file1);
    vector<string> lines2 = getFileLines(file2);
    
    //file name
    string base_1 = file1.substr(file1.find_last_of("/\\") + 1);
    string base_2 = file2.substr(file2.find_last_of("/\\") + 1);

    //get min number of lines to compare 
    //later on add the missing ones as empty for the smaller file
    int n = min(lines1.size(), lines2.size());
    
    //do the recursive comparison
    recursive_line_comparison(lines1, lines2, 0, n, mismatched_lines);

    //print the results
    for (int i = 0; i < mismatched_lines.size(); i++) {
        cout << base_1 << ": " << lines1[mismatched_lines[i]] << endl;
        cout << base_2 << ": " << lines2[mismatched_lines[i]] << endl;
    }

    //if one file was bigger than the other one, handle missing lines
    if(lines1.size() != lines2.size()){
        if(lines1.size() > lines2.size()){
            for(int j = n; j < lines1.size(); j++){
                cout << base_1 << ": " << lines1[j] << endl;
                cout << base_2 << ": " << endl;
            }
        }else{
            for(int j = n; j < lines2.size(); j++){
                cout << base_1 << ": " << endl;
                cout << base_2 << ": " << lines2[j] << endl;
            }
        }
    }
}

//helper for Q6
void recursive_word_comparison(vector<string> line1, vector<string> line2, int start, int end, vector<int>& mismatched_words){
    if (end >= start){
        //same logic as the other helper recursive function
        //convert vectors<string> to string
        string substr1;
        string substr2;

        vector<string> sub1 = vector<string>(line1.begin() + start, line1.begin() + end);
        vector<string> sub2 = vector<string>(line2.begin() + start, line2.begin() + end);
        for (vector<string>::const_iterator i = sub1.begin(); i != sub1.end(); ++i) {
            substr1 += *i;
        }
        for (vector<string>::const_iterator i = sub2.begin(); i != sub2.end(); ++i) {
            substr2 += *i;
        }
        
        //hash
        size_t hash1 = hash_it(substr1);
        size_t hash2 = hash_it(substr2);
        //if not equal, look into halves. if equal, skip and dont look more
        if(hash1 != hash2){
            if(end != start + 1){
                int mid = (start+end) / 2;
                recursive_word_comparison(line1, line2, start, mid,mismatched_words);
                recursive_word_comparison(line1, line2, mid, end,mismatched_words);
            }
            else{
                //add the index to vector
                mismatched_words.push_back(start);
            }
        }
    }

}

//Q6
void list_mismatched_words(string file1, string file2){
    //variables needed
    string mismatch_line1;
    string mismatch_line2;
    vector<int> mismatched_lines;
    vector<int> mismatched_words;
    vector<string> words1;
    vector<string> words2;

    //perform same thing as Q5 to get the lines
    vector<string> lines1 = getFileLines(file1);
    vector<string> lines2 = getFileLines(file2);

    //filenames
    string base_1 = file1.substr(file1.find_last_of("/\\") + 1);
    string base_2 = file2.substr(file2.find_last_of("/\\") + 1);

    int n = min(lines1.size(), lines2.size());
    //get mismatched lines
    recursive_line_comparison(lines1, lines2, 0, n, mismatched_lines);

    for (int i = 0; i < mismatched_lines.size(); i++) {
        mismatch_line1 = lines1[mismatched_lines[i]];
        mismatch_line2 = lines2[mismatched_lines[i]];

        //get words from those lines
        words1 = getWordsFromLine(mismatch_line1);
        words2 = getWordsFromLine(mismatch_line2);
        int l = min(words1.size(), words2.size());
        //perform the recursion to find differences in hashes
        recursive_word_comparison(words1, words2, 0, l, mismatched_words);

        for(int j = 0; j < mismatched_words.size(); j++){
            cout << base_1 << ": " << words1[mismatched_words[j]] << " (line " << mismatched_lines[i]+1 << ")"<< endl;
            cout << base_2 << ": " << words2[mismatched_words[j]] << " (line " << mismatched_lines[i]+1 << ")"<< endl;
        }

        //if a line is longer than the other one, there is for sure missing words
        if(words1.size() != words2.size()){
            if(words1.size() > words2.size()){
                for(int j = l; j < words1.size(); j++){
                    cout << base_1 << ": " << words1[j] << " (line " << mismatched_lines[i]+1 << ")"<< endl;
                    cout << base_2 << ": " << " (line " << mismatched_lines[i]+1 << ")"<< endl;
                }
            }else{
                for(int j = l; j < words2.size(); j++){
                    cout << base_1 << ": " << " (line " << mismatched_lines[i]+1 << ")"<< endl;
                    cout << base_2 << ": " << words2[j] << " (line " << mismatched_lines[i]+1 << ")"<< endl;
                }
            }
        }
        //clear the buffer(vector) for this function
        mismatched_words.clear(); 
    }

    //if a file is bigger than the other one, print each word that occurs in the bigger file to be a mismatch
    if(lines1.size() != lines2.size()){
        if(lines1.size() > lines2.size()){
            for(int j = n; j < lines1.size(); j++){
                words1 = getWordsFromLine(lines1[j]);
                for(int x = 0; x < words1.size(); x++){
                    cout << base_1 << ": " << words1[x] << " (line " << j+1 << ")"<< endl;
                    cout << base_2 << ": " << " (line " << j+1 << ")"<< endl;
                }
            }
        }else{
            for(int j = n; j < lines2.size(); j++){
                words2 = getWordsFromLine(lines2[j]);
                for(int x = 0; x < words2.size(); x++){
                    cout << base_1 << ": " << " (line " << j+1 << ")"<< endl;
                    cout << base_2 << ": " << words2[x] << " (line " << j+1 << ")"<< endl;
                }
            }
        }
    }
}

//CHANGES I MADE TO MAIN (MANDATORY FOR COMPILE)
//REMOVED BOOL'S FROM RESULT (REDECLARATION)
//CHANGED FUNCTION CALL LIST_MISMATCHED_LINES (LIST_MISMATCHED BEFORE)
int main() { 
    
    std::string str1 = "Hello World";
    std::string str2 = "hEllO World";
    std::string str3 = "World";
    std::string str4 = "Hello World";
    bool result = word_diff(str1, str2); // False 
    result = word_diff(str1, str3); // False
    result = word_diff(str1, str4); // True 
    
    std::string file1 = "./txt_folder/file1.txt";
    std::string file2 = "./txt_folder/file2.txt";
    result = classical_file_diff(file1, file2); // False
    
    std::string mystr = "I love this assignment";
    std::size_t h1 = hash_it (mystr);
    std::cout << h1 << std::endl;
    
    result = enhanced_file_diff(file1, file2); // False
    
    list_mismatched_lines(file1, file2); // This should print to the screen the mismatched lines

    list_mismatched_words(file1, file2); // This should print to the screen the mismatched words
}