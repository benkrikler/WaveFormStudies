#ifndef WAVEFORMGENERATOR_H__
#define WAVEFORMGENERATOR_H__

#include <vector>
#include <string>
#include <stdexcept>

#include <ModulesOptions.h>

class WaveformGenerator {

    public:
        WaveformGenerator(const char* name, modules::options* opts){
        };
        virtual ~WaveformGenerator(){};

    public:
        virtual int GetWaveform()=0;
        virtual long int GetNumWFS()=0;

};

#endif // WAVEFORMGENERATOR_H__
