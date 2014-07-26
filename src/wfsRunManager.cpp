#include "wfsRunManager.h"
#include "wfsLog.h"
#include "wfsErrors.h"
#include "ModulesNavigator.h"
#include "BaseModule.h"

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
    if(fOutFile){
        WFSOut(2)<<"Opened file, '"<<filename<<"'"<<std::endl;
        modules::navigator::Instance()->SetOutFile(fOutFile);
    }
}

void RunManager::RunEventLoop(){
    WFSOut(3)<<"Main Event loop"<<std::endl;
    const modules::iterator begin=modules::navigator::Instance()->Begin();
    const modules::iterator end=modules::navigator::Instance()->End();

    for( modules::iterator i_module=begin;i_module!=end;++ i_module ){
        WFSOut(2, "RunManager")<<"BeforeFirstEntry on: "<<i_module->second->GetName();
        i_module->second->BeforeFirstEntry();
    }

    bool go_on=true;
    for( modules::iterator i_module=begin;
            go_on;){
        WFSOut(2, "RunManager")<<"ProcessEntry on: "<<i_module->second->GetName();

        // Run the module
        go_on=i_module->second->ProcessGenericEntry();

        // move to next module.  
        ++i_module ;

        // If it's the last module, go back to the first one
        if(i_module==end) i_module=begin;
    }

    for( modules::iterator i_module=begin;i_module!=end;++ i_module ){
        WFSOut(2, "RunManager")<<"AfterLastEntry on: "<<i_module->second->GetName();
        i_module->second->AfterLastEntry();
    }
}

void RunManager::Abort(){
    WFSOut(3)<<"Aborting"<<std::endl;
    if(fOutFile) fOutFile->Close();
    if(fThis)delete fThis;
    fThis=NULL;
}

