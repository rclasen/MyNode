#ifndef MyNodeCore_h
#define MyNodeCore_h

#define MYNODE_DEBUG 1
#define MYNODE_ERROR 1
//#define MYNODE_WITH_HEARTBEAT

#include "MyProgmem.h"
#include "MyAssert.h"
#include "MyTime.h"
#include "MyNodeNv.h"

// MyNodeItems may request extra features
// linker only adds this code, if it's used
void MyNodeEnableAdc( void );

// TODO: optional panic reason
void MyNodePanic( void );


#endif
