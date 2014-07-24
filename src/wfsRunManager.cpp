#include "wfsRunManager.h"
#include "wfsLog.h"
#include "wfsErrors.h"

#include <unistd.h>
#include <TFile.h>

using std::endl;

WFS::RunManager* WFS::RunManager::fThis=NULL;

using namespace WFS;

struct Usage:public WFS::Errors::BaseError{
    ~Usage()throw(){}
    Usage(const std::string& name,const std::string& error=""):WFS::Errors::BaseError(0),fName(name){}
        virtual void What(std::stringstream& os){
            if(!fError.empty()) os <<"Error: "<<fError<<"\n\n";
            os<<"Usage: "<<fName<<" [options]\n";
        };
        std::string fName,fError;
};

RunManager::RunManager():fCommandLine("RunManager"),fOutFile(NULL){
}

void RunManager::ProcessCommandLine(int argc,char* argv[]){

    WFSOut(3)<<"Processing command line with "<<argc-1<<" arguments"<<std::endl;

    char c;
    while( (c=getopt(argc,argv,"h?o:vm:q"))!=-1 ){
        switch (c){
            case 'h': case '?':
                throw Usage(argv[0]);
                break;
            case 'q': 
                WFS::Logger::Instance()->LessOutput();
                WFS::Logger::Instance()->LessError();
                break;
            case 'v': 
                WFS::Logger::Instance()->MoreOutput();
                WFS::Logger::Instance()->MoreError();
                break;
            case 'o': 
                fCommandLine.SetOption("output_file",optarg);
                break;
            case 'm': 
                fCommandLine.SetOption("module_file",optarg);
                break;
            default:
                std::string error="Unknown option ";
                error+=c;
                throw Usage(argv[0],error);
        }
    }
}

void RunManager::OpenOutputs(){
    WFSOut(3)<<"Opening outputs"<<std::endl;
    if(fOutFile){
        fOutFile->Close();
        delete fOutFile;
    }
    std::string filename=fCommandLine.GetString("output_file","output.root");
    fOutFile=new TFile(filename.c_str(),"recreate");
    if(fOutFile) std::cout<<"Opened file"<<std::endl;
}

void RunManager::RunEventLoop(){
    WFSOut(3)<<"Main Event loop"<<std::endl;
}

void RunManager::Abort(){
    WFSOut(3)<<"Aborting"<<std::endl;
    if(fOutFile) fOutFile->Close();
    if(fThis)delete fThis;
    fThis=NULL;
}

