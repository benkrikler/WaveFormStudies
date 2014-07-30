#include "LoadOldWaveforms.h"
#include "wfsLog.h"
#include "wfsErrors.h"

#include <TFile.h>
#include <TTree.h>

using std::endl;
namespace{
struct ConnectInputError:public WFS::Errors::BaseError{
    ~ConnectInputError()throw(){}
    ConnectInputError(int ret_val):WFS::Errors::BaseError(ret_val){}
        virtual void What(std::stringstream& os){
            os<<"Problem connecting to input\n";
        };
};
}

LoadOldWaveforms::LoadOldWaveforms(modules::options* opts):WaveformGenerator("load_old",opts){
    fInputFileName=opts->GetString("input_file");
    fTreeName=opts->GetString("tree_name","t1");
}

void LoadOldWaveforms::ConnectInput(std::string filename,std::string treename){
    if(fInputFileName.empty()){
        WFSErr(WFS::Logger::kQuiet)
            <<"No input file given"<<endl;
        throw ConnectInputError(1);
    }

    fInFile=TFile::Open(fInputFileName.c_str(),"read");
    if(!fInFile) {
        WFSErr(1) <<"Unable to open input file: '"<<fInputFileName<<"'"<<endl;
        throw ConnectInputError(2);
    }

    fInTree=(TTree*)fInFile->GetObjectChecked(fTreeName.c_str(),"TTree");
    if(!fInTree) {
        WFSErr(1) <<"Unable to find input tree: '"<<fTreeName<<"'"<<endl;
        throw ConnectInputError(3);
    }

    ///fInTree->SetBranchAddress("xc0", fInX[0]);
    ///fInTree->SetBranchAddress("xc1", fInX[1]);
    ///fInTree->SetBranchAddress("yc0", fInY[0]);
    ///fInTree->SetBranchAddress("yc1", fInY[1]);
    fInTree->GetEntry();
}

long int LoadOldWaveforms::GetNumWFS(){
    return -1;
}
int LoadOldWaveforms::GetWaveform(){
return 0;
}

