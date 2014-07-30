#ifndef REMOVEFREQUENCIES_H_
#define REMOVEFREQUENCIES_H_

#include "BaseModule.h"
namespace modules {class options;}

class RemoveFrequencies : public BaseModule {

 public:
  RemoveFrequencies(modules::options* opts);
  ~RemoveFrequencies();

 private:
  virtual int ProcessEntry();
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();
};

#endif //REMOVEFREQUENCIES_H_
