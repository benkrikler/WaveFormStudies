#ifndef LOADOLDWAVEFORMS_H
#define LOADOLDWAVEFORMS_H

#include "WaveformGenerator.h"

class TFile;
class TTree;

class LoadOldWaveforms:public WaveformGenerator{

    public:
        LoadOldWaveforms(modules::options* opts);
        virtual ~LoadOldWaveforms(){};

        virtual long int GetNumWFS();
        virtual int GetWaveform();

        void ConnectInput(std::string filename,std::string treename);

    private:
        std::string fInputFileName, fTreeName;
        TFile* fInFile;
        TTree* fInTree;

};

#endif //LOADOLDWAVEFORMS_H
