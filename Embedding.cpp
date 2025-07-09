#include "Embedding.hpp"
#include "InvalidFileException.hpp"     
#include "InvalidDimensionsException.hpp" 
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>    
#include <iomanip>    
#include <cmath>     

Embedding::Embedding(const char* line) {
    std::string str(line);
    std::istringstream iss(str);
    std::string token;

    if (std::getline(iss, token, ',')) {
        word = token;
    }

    while (std::getline(iss, token, ',')) {
        v.push_back(std::stod(token));
    }
}

Embedding::Embedding(const std::string& line) {
    std::istringstream iss(line);
    std::string token;

    if (std::getline(iss, token, ',')) {
        word = token;
    }

    while (std::getline(iss, token, ',')) {
        v.push_back(std::stod(token));
    }
}

Embedding::Embedding(const Embedding& e){
    word = e.word;
    v = e.v;
}

std::string Embedding::getWord() const {
    return word;
}

std::vector<double> Embedding::getVector() const {
    return v;
}

int Embedding::getDimensions() const {
    return v.size();
}

Embedding& Embedding::operator=(const Embedding& e) {
    if (this != &e) {  
        word = e.word;
        v = e.v;
    }
    return *this;
}

Embedding Embedding::operator+(const Embedding& e) const {
    if (v.size() != e.v.size()) {
        throw InvalidDimensionsException();
    }

    Embedding result(*this);
    result.word = word + "+" + e.word;

    for (size_t i = 0; i < v.size(); i++) {
        result.v[i] += e.v[i];
    }

    return result;
}

Embedding Embedding::operator-(const Embedding& e) const {
    if (v.size() != e.v.size()) {
        throw InvalidDimensionsException();
    }

    Embedding result(*this);
    result.word = word + "-" + e.word;

    for (size_t i = 0; i < v.size(); i++) {
        result.v[i] -= e.v[i];
    }

    return result;
}

// Cosine Similarity
double Embedding::operator*(const Embedding& e) const {
    if (v.size() != e.v.size()) {
        throw InvalidDimensionsException();
    }

    double AB = 0.0;
    for (size_t i = 0; i < v.size(); i++) {
        AB += v[i] * e.v[i];
    }

    double normA = !(*this);
    double normB = !e;

    return AB / (normA * normB);
}

// Norm Embedding
double Embedding::operator!() const {
    double norm = 0.0;
    for (double val : v) {
        norm += val * val;
    }
    return sqrt(norm);
}

std::ostream& operator<<(std::ostream& stream, const Embedding& e) {
    stream << "[" << e.word << "] ";
    for (size_t i = 0; i < e.v.size(); i++) {
        stream << e.v[i];
        if (i != e.v.size() - 1) {
            stream << ", ";
        }
    }
    stream << "\n";
    return stream;
}

//Read CSV,save embeddings,check for Exceptions
std::vector<Embedding> readVectors(std::string filename) {
    std::ifstream file(filename);  
    if (!file.is_open()) {
        throw InvalidFileException();
    }

    std::vector<Embedding> embeddings;
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            embeddings.emplace_back(line); 
        }
    }

    if (!embeddings.empty()) {
        const int expected_dim = embeddings[0].getDimensions();
        for (const auto& e : embeddings) {
            if (e.getDimensions() != expected_dim) {
                throw InvalidDimensionsException();
            }
        }
    }

    return embeddings;
}
