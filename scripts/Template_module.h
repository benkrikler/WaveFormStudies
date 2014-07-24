#ifndef %MODULE%_H_
#define %MODULE%_H_

#include "BaseModule.h"
namespace modules {class options;}

class %module% : public BaseModule {

 public:
  %module%(modules::options* opts);
  ~%module%();

 private:
  virtual int ProcessEntry();
  virtual int BeforeFirstEntry();
  virtual int AfterLastEntry();
};

#endif //%MODULE%_H_
