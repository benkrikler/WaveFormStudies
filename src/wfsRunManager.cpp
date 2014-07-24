#include "wfsRunManager.h"
#include "wfsLog.h"
WFS::RunManager* WFS::RunManager::fThis=NULL;

using namespace WFS;

void RunManager::ProcessCommandLine(int argc,char* argv[]){
    WFSOut(1)<<"Processing command line with "<<argc-1<<" arguments"<<std::endl;
}

void RunManager::OpenOutputs(){
    WFSOut(1)<<"Opening outputs"<<std::endl;
}

void RunManager::RunEventLoop(){
    WFSOut(1)<<"Main Event loop"<<std::endl;
}

void RunManager::Abort(){
    WFSOut(1)<<"Aborting"<<std::endl;
}

