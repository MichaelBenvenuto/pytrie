#include "trie.h"

#include <stdio.h>

#include <string>
#include <stack>
#include <map>

using namespace std;

Trie::Trie() {
}

Trie::Trie(const Trie& other) {
	this->root = other.root;
}

Trie::~Trie() {
	root.~TrieNode();
}

Trie& Trie::operator=(const Trie& other) {
	this->root = other.root;
	return *this;
}

bool Trie::addWord(const char* word) {
	size_t length = strlen(word) + 1;

	TrieNode* current = &this->root;

	for (int i = 0; i < length; i++) {
		current = current->add(TrieNode(word[i]));

		if (!current) {
			return false;
		}
	}

	return true;
}

char** Trie::prefixSearch(const char* prefix, size_t &size) {
	TrieNode* temp_root = &this->root;

	size_t length = strlen(prefix);

	stack<TrieNode*> s;
	stack<string> str_stack;

	for (int i = 0; i < length; i++) {
		temp_root = temp_root->traverse(prefix[i]);

		if (!temp_root){
			return 0;
		}
	}

	s.push(temp_root);

	map<TrieNode*, TrieNode*> parent;

	parent[temp_root] = 0;

	string t_prefix = prefix;
	t_prefix.pop_back();

	while (!s.empty()){

		TrieNode* node = s.top();
		s.pop();

		if (node->getWeight() == 0){
			TrieNode* current = parent[node];
			
			stack<char> suffix;
			
			while (current){
				suffix.push(current->getWeight());
				current = parent[current];
			}

			str_stack.push(t_prefix);

			while (!suffix.empty()){
				str_stack.top() += suffix.top();
				suffix.pop();
			}
		}

		for (int i = node->getChildrenCount() - 1; i >= 0; i--){
			parent[&node->getChildren()[i]] = node;
			s.push(&node->getChildren()[i]);
		}
	}


	size = (int)str_stack.size();

	char** strings = (char**)calloc(size, sizeof(*strings));

	int i = 0;
	while (!str_stack.empty()){
		string s = str_stack.top();
		str_stack.pop();

		strings[i] = (char*)malloc((s.size() + 1) * sizeof(*strings[i]));
		memcpy(strings[i], s.c_str(), s.size() + 1);
		i++;
	}

	return strings;
}
