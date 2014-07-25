#ifndef WFSWAVEFORM_H
#define WFSWAVEFORM_H

#include "wfsDefinitions.h"
#include <TObject.h>

namespace WFS{
    class WaveForm;
}

class WFS::WaveForm: public TObject{
    public:
        WaveForm():fSamples(),fTicks(){};
        WaveForm(int size):fSamples(size),fTicks(size){};
        WaveForm(const WaveformSamples_t& ticks, const WaveformSamples_t& vals):
            fSamples(vals),fTicks(ticks){};
        virtual ~WaveForm(){};

        size_t Size()const{return fSamples.GetNrows();}
        void Size( int s){ 
            fSamples.ResizeTo(s);
            fTicks.ResizeTo(s);
        }

        void SetSample(int i, RealVal_t val){ fSamples[i]=val; }
        void SetTick(int i, RealVal_t val){ fTicks[i]=val; }

        void SetSamples(const RealVal_t* val){ fSamples.SetElements(val); }
        void SetTicks(const RealVal_t* val){ fTicks.SetElements(val); }

        WaveForm& operator=(const WaveForm& rhs){
            fSamples=rhs.fSamples;
            fTicks=rhs.fTicks;
            return *this;
        }

        // add the samples of another waveform over the samples of this one
        // Keep the ticks of this waveform (ie. ignore rhs )
        void Superpose(const WaveForm& rhs){
            if(fComponents.empty()) fComponents.push_back(fSamples);
            fComponents.push_back(rhs.fSamples);
            fSamples+rhs.fSamples;
        }

        void Draw(const char* opts="") {
        //  | Eventually should call draw for each waveform in the components list then set the x-ticks
        //  | or axis using the fTicks
         fSamples.Draw(opts);
        }

    private:
        WaveformSamples_t fSamples;
        WaveformSamples_t fTicks;
        std::vector<WaveformSamples_t> fComponents;

        ClassDef(WaveForm,1);
};

#endif //WFSWAVEFORM_H
