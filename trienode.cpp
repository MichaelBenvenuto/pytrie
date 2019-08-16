#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include <vector>
#include <stack>
#include <map>

using namespace std;

TrieNode::TrieNode() {
	this->weight = 0;
	this->children = 0;
	this->children_size = 0;
}

TrieNode::TrieNode(uint8_t weight) {
	this->weight = weight;
	this->children = 0;
	this->children_size = 0;
}

TrieNode::TrieNode(const TrieNode& other) {
	this->weight = other.weight;

	if (this->children) {
		free(this->children);
		this->children = 0;
	}

	this->children = (TrieNode*)calloc(other.children_size, sizeof(TrieNode));

	if (this->children) {
		memcpy(other.children, this->children, this->children_size * sizeof(TrieNode));
		this->children_size = other.children_size;
	}
	else {
		this->children = 0;
		this->children_size = 0;
	}
}

TrieNode::~TrieNode() {
	for (unsigned int i = 0; i < this->children_size; i++) {
		if (this->children){
			this->children[i].~TrieNode();
		}
	}

	if (this->children) {
		free(this->children);
		this->children = 0;
	}
}

TrieNode& TrieNode::operator=(const TrieNode& other) {
	this->weight = other.weight;

	this->children_size = other.children_size;

	if (this->children) {
		free(this->children);
		this->children = 0;
	}

	this->children = (TrieNode*)calloc(other.children_size, sizeof(TrieNode));

	if (this->children) {
		memcpy(this->children, other.children, this->children_size * sizeof(TrieNode));
	}
	else {
		this->children_size = 0;
	}

	return *this;
}

TrieNode* TrieNode::add(const TrieNode& other) {

	if (this->children_size == 0) {
		this->children = (TrieNode*)calloc(1, sizeof(TrieNode));

		this->children_size = 1;

		if (!this->children) {
			this->children_size = 0;
			return this;
		}

		this->children[0] = other;

		return &this->children[0];
	}

	int exists = this->search(other.weight);

	if (exists >= 0){
		return &this->children[exists];
	}

	TrieNode* new_children = (TrieNode*)calloc((size_t)(this->children_size + 1), sizeof(TrieNode));

	if (!new_children) {
		return 0;
	}

	int mod = 0;
	int at = -1;

	for (unsigned int i = 0; i < this->children_size; i++){

		if (this->children[i].weight > other.weight && !mod){
			new_children[i + 1] = this->children[i];
			new_children[i] = other;
			mod = 1;
			at = i;
		}
		else if (i == this->children_size - 1 && !mod){
			new_children[i] = children[i];
			new_children[i + 1] = other;

			at = i + 1;
		}
		else{
			new_children[i + mod] = children[i];
		}

	}

	if (this->children){
		free(this->children);
	}

	this->children_size++;

	this->children = new_children;

	return &this->children[at];
	
}

TrieNode* TrieNode::traverse(uint8_t weight) {
	int find = this->search(weight);

	if (find < 0) {
		return 0;
	}

	return &this->children[find];
}

uint8_t* TrieNode::serialize(size_t &size) {
	vector<uint8_t> vec;
	stack<TrieNode*> nodes;

	nodes.push(this);

	map<TrieNode*, TrieNode*> parent;
	parent[this] = NULL;

	while (!nodes.empty()) {

		TrieNode* node = nodes.top();
		vec.push_back(node->getWeight());
		nodes.pop();

		if (node) {
			for (int i = node->children_size - 1; i >= 0; i--) {
				nodes.push(&node->children[i]);
				parent[&node->children[i]] = node;
			}

			if (node->getWeight() == 0 && vec.size() > 1) {
				uint16_t count = 0;
				TrieNode* current = node;
				while (parent[current]) {
					if (!nodes.empty()) {
						count++;
						if (parent[current]->traverse(nodes.top()->weight) == nodes.top()) {
							break;
						}
					}
					//vec.push_back(0);
					current = parent[current];
				}

				uint8_t* bytes_arr = (uint8_t*)&count;
				
				vec.push_back(bytes_arr[0]);
				vec.push_back(bytes_arr[1]);

			}
		}
	}

	uint8_t* arr = (uint8_t*)calloc(vec.size(), sizeof(*arr));

	if (!arr) {
		size = 0;
		return 0;
	}

	size = vec.size();

	memcpy(arr, vec.data(), vec.size() * sizeof(*arr));

	return arr;
}

int TrieNode::search(uint8_t weight) {
	int left = 0;
	int right = this->children_size - 1;

	while (left <= right) {
		int pivot = (left + right) / 2;
		if (this->children[pivot].weight < weight) {
			left = pivot + (size_t)1;
		}
		else if (this->children[pivot].weight > weight) {
			right = pivot - (size_t)1;
		}
		else {
			return pivot;
		}
	}

	return -1;
}
