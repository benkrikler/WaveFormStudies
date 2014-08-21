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
        BaseError(int e , const std::string& w=""):fExitCode(e),fWhat(w){};
        BaseError(const BaseError& rhs){
            fExitCode=rhs.fExitCode;
            fWhat.clear();
            fWhat.str(rhs.fWhat.str());
        };
        virtual ~BaseError()throw(){};
        virtual const char* what(){return fWhat.str().c_str();}
        std::stringstream& SetWhat(){return fWhat;}
        int Exit(int parent)const{return parent*100 + fExitCode;}

    protected: 
        void SetExitCode(int i ){fExitCode=i;}
    private:
        int fExitCode;
        std::stringstream fWhat;
};

class WFS::Errors::CannotOpenFile:public WFS::Errors::BaseError{
    CannotOpenFile(const std::string& f, int i):
        BaseError(i,"Failed to open file: '"+f+"'"){};
    ~CannotOpenFile()throw(){}
};
