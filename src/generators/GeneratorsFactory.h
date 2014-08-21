#ifndef GENERATORSFACTORY__HH_
#define GENERATORSFACTORY__HH_
#include "TemplateFactory.h"

class WaveformGenerator;

namespace generators{
    class factory;
    class options;
}

class generators::factory:public TemplateFactory<WaveformGenerator,generators::options>{
	private:
		factory():TemplateFactory<WaveformGenerator,generators::options>(){};
		~factory(){};

    public:
		// Get the single instance of this class
		static factory* Instance();

     private:
        static factory* fThis;
};

inline generators::factory* generators::factory::Instance(){
    if(!fThis) {
        fThis=new factory();
    }
    return fThis;
}

inline static generators::factory* generators::factory::fThis=NULL;

#define REGISTER_GENERATOR( CLASS , ... ) \
RegistryProxy<CLASS##Generator,WaveformGenerator,generators::options,generators::factory> p_##CLASS(#CLASS, #__VA_ARGS__);

#endif // GENERATORSFACTORY__HH_
