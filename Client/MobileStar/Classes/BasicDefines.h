
#ifndef __BASIC_DEFINES_H__
#define __BASIC_DEFINES_H__

#define PLATFORM_LINUX 1
#define PLATFORM_WINDOWS 2



/////////////////
#define OS_PLATFORM PLATFORM_LINUX

#if OS_PLATFORM == PLATFORM_WINDOWS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#endif

#endif //__BASIC_DEFINES_H__
