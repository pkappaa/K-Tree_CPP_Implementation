#include "Distance.hpp"
#include <iostream>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const Distance& d) {
    // Save original stream state
    std::ios_base::fmtflags original_flags = os.flags();
    std::streamsize original_precision = os.precision();
    
    // Apply new formatting
    os << std::fixed << std::setprecision(5) 
       << d.distance << " [" << d.word << "]";
    
    // Restore original stream state
    os.flags(original_flags);
    os.precision(original_precision);
    
    return os;
}
