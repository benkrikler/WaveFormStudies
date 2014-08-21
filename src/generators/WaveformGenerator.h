#ifndef WAVEFORMGENERATOR_H__
#define WAVEFORMGENERATOR_H__

#include <vector>
#include <string>

#include <GeneratorsOptions.h>
#include "wfsWaveform.h"
#include "wfsErrors.h"

namespace WFS { 
    namespace Errors{
        struct stop_generating:public WFS::Errors::BaseError{
            stop_generating(int i): BaseError(i){
                    SetWhat()<<"Stopped after event "<<i;
            }
            ~stop_generating()throw(){}
        };
    }
}

class WaveformGenerator {

    public:
        WaveformGenerator(const char* name, generators::options* opts){
        };
        virtual ~WaveformGenerator(){};

    public:
        virtual const WFS::WaveForm& GetWaveform()=0;
        virtual long int GetNumWFS()=0;

};

#endif // WAVEFORMGENERATOR_H__
