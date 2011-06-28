#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "helpers/types.h"

void loadExtensions(const char* extConfFileName);
extern "C" void maInvokeExtension(uint function);

#endif	//EXTENSIONS_H
