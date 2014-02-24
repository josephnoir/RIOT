#include "cppMix.hpp"

cppMix::cppMix()
{
	printf("Instanciating Object [constructor called]\n");
    greet();
}

cppMix::~cppMix()
{
	printf("Destroying Object [destructor called]\n");
    printf("Im shutting down!\n");
}

void cppMix::sayHello(void)
{
    printf("Hello!\n");
}

void cppMix::greet(void)
{
    printf("Im starting!\n");
}
