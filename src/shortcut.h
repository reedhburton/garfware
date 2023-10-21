#ifndef _SHORTCUT_FILE
#define _SHORTCUT_FILE


#define INITGUID
#include <windows.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <initguid.h>
#include <strsafe.h>

HRESULT ResolveIt(HWND hwnd, LPCSTR lpszLinkFile, LPCSTR iconPath);

#endif
