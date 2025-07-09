#ifndef __EMBEDDING_HPP__
#define __EMBEDDING_HPP__
#include <string>
#include <vector>

using std::string;
using std::vector;

class Embedding {
private:
  string word;
  vector<double> v;  
  
public:  
  Embedding(const char* line);
  Embedding(const std::string& line);
  Embedding(const Embedding& e);

  string getWord() const;
  vector<double> getVector() const;
  int getDimensions() const;

  Embedding& operator = (const Embedding& e);
  
  Embedding operator + (const Embedding& e) const;
  Embedding operator - (const Embedding& e) const;
  double    operator * (const Embedding& e) const;

  double    operator ! () const;
  friend std::ostream& operator<<(std::ostream& stream, const Embedding& e);
};

std::vector<Embedding> readVectors(const std::string filename);

#endif

