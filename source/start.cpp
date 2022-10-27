#include "engine.hpp"
#include "platform/platform.hpp"


void start(Platform& pf)
{
    herocore::Engine herocore(pf);
    herocore.run();
}
