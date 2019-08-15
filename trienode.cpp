#include "trie.h"

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

static int constructed = 0;

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

	TrieNode* new_children = (TrieNode*)calloc(this->children_size + 1, sizeof(TrieNode));

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

int TrieNode::search(uint8_t weight) {
	int left = 0;
	int right = this->children_size - 1;

	while (left <= right) {
		int pivot = (left + right) / 2;
		if (this->children[pivot].weight < weight) {
			left = pivot + 1;
		}
		else if (this->children[pivot].weight > weight) {
			right = pivot - 1;
		}
		else {
			return pivot;
		}
	}

	return -1;
}
