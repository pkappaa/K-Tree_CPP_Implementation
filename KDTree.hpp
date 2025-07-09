#ifndef __KDTREE_HPP__
#define __KDTREE_HPP__

#include "KDNode.hpp"
#include "Distance.hpp"
#include <vector>
#include <iostream>
#include <queue>
#include <unordered_set>

class KDTree {
private:
  int size;
  KDNode* root;
  void collectUniqueEmbeddings(KDNode* node, std::vector<Embedding>& embeddings, std::unordered_set<std::string>& seen_words) const;

public: 
  KDTree(std::string filename);
  KDTree(const KDTree &ht);
  ~KDTree();

  std::vector<Distance> KNN_search(const Embedding& e, int k);

  KDTree& operator=(const KDTree &tree);
  KDTree& operator+=(const KDTree& tree);
  KDTree operator+(const KDTree& tree) const;

  friend std::ostream& operator<<(std::ostream &stream, const KDTree& t);
};

#endif 
