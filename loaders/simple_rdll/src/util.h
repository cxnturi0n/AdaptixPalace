#ifndef _UTIL_H_
#define _UTIL_H_

WINBASEAPI LPVOID WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
DECLSPEC_IMPORT BOOL WINAPI KERNEL32$VirtualProtect (LPVOID, SIZE_T, DWORD, PDWORD);

#define GETRESOURCE(x) ( char * ) &x

typedef struct {
    int  length;
    char value[ ];
}RESOURCE;

#endif
