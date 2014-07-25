#include "OldInputsToWaveforms.h"
#include "ModulesFactory.h"
#include "ModulesOptions.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"
#include "wfsRunManager.h"

#include <TFile.h>
#include <TTree.h>
#include <string>

using std::endl;
using std::cout;

OldInputsToWaveforms::OldInputsToWaveforms(modules::options* opts):
   BaseModule("OldInputsToWaveforms",opts),fInFile(NULL),fInTree(NULL){
       fInputFileName=opts->GetString("input_file");
       fTreeName=opts->GetString("tree_name","t1");
}

OldInputsToWaveforms::~OldInputsToWaveforms(){
}

int OldInputsToWaveforms::BeforeFirstEntry(){
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
    fInTree->SetBranchAddress("xc0", fInX[0]);
    fInTree->SetBranchAddress("xc1", fInX[1]);
    fInTree->SetBranchAddress("yc0", fInY[0]);
    fInTree->SetBranchAddress("yc1", fInY[1]);
    fInTree->GetEntry();

    // Now make the tree for the converted objects
    GetDirectory()->cd();
    std::string title="Input waveforms from file ";
    title+=fInputFileName;
    fOutTree=new TTree("Input_waveforms",title.c_str());
    fOutTree->Branch("ch0",&fOutWaveform[0] );
    fOutTree->Branch("ch1",&fOutWaveform[1] );
    fOutWaveform[0].Size((int)fNoData);
    fOutWaveform[1].Size((int)fNoData);

  return 0;
}

int OldInputsToWaveforms::ProcessEntry(){
    // loop over all waveforms in input tree and for each channel fill the
    // waveforms as such
    
    long int n_entries=fInTree->GetEntries();
    WFSOut(2, "OldInputsToWaveforms")<<"Input file has "
        <<n_entries<<" waveforms to convert "<<endl;

    // Loop over each waveform
    for(long int i_event =0; i_event < n_entries;++i_event){
        fInTree->GetEntry(i_event);
        // Loop over each channel
        for (int i_channel=0; i_channel<2; ++i_channel){
            fOutWaveform[i_channel].SetTicks(fInX[i_channel]);
            fOutWaveform[i_channel].SetSamples(fInY[i_channel]);
        } //channels

        // Store the converted waveforms
        fOutTree->Fill();
    } //events

  return 0;
}

int OldInputsToWaveforms::AfterLastEntry(){
    fInFile->Close();
    GetDirectory()->cd();
    fOutTree->Write();
  return 0;
}

REGISTER_MODULE(OldInputsToWaveforms, input_file, tree_name);
