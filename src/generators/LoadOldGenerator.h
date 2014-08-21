#ifndef GENERATORS_LOADOLDGENERATOR_H
#define GENERATORS_LOADOLDGENERATOR_H

#include "WaveformGenerator.h"
class TFile;
class TTree;

class LoadOldGenerator:public WaveformGenerator {
    public:
        LoadOldGenerator(generators::options* opts);
        ~LoadOldGenerator(){}

        virtual int Initialise();
        virtual const WFS::WaveForm& GetWaveform();
        virtual int Finalize();

        virtual long int GetNumWFS(){return fNumEntries - fFirstEntry;}

    private:
        std::string fInputFileName;
        std::string fTreeName;
        TFile* fInFile;
        TTree* fInTree;
        long int fCurrentEntry, fFirstEntry;
        long int fNumEntries;
        int fChannel;

        enum { fNoSamples=1024};
        Float_t fInX[fNoSamples];
        Float_t fInY[fNoSamples];

        WFS::WaveForm fOutWaveform;

};

#endif // GENERATORS_LOADOLDGENERATOR_H
