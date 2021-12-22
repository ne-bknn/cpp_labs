#pragma once

#include <stdbool.h>
#include <vector>

struct TrieNode {
	bool is_leaf;
	struct TrieNode **children;
	unsigned int index;
};

class Trie {
  private:
    struct TrieNode* root;

    struct TrieNode* make_trie_node();
    struct TrieNode* get(const char* word) const;
    void free_trie(struct TrieNode* root);
    std::vector<unsigned int> traverse(struct TrieNode* root) const;
    void copy(struct TrieNode* root, struct TrieNode* new_root);
  public:
    explicit Trie() noexcept;
    Trie(const Trie &obj) noexcept;
    Trie(Trie &&obj) noexcept;
    Trie& operator=(const Trie &obj) noexcept;
    Trie& operator=(Trie&& obj) noexcept;
    ~Trie();

    void add(const char* word, int index);
    bool contains(const char* word) const;
    int get_index(const char* word) const;
    void set_index(const char* word, unsigned int index);
    std::vector<unsigned int> get_indices_by_prefix(const char* prefix) const;
};
