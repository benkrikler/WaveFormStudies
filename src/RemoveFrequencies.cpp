#include "RemoveFrequencies.h"
#include "ModulesFactory.h"
#include "ModulesOptions.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"

using std::cout;
using std::endl;

RemoveFrequencies::RemoveFrequencies(modules::options* opts):
   BaseModule("RemoveFrequencies",opts){
  
}

RemoveFrequencies::~RemoveFrequencies(){
}

int RemoveFrequencies::BeforeFirstEntry(){

  // Setup transform
  //
  // Get list of waveforms / access to waveforms

  return 0;
}

int RemoveFrequencies::ProcessEntry(){
    // Get the current waveform
    //
    // Set points on the transform's 
    //
    // Transofrm
    //
    // Multiply by mask
    //
    // InverseTransform
    //
    // Store waveform
  return 0;
}

int RemoveFrequencies::AfterLastEntry(){
  return 0;
}

REGISTER_MODULE(RemoveFrequencies);
