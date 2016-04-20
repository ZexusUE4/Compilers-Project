#include "compiler.h"

int main()
{
    compiler* comp = new compiler("configuration.in");
    comp->start_compilation();
}


