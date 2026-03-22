/*
 * Crystal Palace convention for getting ahold of data linked with this loader.
 */
char _DLL_[0] __attribute__((section("dll")));

WINBASEAPI LPVOID WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);

typedef void (WINAPI * _GetVersions)();

#define GETRESOURCE(x) ( char * ) &x

typedef struct {
    int  length;
    char value[ ];
}RESOURCE;