
#ifndef DISTANCE_HPP
#define DISTANCE_HPP

#include <string>
#include <iostream>
#include <iomanip>

struct Distance {
    double distance;
    std::string word;
    
    Distance(double d, const std::string& w) : distance(d), word(w) {}
    
    bool operator<(const Distance& other) const {
        return distance < other.distance;
    }
    
    friend std::ostream& operator<<(std::ostream& stream, const Distance& d);
};

#endif 
