#ifndef SEARCH_TRIE
#define SEARCH_TRIE

#include <stdint.h>

class TrieNode {
public:
	TrieNode();
	TrieNode(uint8_t);
	TrieNode(const TrieNode&);
	~TrieNode();

	TrieNode& operator=(const TrieNode&);

	TrieNode* add(const TrieNode&);

	TrieNode* traverse(uint8_t);
	
	uint8_t getWeight() { return this->weight; }
	unsigned int getChildrenCount() { return this->children_size; }

	TrieNode* getChildren() { return this->children; }


	int search(uint8_t);
protected:
	uint8_t weight;

private:
	unsigned int children_size;
	TrieNode* children;
};

class Trie {
public:
	Trie();
	Trie(const Trie&);
	~Trie();

	Trie& operator=(const Trie&);

	bool addWord(const char* word);

	char** prefixSearch(const char*, size_t &size);

	TrieNode* getRoot() { return &this->root; }

private:
	TrieNode root;
};

#endif
