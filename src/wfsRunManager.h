#include "wfsDefinitions.h"
#include "ModulesOptions.h"
#include "wfsWaveform.h"

class TFile;

namespace WFS{
    class RunManager;
}

class WFS::RunManager{

        RunManager();
        ~RunManager(){
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

        TFile* GetOutFile()const {return fOutFile;}

        static void SetGlobalWaveform(const std::string& name, WFS::WaveForm& waveform){
            Instance()->fGlobalWaveforms[name]=&waveform;
        }

    private:
        static RunManager* fThis;
        modules::options fCommandLine;
        TFile* fOutFile;

        std::map<std::string, WFS::WaveForm*> fGlobalWaveforms;
};
