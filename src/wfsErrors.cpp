#include "wfsErrors.h"

const char* WFS::Errors::BaseError::what(){
    std::stringstream ss;
    What(ss);
    return ss.str().c_str();
}
