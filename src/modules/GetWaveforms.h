#ifndef GETWAVEFORMS_H_
#define GETWAVEFORMS_H_

#include "BaseModule.h"
class WaveformGenerator;
namespace modules {class options;}

class GetWaveforms : public BaseModule {

 public:
  GetWaveforms(modules::options* opts);
  ~GetWaveforms();

 private:
  virtual int ProcessEntry(bool & go_on);
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();

  modules::options* fOptions;
  WaveformGenerator* fGenerator;
  std::string fMode;
};

#endif //GETWAVEFORMS_H_
