#ifndef WAVEFORMSOURCE_H_
#define WAVEFORMSOURCE_H_

#include "BaseModule.h"
class WaveformGenerator;

class WaveformSource : public BaseModule {

 public:
  WaveformSource(modules::options* opts);
  ~WaveformSource();

 private:
  virtual int ProcessEntry(bool & go_on);
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();

  modules::options* fOptions;
  WaveformGenerator* fGenerator;
  std::string fMode;
};

#endif //WAVEFORMSOURCE_H_
