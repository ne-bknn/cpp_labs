#include <iostream>

#include "word_array.h"
#include "uilib/uilib.h"
#include <exception>
#include <algorithm>
#include <string>

WordArray::WordArray() {
}

WordArray::WordArray(const char* word) {
	add(word);
}

WordArray::WordArray(unsigned int n_words, const char** words) {
	for (unsigned int i = 0; i < n_words; ++i) {
		add(words[i]);
	}
}

void WordArray::add(const char* word) {
	if (this->_trie.contains(word)) {
		return;
	}

	this->_trie.add(word, this->_word_array.size());
	std::string str(word);
	this->_word_array.push_back(str);
}

int WordArray::index(const char* word) const {
	if (!this->_trie.contains(word)) {
		return -1;
	}

	return this->_trie.get_index(word);
}

std::string WordArray::get(unsigned int index) const {
	if (index+1 > this->_word_array.size()) {
		throw std::out_of_range("Index access out of range");
	}
	return this->_word_array[index];
}

WordArray* WordArray::prefix(const char* prefix) {
	WordArray* new_array = new WordArray;
	
	std::vector<unsigned int> indices = this->_trie.get_indices_by_prefix(prefix);
	for (std::vector<unsigned int>::iterator it = indices.begin(); it != indices.end(); ++it) {
		std::string word = this->get(*it);
		new_array->add(word.c_str());
	}
	return new_array;
}

void WordArray::sort() {
	std::sort(this->_word_array.begin(), this->_word_array.end());
	for (unsigned int i = 0; i < this->_word_array.size(); ++i) {
		this->_trie.set_index(this->_word_array[i].c_str(), i);
	}
}

unsigned int WordArray::length() const {
	return this->_word_array.size();
}

std::ostream& operator<<(std::ostream& os, const WordArray& word_array)
{
    for (unsigned int i = 0; i < word_array.length(); ++i) {
    	os << word_array.get(i) << "\n";
    }
    return os;
}

std::istream& operator>> (std::istream& is, WordArray& word_array)
{
    while (true) {
	try {
		std::string k = ui::get_str_custom(is);
		const char* r = k.c_str();
		word_array.add(r);
	} catch (std::runtime_error&) {
		break;
	}
    }
    return is;
}

auto WordArray::begin() {
	return this->_word_array.begin();
}

auto WordArray::end() {
	return this->_word_array.end();
}

WordArray WordArray::operator+(WordArray& b) {
	WordArray new_array;

	for (const auto& value: *this) {
		new_array.add(value.c_str());
	}

	for (const auto& value: b) {
		new_array.add(value.c_str());
	}
	return new_array;
}

bool WordArray::operator==(const WordArray& b) {
	if (this->length() != b.length()) {
		return false;
	}

	for (const auto& value: *this) {
		if (b.index(value.c_str()) == -1) {
			return false;
		}
	}
	return true;
}

bool WordArray::operator!=(const WordArray& b) {
	return ! (*this == b);
}

WordArray WordArray::operator-(const WordArray& b) {
	WordArray new_array;
	
	for (const auto& value: *this) {
		if (b.index(value.c_str()) == -1) {
			new_array.add(value.c_str());
		}
	}

	return new_array;
}

WordArray WordArray::operator&&(const WordArray& b) {
	WordArray new_array;
	
	for (const auto& value: *this) {
		if (b.index(value.c_str()) != -1) {
			new_array.add(value.c_str());
		}
	}

	return new_array;
}

std::string const& WordArray::operator[](unsigned int index) const {
	if (this->length() < index + 1) {
		throw std::out_of_range("Get out of bounds");
	}

	return this->_word_array[index];
}

