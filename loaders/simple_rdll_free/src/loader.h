char _DLL_[0] __attribute__((section("dll")));
char _PICO_[0] __attribute__((section("pico")));


WINBASEAPI LPVOID WINAPI KERNEL32$VirtualAlloc (LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);

typedef void (WINAPI * _GetVersions)();

typedef void (*PICOMAIN_FUNC_3)(char * loader, char * dllEntry, char * dllBase);

#define GETRESOURCE(x) ( char * ) &x

typedef struct {
    int  length;
    char value[ ];
}RESOURCE;

void go();

char * getLoaderStartAddress();

void JumpToFreePICO(IMPORTFUNCS * funcs, char * dllEntry, char *dllBase);