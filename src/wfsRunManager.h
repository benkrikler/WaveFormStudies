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

        long int NumberEventsToProcess()const{return fProcessNumEvents;}
        void NumberEventsToProcess(long int n){fProcessNumEvents=n;}

        static void SetGlobalWaveform(const std::string& name,const WFS::WaveForm& waveform){
            Instance()->fGlobalWaveforms[name]=&waveform;
        }

        static const WFS::WaveForm* GetGlobalWaveform(const std::string& name){
            std::map<std::string, const WFS::WaveForm*>::iterator i_wave= Instance()->fGlobalWaveforms.find(name);
            if(i_wave==Instance()->fGlobalWaveforms.end()) return NULL;
            return i_wave->second;
        }


    private:
        static RunManager* fThis;
        modules::options fCommandLine;
        TFile* fOutFile;
        long unsigned int fProcessNumEvents;

        std::map<std::string, const WFS::WaveForm*> fGlobalWaveforms;
};
