#ifndef REMOVEFREQUENCIES_H_
#define REMOVEFREQUENCIES_H_

#include "BaseModule.h"
namespace modules {class options;}

class RemoveFrequencies : public BaseModule {

 public:
  RemoveFrequencies(modules::options* opts);
  ~RemoveFrequencies();

 private:
  virtual int ProcessEntry(bool & go_on);
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();
};

#endif //REMOVEFREQUENCIES_H_
