// This file sets up listeners that integrate with the IDE.
#include "testify/testify.hpp"
using namespace Testify;
#ifndef TESTIFY_IDE_SETUP
#define TESTIFY_IDE_SETUP
#include "testify/listenerhook.hpp"
#include "testify/idelistener.hpp"
static ListenerHook __h__( new IDEListener() );
#endif
