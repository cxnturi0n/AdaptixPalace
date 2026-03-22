DECLSPEC_IMPORT BOOL WINAPI KERNEL32$VirtualProtect (LPVOID, SIZE_T, DWORD, PDWORD);

void fix_section_permissions ( DLLDATA * dll, char * src, char * dst );
