#include "wfsLog.h"
#include <iostream>
#include <fstream>

WFS::Logger* WFS::Logger::fThis=NULL;

WFS::Logger::Logger(const Logger& rhs):fOut(NULL),fErr(NULL),fNull(NULL){
}

WFS::Logger::Logger():fOut(NULL),fErr(NULL),fNull(NULL){
    fNull= new std::ofstream("/dev/null", std::ios::out|std::ios::app);
}

WFS::Logger::~Logger(){
    std::ofstream* strs[3]={fOut,fNull,fErr};
    for(int i=0;i<3;i++){
        if(strs[i]){
            strs[i]->close();
            delete strs[i];
        }
    }
}

void WFS::Logger::IncremementVerbosity(Level_t& l, int amount){
    if(amount>=0){
        switch (l){
            case kQuiet:    l=kTimid    ; break ; 
            case kTimid:    l=kOutgoing ; break ; 
            case kOutgoing: l=kShowoff  ; break ; 
            case kShowoff:              ; break ; 
        }
    } else {
        switch (l){
            case kQuiet:                ; break ; 
            case kTimid:    l=kQuiet    ; break ; 
            case kOutgoing: l=kTimid    ; break ; 
            case kShowoff:  l=kOutgoing ; break ; 
        }
    }
}
