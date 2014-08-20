#include "WaveformSource.h"
#include "ModulesFactory.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"
#include "wfsRunManager.h"

#include "WaveformGenerator.h"
#include "LoadOldWaveforms.h"

using std::cout;
using std::endl;

WaveformSource::WaveformSource(modules::options* opts):
   BaseModule("WaveformSource",opts),fOptions(opts),fGenerator(NULL){
       fMode=opts->GetString("mode","");
}

WaveformSource::~WaveformSource(){
}

int WaveformSource::BeforeFirstEntry(){
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

int WaveformSource::ProcessEntry(bool& go_on){
  return fGenerator->GetWaveform();
  return 0;
}

int WaveformSource::AfterLastEntry(){
  return 0;
}

REGISTER_MODULE(WaveformSource,mode);
