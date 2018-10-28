#ifndef __DATATYPE_H__
#define __DATATYPE_H__

#include <stdint.h>

#ifndef _HAVE_TYPE_BOOL
#define _HAVE_TYPE_BOOL
	typedef int BOOL;
#endif

#ifndef TRUE
	#define TRUE	1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifndef IO
    #define IO
#endif

#ifdef __GNUC__ 
#define HOWCALL
#else
#define HOWCALL __stdcall
#endif

#endif //__DATATYPE_H__
