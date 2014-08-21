#include "WaveformSource.h"
#include "ModulesFactory.h"
#include "GeneratorsFactory.h"
#include "GeneratorsOptions.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"
#include "wfsRunManager.h"
#include "wfsWaveform.h"

#include "WaveformGenerator.h"

using std::cout;
using std::endl;

WaveformSource::WaveformSource(modules::options* opts):
   BaseModule("WaveformSource",opts),fOptions(opts),fGenerator(NULL){
       fMode=opts->GetString("mode");
}

WaveformSource::~WaveformSource(){
}

int WaveformSource::BeforeFirstEntry(){
    // Set up the generator
    fGenerator=generators::factory::Instance()->createModule(fMode,new generators::options(*fOptions));
    if(!fGenerator) {
        WFSErr(1)<<"Unknown mode: '"<<fMode<<"'"<<endl;
        return 1;
    }
    WFS::RunManager::Instance()->NumberEventsToProcess(fGenerator->GetNumWFS());
  return 0;
}

int WaveformSource::ProcessEntry(bool& go_on){
    try{
        const WFS::WaveForm& waveform=fGenerator->GetWaveform();
        WFS::RunManager::SetGlobalWaveform("input",waveform);
    } catch( WFS::Errors::stop_generating& e){}
    return 0;
}

int WaveformSource::AfterLastEntry(){
  return 0;
}

REGISTER_MODULE(WaveformSource,mode);
