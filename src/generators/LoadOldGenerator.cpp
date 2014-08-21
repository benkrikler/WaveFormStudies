#include "LoadOldGenerator.h"
#include "GeneratorsFactory.h"
#include "wfsLog.h"

#include <TTree.h>
#include <TFile.h>

using std::cout;
using std::endl;

LoadOldGenerator::LoadOldGenerator(generators::options* opts):
    WaveformGenerator("LoadOld",opts),
    fInFile(NULL),fInTree(NULL),fNumEntries(-1){
        fInputFileName=opts->GetString("input_file");
        fTreeName=opts->GetString("tree_name","t1");
        fFirstEntry=opts->GetInt("first_event",0);
        fChannel=opts->GetInt("channel",2,"x==1 || x==2 ");
    }

int LoadOldGenerator::Initialise(){
    if(fInputFileName.empty()){
        WFSErr(WFS::Logger::kQuiet)
            <<"No input file given"<<endl;
        return 1;
    }

    fInFile=TFile::Open(fInputFileName.c_str(),"read");
    if(!fInFile) {
        WFSErr(1) <<"Unable to open input file: '"<<fInputFileName<<"'"<<endl;
        return 2;
    }

    fInTree=(TTree*)fInFile->GetObjectChecked(fTreeName.c_str(),"TTree");
    if(!fInTree) {
        WFSErr(1) <<"Unable to find input tree: '"<<fTreeName<<"'"<<endl;
        return 3;
    }
    int ret_val1=fInTree->SetBranchAddress(Form("xc%d",fChannel), fInX);
    int ret_val2=fInTree->SetBranchAddress(Form("yc%d",fChannel), fInY);
    if(ret_val1 < 0 || ret_val2<0){
        WFSErr(1)<<"Cannot find one of branches xc"<<fChannel<<" yc"<<fChannel<<" in tree "<<fTreeName<<endl;
        return 4;
    }
    fInTree->GetEntry();

    fNumEntries=fInTree->GetEntries();
    WFSOut(2, "OldInputsToWaveforms")<<"Input file has "
        <<fNumEntries<<" waveforms to convert "<<endl;

    fCurrentEntry=fFirstEntry-1;

    return 0;
}

const WFS::WaveForm& LoadOldGenerator::GetWaveform(){
    // load next event
    ++fCurrentEntry;
    fInTree->GetEntry(fCurrentEntry);

    // Loop over each channel
    fOutWaveform.SetTicks(fInX);
    fOutWaveform.SetSamples(fInY);

    return fOutWaveform;
}

int LoadOldGenerator::Finalize(){
    fInFile->Close();
    return 0;
}

REGISTER_GENERATOR(LoadOld,input_file, tree_name);
