#include "KDTree.hpp"
#include "KDNode.hpp"
#include "Embedding.hpp"
#include "Distance.hpp"
#include "InvalidFileException.hpp"
#include "InvalidDimensionsException.hpp"

#include <vector>
#include <queue>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <functional>
#include <string>


void KDTree::collectUniqueEmbeddings(KDNode* node, std::vector<Embedding>& embeddings, std::unordered_set<std::string>& seen_words) const {
    if (!node) return;
    
    const auto& word = node->getEmbedding().getWord();
    if (seen_words.insert(word).second) {  
        embeddings.push_back(node->getEmbedding());
    }
    
    collectUniqueEmbeddings(node->getLeft(), embeddings, seen_words);
    collectUniqueEmbeddings(node->getRight(), embeddings, seen_words);
}

// Helper function: sort embeddings, create left,right sublists
KDNode* buildKDTree(vector<Embedding>& embeddings, int depth, KDNode* parent) {
    if (embeddings.empty()) 
		return nullptr;

    int dims = embeddings[0].getDimensions();           
    int axis = depth % dims;

    sort(embeddings.begin(), embeddings.end(), [axis](const Embedding& a, const Embedding& b) {
        return a.getVector()[axis] < b.getVector()[axis];
    });

	int medianIndex = (embeddings.size() - 1) / 2;
    Embedding medianEmbedding = embeddings[medianIndex];
	
	std::vector<Embedding> leftEmbeds(embeddings.begin(), embeddings.begin() + medianIndex);
    std::vector<Embedding> rightEmbeds(embeddings.begin() + medianIndex + 1, embeddings.end());

    KDNode* node = new KDNode(medianEmbedding, axis, nullptr, nullptr, parent);
    node->setLeft(buildKDTree(leftEmbeds, depth + 1, node));
    node->setRight(buildKDTree(rightEmbeds, depth + 1, node));

    return node;
}

KDTree::KDTree(std::string filename) {
    std::vector<Embedding> embeddings = readVectors(std::string(filename));
    
    if (embeddings.empty()) {
        throw InvalidFileException();  
    }

    size = embeddings.size();
    root = buildKDTree(embeddings, 0, nullptr);
}

KDTree::KDTree(const KDTree& ht) {
    if (ht.root) {
        root = new KDNode(*ht.root);
    } else {
        root = nullptr;
    }
    size = ht.size;
}

KDTree::~KDTree() {
    delete root;
}

// Assignment operator
KDTree& KDTree::operator=(const KDTree& tree) {
    if (this == &tree) 
		return *this;
    
	delete root;
    root = tree.root ? new KDNode(*tree.root) : nullptr;
    size = tree.size;
    return *this;
}

// Merge operator
KDTree KDTree::operator+(const KDTree& tree) const {
    KDTree result = *this;  
    result += tree;         
    return result;
}

// Merge assign operator
KDTree& KDTree::operator+=(const KDTree& tree) {
    if (this->root->getEmbedding().getDimensions() != tree.root->getEmbedding().getDimensions()) {
        throw InvalidDimensionsException();
    }

    std::vector<Embedding> combined;
    std::unordered_set<std::string> seen_words;
    
    this->collectUniqueEmbeddings(this->root, combined, seen_words);
    
    this->collectUniqueEmbeddings(tree.root, combined, seen_words);
    
    this->root = buildKDTree(combined, 0, nullptr);
    
    return *this;
}

// KNN Helper: find distance,insert in heap, decide the subtree to continue
void knnSearch(KDNode* node, const Embedding& target, int k, int depth, std::priority_queue<Distance>& maxHeap) {
    if (!node) return;

    int axis = depth % target.getDimensions();
    double dist = !(target - node->getEmbedding());

    Distance temp(dist, node->getEmbedding().getWord());
    std::cout << "Adding: " << temp << '\n';
    maxHeap.push(temp);
    
    if ((int)maxHeap.size() > k) {
        std::cout << "Removing: " << maxHeap.top() << '\n';
        maxHeap.pop();
    }

	
	KDNode* next = nullptr;
    KDNode* other = nullptr;

    if (target.getVector()[axis] < node->getEmbedding().getVector()[axis]) {
        next = node->getLeft();
        other = node->getRight();
    } else {
        next = node->getRight();
        other = node->getLeft();
    }

    knnSearch(next, target, k, depth + 1, maxHeap);

    double axisDist = fabs(target.getVector()[axis] - node->getEmbedding().getVector()[axis]);
    if ((int)maxHeap.size() < k || axisDist * axisDist < maxHeap.top().distance) {
        knnSearch(other, target, k, depth + 1, maxHeap);
    }
}

// KNN search 
vector<Distance> KDTree::KNN_search(const Embedding& e, int k) {
    std::priority_queue<Distance> maxHeap;
    knnSearch(root, e, k, 0, maxHeap);

    vector<Distance> result;
    while (!maxHeap.empty()) {
        result.push_back(maxHeap.top());
        maxHeap.pop();
    }

    return result;
}

// Stream output
std::ostream& operator<<(std::ostream& stream, const KDTree& t) {
    std::function<void(KDNode*)> print = [&](KDNode* node) {
        if (!node) return;
        stream << node->getEmbedding().getWord() << '\n';
        print(node->getLeft());
        print(node->getRight());
    };

    print(t.root);
    return stream;
}
  
  
