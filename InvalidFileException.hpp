#ifndef __INVALID_FILE_EXCEPTION_H__
#define __INVALID_FILE_EXCEPTION_H__

class InvalidFileException : public std::exception {
  
public:
  virtual const char* what() const noexcept {
    return " ----- Invalid File Exception -----\n";
  }
};

#endif

