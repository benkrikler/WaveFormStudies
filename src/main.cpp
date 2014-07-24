#include <iostream>
#include "ModulesNavigator.h"
#include "wfsRunManager.h"
#include "wfsDefinitions.h"
#include "wfsErrors.h"

using namespace WFS;

void Abort();

int main(int argc, char*argv[]) {


    try{ // Everything in a try .. catch block to handle error catching

    // Process command line options
    RunManager* runMgr=RunManager::Instance();
    runMgr->ProcessCommandLine(argc,argv);

    // Load the modules file
    int ok=modules::navigator::Instance()->LoadModulesFile(runMgr->GetModulesFileName());
    if(ok!=0) { runMgr->Abort(); return ok; }

    // Create any output files
    runMgr->OpenOutputs();

    // Create the modules to be run
    ok= modules::navigator::Instance()->MakeModules();
    if(ok!=0) { runMgr->Abort(); return ok; }

    // Enter the main event loop
    runMgr->RunEventLoop();

    }catch ( Errors::BaseError& e){
        e.what();
        if(RunManager::Instance())
            RunManager::Instance()->Abort();
        return e.Exit(1);
    }

    return 0;
}
