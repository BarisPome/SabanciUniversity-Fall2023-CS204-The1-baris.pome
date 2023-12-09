// Barış Pome
// 31311
// A-2 Section
// 2023-2024 Fall CS-204 HW1 

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
bool file_check(string& filename) { // in this function we check if the file is exist or not
    ifstream file(filename);
    return file.is_open();
}
void input_file(string& filename) {
    cout << "Enter the name of the file" << endl;  // in this function we get the name of the file from the user and check if the file is exist and can be opened or not
    cin >> filename;
    if (!file_check(filename)) {
        cout << "Could not open the file " << filename << endl;
    }
}
bool size_check(string& filename) { // in this function we check size of lines and columns in the matrix and check if the characters in the matrix are upper case or not
    ifstream file(filename);
    string line;
    getline(file, line);
    int size = stoi(line);
    for (unsigned int x = 0; x < size; x++) { // then we check if the size of the matrix is equal to the number of the lines in the file
        getline(file, line);
        if (line.size() != size) {
            return false;
        }
        for (unsigned int i = 0; i < size; i++) { // then we check if the characters in the matrix are upper case or not
            char character = line[i];
            if (!isupper(character)) {
                return false;
            }
        }
    }
    return true;
}
vector<vector<string> > create_matrix(string& filename) { // in this function we create the matrix from the file after checking the size of the matrix and the characters in the matrix
    ifstream file(filename);
    string line;
    getline(file, line);
    int size = stoi(line);
    vector<vector<string> > matrix_to_search(size, vector<string>(size));
    for (unsigned int a = 0; a < size; a++) { // we create the matrix from the file in a 2D vector
        getline(file, line);
        for (unsigned int b = 0; b < size; b++) {
            matrix_to_search[a][b] = line[b];
        }
    }
    return matrix_to_search;
}
vector<string> create_matrix_words(string& filename) {  // in this function we create the vector of the words that we want to find in the matrix
    ifstream file(filename);
    string line;
    getline(file, line);
    int size = stoi(line);
    vector<string> words_to_look;
    for (unsigned int i = 0; i < size; i++) { // we skip the lines that are the matrix
        getline(file, line);
    }
    getline(file, line);
    int word_count = stoi(line);  // we get the number of the words that we want to find in the matrix
    for (unsigned int count = 0; count < word_count; count++) { // we create the vector of the words that we want to find in the matrix
        getline(file, line);
        if (find(words_to_look.begin(), words_to_look.end(), line) == words_to_look.end()) { // we check if the word is already in the vector or not as we see in the example matrix_30_8.board
            words_to_look.push_back(line);
        }
    }
    return words_to_look;
}
vector<vector<int> > finding_possible_start_points(vector<vector<string> > matrix_to_search, string word_to_find) { // I think this function is important for efficiency of the program because it eliminates the points that we don't need to search
    vector<vector<int> > possible_start_points;
    int size = matrix_to_search.size();
    string first_letter = word_to_find.substr(0, 1); // we get the first letter of the word that we want to find in the matrix
    for (unsigned int index_row = 0; index_row < size; index_row++) {
        for (unsigned int index_colum = 0; index_colum < size; index_colum++) {
            if (matrix_to_search[index_row][index_colum] == first_letter) { // we check if the first letter of the word that we want to find in the matrix is in the matrix or not
                vector<int> start_point;
                start_point.push_back(index_row);
                start_point.push_back(index_colum);
                possible_start_points.push_back(start_point);
            }
        }
    }
    return possible_start_points;
}

// the functions below are the search algorithms that we use to find the words in the matrix. 4 of them are the clockwise, the other 4 are the reverse  of the first 4 or the counter clockwise
// I think these functions are important for efficiency of the program because they are the main part of the program that we search the words in the matrix

// for efficiency I check the size of the word that we want to find in the matrix and if the size of the word is bigger than the size of the matrix I don't search the word in the matrix
// and more importantly (word_to_find.substr(0, current_word.size()) == current_word) part checks in every step and stops the search immediately after seeing that the word that we search is not in the matrix

// while I was writing the code I realized that I can take help from modular arithmetic to make the code shorter and more efficient so I use modular arithmetic with %4 and %2
// %4 is for the directions (up down left right) and %2 is for the step_counter because the step_counter increases by 1 in every 2 cycles. The only change in the search algorithms is the directions, in other words the order of directions.

// I explained the logic of the search algorithms only in the first one because the logic of the others are the same with the first one. I just change the directions of the search algorithms
string search_algorithm1(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0; // I use cycle_count to change the directions of the search algorithms
    int step_counter = 1; // I use step_counter to increase the step in every cycle
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) { // This loop will continue as long as the current word matches a prefix of the word_to_find and is shorter than word_to_find.
        for (unsigned int step = 0; step < step_counter; step++) {                                                              // In every step we check if the current word matches a prefix of the word_to_find and is shorter than word_to_find. I thought that checking i every step helps to make the program more efficient.
            if (cycle_count % 4 == 0 && c > 0) { //move left
                c--;
            }
            else if (cycle_count % 4 == 1 && r > 0) { //move up
                r--;
            }
            else if (cycle_count % 4 == 2 && c < max_size - 1) { //move right
                c++;
            }else if (cycle_count % 4 == 3 && r < max_size - 1) { //move down
                r++;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) { // we increase the step_counter by 1 in every 2 cycles
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm2(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && c > 0) { //move left
                c--;
            }
            else if (cycle_count % 4 == 1 && r < max_size - 1 ) { //move down
                r++;
            }
            else if (cycle_count % 4 == 2 && c < max_size - 1) { //move right
                c++;
            }else if (cycle_count % 4 == 3 && r > 0) { //move up
                r--;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm3(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && c < max_size - 1) { //move right
                c++;
            }
            else if (cycle_count % 4 == 1 && r > 0) { //move up
                r--;
            }
            else if (cycle_count % 4 == 2 && c > 0) { //move left
                c--;
            }else if (cycle_count % 4 == 3 && r < max_size - 1) { //move down
                r++;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm4(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && c < max_size - 1) { //move right
                c++;
            }
            else if (cycle_count % 4 == 1 && r < max_size - 1 ) { //move down
                r++;
            }
            else if (cycle_count % 4 == 2 && c > 0) { //move left
                c--;
            }else if (cycle_count % 4 == 3 && r > 0) { //move up
                r--;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm5(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && r > 0) { //move up
                r--;
            }
            else if (cycle_count % 4 == 1 && c > 0) { //move left
                c--;
            }
            else if (cycle_count % 4 == 2 && r < max_size - 1) { //move down
                r++;
            }else if (cycle_count % 4 == 3 && c < max_size - 1) { //move right
                c++;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm6(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && r > 0) { //move up
                r--;
            }
            else if (cycle_count % 4 == 1 && c < max_size - 1) { //move right
                c++;
            }
            else if (cycle_count % 4 == 2 && r < max_size - 1) { //move down
                r++;
            }else if (cycle_count % 4 == 3 && c > 0) { //move left
                c--;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm7(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && r < max_size - 1) { //move down
                r++;
            }
            else if (cycle_count % 4 == 1 && c > 0) { //move left
                c--;
            }
            else if (cycle_count % 4 == 2 && r > 0) { //move up
                r--;
            }else if (cycle_count % 4 == 3 && c < max_size - 1) { //move right
                c++;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
string search_algorithm8(vector<vector<string> > matrix_to_search, int r, int c, string word_to_find) {
    int max_size = matrix_to_search.size();
    int cycle_count = 0;
    int step_counter = 1;
    string current_word = matrix_to_search[r][c];
    while (word_to_find.substr(0, current_word.size()) == current_word && current_word.size() < word_to_find.size()) {
        for (unsigned int step = 0; step < step_counter; step++) {
            if (cycle_count % 4 == 0 && r < max_size - 1) { //move down
                r++;
            }
            else if (cycle_count % 4 == 1 && c < max_size - 1) { //move right
                c++;
            }
            else if (cycle_count % 4 == 2 && r > 0) { //move up
                r--;
            }else if (cycle_count % 4 == 3 && c > 0) { //move left
                c--;
            }
            current_word += matrix_to_search[r][c];
            if (current_word.size() == word_to_find.size()) {
                break;
            }
        }
        cycle_count++;
        if (cycle_count % 2 == 0) {
            step_counter++;
        }
    }
    return current_word;
}
int main() {
    string filename;
    do {
        input_file(filename);
    } while (!file_check(filename)); // we get the name of the file from the user and check if the file is exist or not and continue to get the name of the file until the file is exist
    if (!size_check(filename)) { // we check the size of the matrix and the characters in the matrix
        cout << "Error: Input file is not in correct format!" << endl;
        return 0;
    }
    else {
        vector<vector<string> > matrix_to_search = create_matrix(filename);
        vector<string> words_to_find = create_matrix_words(filename);
        vector<string> founded_words;
        for (unsigned int i= 0; i < words_to_find.size(); i++) { // we search the words in the matrix and firstly we find the possible start points of the words in the matrix
            vector<vector<int> > possible_start_points = finding_possible_start_points(matrix_to_search, words_to_find[i]);
            for (unsigned int index_points = 0; index_points < possible_start_points.size(); index_points++ ) { // I use break in this loop because I don't want to search the same word again in the matrix after finding the word
                vector<int> start_point = possible_start_points[index_points];                                  // in sample matrix_50_10.board I realized that there are some words that are in the matrix more than once
                int start_row = start_point[0];
                int start_colum = start_point[1];
                if (search_algorithm1(matrix_to_search, start_row, start_colum, words_to_find[i]) == words_to_find[i]) { // we search the words in the matrix with 8 different search algorithms
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm2(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm3(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm4(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm5(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm6(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm7(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                } else if (search_algorithm8(matrix_to_search, start_row, start_colum, words_to_find[i]) ==
                           words_to_find[i]) {
                    founded_words.push_back(words_to_find[i]);
                    break;
                }
            }
        }
        if (founded_words.size() > 0) { // we print the words that we found in the matrix
            if (founded_words.size() == 1) { // we check if the number of the words that we found in the matrix is 1 or more than 1
                cout << "1 Word is found:  "; // I did not encounter with that in samples but I think it can be seen that there is only 1 word in the matrix.
                cout << founded_words[0] << endl; // I did not want to see that 1 Words are found.
            }
            else {
                cout << founded_words.size() << " Words are found:  "; // we print the words that we found in the matrix
                for (unsigned int index_founded_words = 0; index_founded_words < founded_words.size(); index_founded_words++) { //
                    cout << founded_words[index_founded_words] << " ";
                }
            }
        }
        return 0;
    }
}

