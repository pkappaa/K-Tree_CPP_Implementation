#ifndef __INVALID_DIMENSIONS_EXCEPTION_H__
#define __INVALID_DIMENSIONS_EXCEPTION_H__

class InvalidDimensionsException : public std::exception {
  
public:
  virtual const char* what() const noexcept {
    return " ----- Invalid Dimensions Exception -----\n";
  }
};

#endif


