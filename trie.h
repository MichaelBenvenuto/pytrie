#ifndef SEARCH_TRIE
#define SEARCH_TRIE

#include <stdint.h>

#include <vector>

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
	size_t getChildrenCount() { return this->children_size; }

	TrieNode* getChildren() { return this->children; }

	uint8_t* serialize(size_t &size);

	int search(uint8_t);
protected:
	uint8_t weight;

private:
	size_t children_size;
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

	static Trie* deserialize(const uint8_t* serialized, const size_t size);

	uint8_t* serialize(size_t &size) { return this->root.serialize(size); }

private:
	TrieNode root;
};

#endif
