#include "wfsRunManager.h"
#include "wfsLog.h"
#include "wfsErrors.h"
#include "ModulesNavigator.h"
#include "ModulesParser.h"
#include "BaseModule.h"

#include <unistd.h>
#include <TFile.h>

using std::endl;

WFS::RunManager* WFS::RunManager::fThis=NULL;


namespace{
    bool PowerOfTwo(long int x){
        return ((x != 0) && !(x & (x - 1)));
    }
}

using namespace WFS;

class Usage:public WFS::Errors::BaseError{
    public:
    Usage(const std::string& name,const std::string& error=""):WFS::Errors::BaseError(0){
        if(!error.empty()) SetWhat() <<"Error: "<<error<<"\n\n";
        SetWhat()<<"Usage: "<<name<<" [options]\n";
    }
    ~Usage()throw(){}
};

class EventLoopError:public WFS::Errors::BaseError{
    public:
    EventLoopError(int ret_val):WFS::Errors::BaseError(ret_val,"Problem in main event loop\n"){}
    ~EventLoopError()throw(){}
};

RunManager::RunManager():fCommandLine("RunManager"),fOutFile(NULL),fProcessNumEvents(0){
}

void RunManager::ProcessCommandLine(int argc,char* argv[]){

    WFSOut(3)<<"Processing command line with "<<argc-1<<" arguments"<<std::endl;

    char c;
    while( (c=getopt(argc,argv,"h?n:o:vm:q"))!=-1 ){
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
            case 'n': 
                fProcessNumEvents=modules::parser::GetNumber(optarg);
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
        WFSOut(2,"RunManager")<<"Opened file, '"<<filename<<"'"<<std::endl;
        modules::navigator::Instance()->SetOutFile(fOutFile);
    }
}

void RunManager::RunEventLoop(){
    WFSOut(3)<<"Main Event loop"<<std::endl;

    // Setup the module iterators
    const modules::iterator begin=modules::navigator::Instance()->Begin();
    const modules::iterator end=modules::navigator::Instance()->End();
    modules::iterator i_module;

    // Run all BeforeFirstEntry methods
    int ret_val=0;
    for( i_module=begin;i_module!=end;++ i_module ){
        WFSOut(2, "RunManager")<<"BeforeFirstEntry on: "<<i_module->second->GetName()<<endl;
        ret_val=i_module->second->BeforeFirstEntry();
        if(ret_val!=0){
            WFSErr(1)<<"Error running BeforeFirstEntry for module '"<<i_module->first<<"'"<<endl;
            throw EventLoopError(ret_val);
        }
    }

    // Run all ProcessEntry methods
    bool go_on=true;
    for(long int i_event=0;
            i_event<NumberEventsToProcess() && go_on;
            ++i_event){
        // If we're a power of 2 print out the current event number
        if(PowerOfTwo(i_event)) WFSOut(1,"RunManager")<<"Processing event number: "<<i_event<<endl;

        // Loop over each module
        for( i_module=begin;i_module!=end && go_on; ++i_module ){
            WFSOut(3, "RunManager")<<"ProcessEntry on: "<<i_module->second->GetName()<<endl;

            // Run the module
            ret_val=i_module->second->ProcessGenericEntry(go_on);
            if(ret_val!=0){
                WFSErr(1)<<"Error running ProcessEntry for module '"<<i_module->first<<"'"<<endl;
                throw EventLoopError(ret_val);
            }
        }
    }

    // Run all AfterLastEntry methods
    for( i_module=begin;i_module!=end; ++i_module ){
        WFSOut(2, "RunManager")<<"AfterLastEntry on: "<<i_module->second->GetName()<<endl;
        ret_val=i_module->second->AfterLastEntry();
        if(ret_val!=0){
            WFSErr(1)<<"Error running AfterLastEntry for module '"<<i_module->first<<"'"<<endl;
            throw EventLoopError(ret_val);
        }
    }
}

void RunManager::Abort(){
    WFSOut(3)<<"Aborting"<<std::endl;
    if(fOutFile) fOutFile->Close();
    if(fThis)delete fThis;
    fThis=NULL;
}

