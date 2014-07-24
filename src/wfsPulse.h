#ifndef WFPULSE_H
#define WFPULSE_H

#include "wfsDefinitions.h"
#include <TObject.h>

namespace WFS{
    class WaveForm;
}

class WFS::WaveForm: public TObject{
    public:
        WaveForm(){};
        WaveForm(int size):fSamples(size,0){};
        virtual ~WaveForm(){};

        size_t Size()const{return fSamples.size();}

        void SetSample(int i, RealVal_t val){ 
            fSamples.reserve(i);
            fSamples[i]=val;
        }

        WaveformSamples_t::iterator Begin(){return fSamples.begin();}
        WaveformSamples_t::const_iterator Begin()const{return fSamples.begin();}

        WaveformSamples_t::iterator End(){return fSamples.end();}
        WaveformSamples_t::const_iterator End()const{return fSamples.end();}

        template <typename Functor_t>
        size_t ForEach(Functor_t& funct) {
            bool go_on=true;
            for(WaveformSamples_t::iterator i_sample=Begin(); 
                    i_sample!= End() && go_on; ++i_sample){
                go_on=funct(i_sample-Begin(),*i_sample);
            }
        }

    private:
        WaveformSamples_t fSamples;

        ClassDef(WaveForm,1);
};

#endif //WFPULSE_H
