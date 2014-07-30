#ifndef WFS_LOG_H
#define WFS_LOG_H

#include <iostream>
#include <iomanip>

namespace WFS{
    class Logger;
}

class WFS::Logger {
    public:
    enum Level_t{ kQuiet=1, kTimid=2, kOutgoing=3, kShowoff=4};

    static Logger* Instance(){
        if(!fThis) fThis=new Logger();
        return fThis;
    }

    private:
        Logger();
        ~Logger();
        Logger(const Logger&);
        void operator=(const Logger&){};

    public:

        Level_t OutVerbosity()const { return fOutVerbosity;}
        Level_t ErrVerbosity()const { return fErrVerbosity;}
        void OutVerbosity(Level_t l){ fOutVerbosity=l;}
        void ErrVerbosity(Level_t l){ fErrVerbosity=l;}
        void MoreOutput(){ IncremementVerbosity(fOutVerbosity, 1);}
        void MoreError() { IncremementVerbosity(fErrVerbosity, 1);}
        void LessOutput(){ IncremementVerbosity(fOutVerbosity,-1);}
        void LessError() { IncremementVerbosity(fErrVerbosity,-1);}

        std::ostream& Output( Level_t level,const std::string& header=""){
            return Write(std::string(level,'%')+' '+header,level,fOutVerbosity,GetOutStream());
        }
        std::ostream& Error ( Level_t level,const std::string& header=""){
            return Write(header,level,fErrVerbosity,GetErrStream());
        }

    private:
        std::ostream& Write(const std::string& header, Level_t level, Level_t current, std::ostream& os){
            if(level<current) {
                if(!header.empty()) os<<header<<": ";
                return os<<std::boolalpha;
            }
            return *(std::ostream*)fNull;
        }

        std::ostream& GetOutStream()const { return fOut?*(std::ostream*)fOut:std::cout;}
        std::ostream& GetErrStream()const { return fErr?*(std::ostream*)fErr:std::cerr;}

        void IncremementVerbosity(Level_t& l, int amount);
            
    private:
        std::ofstream* fOut;
        std::ofstream* fErr;
        std::ofstream* fNull;
        Level_t fOutVerbosity;
        Level_t fErrVerbosity;

        static Logger* fThis;

};

#define WFSOut(level, ...) \
    WFS::Logger::Instance()->Output( (WFS::Logger::Level_t) level, ##__VA_ARGS__  )

#define WFSErr3(level,lineno) \
    WFS::Logger::Instance()->Error( (WFS::Logger::Level_t) level, "Error: "__FILE__ "(" #lineno ")" )

#define WFSErr2(level,lineno) WFSErr3(level,lineno) 

#define WFSErr(level) WFSErr2(level,__LINE__)

#endif //WFS_LOG_H
