#include <exception>
#include <ios>
#include <string>
#include <sstream>

namespace WFS{
    namespace Errors{
        class BaseError;
        class CannotOpenFile;
    }
}

class WFS::Errors::BaseError: public std::exception{
    public:
        BaseError(int e):fExitCode(e){};
        virtual ~BaseError()throw(){};
        virtual void What(std::stringstream& )=0;
        virtual const char* what();
        int Exit(int parent)const{return parent*100 + fExitCode;}

    private:
        int fExitCode;
};

class WFS::Errors::CannotOpenFile:public WFS::Errors::BaseError{
    CannotOpenFile(const std::string& f, int i):BaseError(i),fFile(f){};
    ~CannotOpenFile()throw(){}
    void What(std::stringstream& os){ os<<"Failed to open file: '"<<fFile<<"'";}
    private:
        std::string fFile;
};
