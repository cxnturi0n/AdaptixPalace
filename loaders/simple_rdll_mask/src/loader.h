#ifndef _LOADER_H_
#define _LOADER_H_

WINBASEAPI LPVOID WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI BOOL WINAPI KERNEL32$VirtualFree (LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);

typedef void (WINAPI * _GetVersions)();

#define GETRESOURCE(x) ( char * ) &x

typedef struct {
    int  length;
    char value[ ];
}RESOURCE;

#endif