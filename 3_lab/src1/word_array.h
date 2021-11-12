#pragma once

#include "trie.h"
#include <vector>
#include <iostream>
#include <iterator>

class WordArray {
  private:
    std::vector<std::string> _word_array;
    Trie _trie;
  
  public:
    // constructors
    explicit WordArray();
    explicit WordArray(const char* word);
    explicit WordArray(unsigned int n_words, const char** words);

    // functions
    void add(const char* word);
    int index(const char* word) const;
    std::string get(unsigned int index) const;
    WordArray* prefix(const char* prefix);
    void sort();
    unsigned int length() const;
	
    // io
    friend std::ostream& operator<<(std::ostream& os, const WordArray& word_array);
    friend std::istream& operator>>(std::istream& is, WordArray& word_array);
    // iteration
    auto begin();
    auto end();

    // misc ops
    WordArray operator+(WordArray& b);
    bool operator==(const WordArray& b);
    bool operator!=(const WordArray& b);
    WordArray operator-(const WordArray& b);
    WordArray operator&&(const WordArray& b);

    // access op
    std::string const& operator[](unsigned int index) const; // no assignment
};
