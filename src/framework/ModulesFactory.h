#ifndef MODULESFACTORY__HH_
#define MODULESFACTORY__HH_
#include "TemplateFactory.h"

class BaseModule;

namespace modules{
   typedef BaseModule BaseModule;
    class factory;
    class options;
}

class modules::factory:public TemplateFactory<modules::BaseModule,modules::options>{
	private:
		factory():TemplateFactory<modules::BaseModule,modules::options>(){};
		~factory(){};
	 public:
		// Get the single instance of this class
		static factory* Instance();

};

inline modules::factory* modules::factory::Instance(){
    static modules::factory* instance=NULL;
    if(!instance) {
	instance=new factory();
    }
    return instance;
}

#define REGISTER_MODULE( CLASS , ... ) \
RegistryProxy<CLASS,modules::BaseModule,modules::options,modules::factory> p_##CLASS(#CLASS, #__VA_ARGS__);

#endif // MODULESFACTORY__HH_
