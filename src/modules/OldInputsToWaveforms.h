#ifndef OLDINPUTSTOWAVEFORMS_H_
#define OLDINPUTSTOWAVEFORMS_H_

#include "BaseModule.h"
#include <string>
#include "wfsWaveform.h"

class TFile;
class TTree;
namespace modules {class options;}

class OldInputsToWaveforms : public BaseModule {

 public:
  OldInputsToWaveforms(modules::options* opts);
  ~OldInputsToWaveforms();

 private:
  virtual int ProcessEntry();
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();

  std::string fInputFileName;
  std::string fTreeName;

  TFile *fInFile;
  TTree *fInTree, *fOutTree;

  WFS::WaveForm fOutWaveform[2];

  enum { fNoData=1024};
  Float_t fInX[2][fNoData];
  Float_t fInY[2][fNoData];
};

#endif //OLDINPUTSTOWAVEFORMS_H_
