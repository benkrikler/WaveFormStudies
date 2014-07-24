#include "wfsDefinitions.h"
#include "ModulesOptions.h"

namespace WFS{
    class RunManager;
}

class WFS::RunManager{

        RunManager():fCommandLine("RunManager"){}
        ~RunManager(){
            if(fThis)delete fThis;
            fThis=NULL;
        }

    public:
        static RunManager* Instance(){
            if(!fThis) fThis=new RunManager();
            return fThis;
        }
        void ProcessCommandLine(int argc,char* argv[]);
        void OpenOutputs();
        void RunEventLoop();
        void Abort();

        std::string GetModulesFileName(){
            return fCommandLine.GetString("module_file","MODULES.txt");
        };


    private:
        static RunManager* fThis;
        modules::options fCommandLine;
};
