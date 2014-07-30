#include "%module%.h"
#include "ModulesFactory.h"
#include "ModulesOptions.h"
#include "wfsDefinitions.h"
#include "wfsLog.h"

using std::cout;
using std::endl;

%module%::%module%(modules::options* opts):
   BaseModule("%module%",opts){
  
}

%module%::~%module%(){
}

int %module%::BeforeFirstEntry(){

  return 0;
}

// Called once for each event in the main event loop
// Return non-zero to indicate a problem and terminate the event loop
int %module%::ProcessEntry(bool &go_on){
  return 0;
}

// Called just after the main event loop
// Can be used to write things out, dump a summary etc
// Return non-zero to indicate a problem
int %module%::AfterLastEntry(){

  // Print extra info if we're debugging this module:
  if(Debug()){
     cout<<"-----%module%::AfterLastEntry(): I'm debugging!"<<endl;
  }

  return 0;
}

REGISTER_MODULE(%module%);
