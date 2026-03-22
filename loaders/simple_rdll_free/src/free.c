#include <windows.h>
#include "free.h"

void go(char * loader, char * dllEntry, char * dllBase) {
	/* Free loader memory */
	KERNEL32$VirtualFree(loader, 0, MEM_RELEASE);

	/* Call entrypoint of Reflective DLL */
	((DLLMAIN_FUNC)dllEntry)((HINSTANCE)dllBase, DLL_PROCESS_ATTACH, NULL);

	/* Exit the current thread, otherwise it would come back to freed memory and crash */
	KERNEL32$ExitThread(0);
}