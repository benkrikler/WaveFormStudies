#include "GetWaveforms.h"
#include "ModulesFactory.h"
#include "ModulesOptions.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"
#include "wfsRunManager.h"

#include "WaveformGenerator.h"
#include "LoadOldWaveforms.h"

using std::cout;
using std::endl;

GetWaveforms::GetWaveforms(modules::options* opts):
   BaseModule("GetWaveforms",opts),fOptions(opts),fGenerator(NULL){
       fMode=opts->GetString("mode","");
}

GetWaveforms::~GetWaveforms(){
}

int GetWaveforms::BeforeFirstEntry(){
    if(fMode=="load_old"){
        WFSOut(2)<<"Obtaining waveforms from old files"<<endl;
        fGenerator=new LoadOldWaveforms(fOptions);
    }else {
        WFSErr(1)<<"Unknown mode: '"<<fMode<<"'"<<endl;
        return 1;
    }

    WFS::RunManager::Instance()->NumberEventsToProcess(fGenerator->GetNumWFS());

  return 0;
}

int GetWaveforms::ProcessEntry(bool& go_on){
  return fGenerator->GetWaveform();
  return 0;
}

int GetWaveforms::AfterLastEntry(){
  return 0;
}

REGISTER_MODULE(GetWaveforms,mode);
