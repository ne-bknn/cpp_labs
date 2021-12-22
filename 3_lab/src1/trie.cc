#include "trie.h"
#include "string.h"
#include <exception>
#include <stdexcept>
#include <vector>
#include <iostream>

Trie::Trie() noexcept {
	this->root = make_trie_node();
}

void Trie::copy(struct TrieNode* root, struct TrieNode* new_root) {
	if (nullptr == root) {
		return;
	}

	new_root->is_leaf = root->is_leaf;
	new_root->index = root->index;
	for (unsigned int i = 0; i < 256; ++i) {
		if (nullptr != root->children[i]) {
			new_root->children[i] = make_trie_node();
			copy(root->children[i], new_root->children[i]);
		}
	}
}

Trie::Trie(const Trie &obj) noexcept {
	this->root = make_trie_node();
	copy(obj.root, this->root);
}

Trie::Trie(Trie &&obj) noexcept {
	this->root = obj.root;
	obj.root = nullptr;
}

// copy
Trie& Trie::operator=(const Trie &obj) noexcept {
	if (this != &obj) {
		free_trie(this->root);
		this->root = make_trie_node();
		copy(obj.root, this->root);
	}

	return *this;
}

// move
Trie& Trie::operator=(Trie&& obj) noexcept {
	if (this != &obj) {
		this->root = obj.root;
		obj.root = nullptr;
	}
	return *this;
}

Trie::~Trie() {
	free_trie(this->root);
}

void Trie::free_trie(struct TrieNode* root) {
	if (nullptr == root) {
		return;
	}
	for (unsigned int i = 0; i < 256; ++i) {
		if (nullptr != root->children[i]) {
			free_trie(root->children[i]);
		}
	}
	delete[] root->children;
	delete root;
}

struct TrieNode* Trie::make_trie_node() {
	struct TrieNode* node = new struct TrieNode;
	node->children = new struct TrieNode*[256];
	for (int i = 0; i < 256; ++i) {
		node->children[i] = nullptr;
	}
	node->is_leaf = false;
	return node;
}

void Trie::add(const char* word, int index) {
	if (nullptr == word) {
		return;
	}
	
	unsigned int word_len = strlen(word);
	struct TrieNode* current_node = this->root;
	for (unsigned int i = 0; i < word_len; ++i) {
		char current_symbol = word[i];
		if (nullptr == current_node->children[current_symbol]) {
			current_node->children[current_symbol] = make_trie_node();
		}
		current_node = current_node->children[current_symbol];
	}
	current_node->is_leaf = true;
	current_node->index = index;
}

struct TrieNode* Trie::get(const char* word) const {
	if (nullptr == word) {
		return nullptr;
	}

	unsigned int word_len = strlen(word);
	struct TrieNode* current_node = this->root;
	for (unsigned int i = 0; i < word_len; ++i) {
		char current_symbol = word[i];
		if (nullptr == current_node->children[current_symbol]) {
			return nullptr;
		} else {
			current_node = current_node->children[current_symbol];
		}
	}
	if (nullptr != current_node && current_node->is_leaf) {
		return current_node;
	} else {
		return nullptr;
	}
}

bool Trie::contains(const char* word) const {
	return nullptr != get(word);
}

int Trie::get_index(const char* word) const {
	struct TrieNode* node = get(word);
	if (nullptr == node) {
		throw "Trie does not contain this word";
	}
	return node->index;
}

std::vector<unsigned int> Trie::traverse(struct TrieNode* root) const {
	std::vector<unsigned int> indices;
	if (root->is_leaf) {
		indices.push_back(root->index);
	}
	
	for (unsigned int i = 0; i < 256; ++i) {
		if (nullptr != root->children[i]) {
			std::vector<unsigned int> new_indices = traverse(root->children[i]);
			indices.reserve(indices.size() + distance(new_indices.begin(), new_indices.end()));
			indices.insert(indices.end(),new_indices.begin(),new_indices.end());
		}
	}

	return indices;
}

std::vector<unsigned int> Trie::get_indices_by_prefix(const char* prefix) const {
	std::vector<unsigned int> indices;
	unsigned int word_len = strlen(prefix);
	struct TrieNode* current_node = this->root;
	for (unsigned int i = 0; i < word_len; ++i) {
		char current_symbol = prefix[i];
		if (nullptr == current_node->children[current_symbol]) {
			return indices;
		} else {
			current_node = current_node->children[current_symbol];
		}
	}

	indices = traverse(current_node);
	return indices;
}

void Trie::set_index(const char* word, unsigned int index) {
	struct TrieNode* node = this->get(word);
	if (nullptr != node) {
		node->index = index;
	} else {
		throw std::out_of_range("Trie does not contain this word");
	}
}
