#ifndef GENERATORS_GENERATORSOPTIONS_H
#define GENERATORS_GENERATORSOPTIONS_H

#include "ModulesOptions.h"

namespace generators{
    class options;
}

class generators::options:public modules::options{
    public:
        options(const std::string& name):modules::options(name){}
        options(const modules::options& opts):modules::options(opts){}
};

#endif // GENERATORS_GENERATORSOPTIONS_H
