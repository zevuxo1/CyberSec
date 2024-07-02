#include <Windows.h>
#include <stdio.h>
#include "headers.h"
#include "moduelHandle.h"

#ifndef STRUCTS
#include <winternl.h>
#endif // !STRUCTS

#define CONTAINING_RECORD(address, type, field) \ ((type *)((PCHAR)(address) - (ULONG PTR)(&((type * )0)->field))


// Converts Each Input to a lower case strings them compares them
BOOL IsEqual(IN LPCWSTR str1, IN LPCWSTR str2) {
	WCHAR lStr1[MAX_PATH];
	// Buffers For lowercase Strings
	WCHAR lStr2[MAX_PATH];

	int len1 = lstrlenW(str1);
	// Get Length Of Each String
	int len2 = lstrlenW(str2);

	int i = 0;
	// Indexes
	int j = 0;

	// Quick Check For Buffer overFlows
	if (len1 >= MAX_PATH || len2 >= MAX_PATH) {
		return FALSE;
	}

	for (i = 0; i < len1; i++) {
		lStr1[i] = (WCHAR)tolower(str1[i]); // Convert Each Char To lowercase
	}
	lStr1[i++] = L'\0'; // Null Terminate it

	for (j = 0; j < len2; j++) {
		lStr2[j] = (WCHAR)tolower(str2[j]); // And Again
	}
	lStr2[j++] = L'\0';

	if (lstrcmpiW(lStr1, lStr2) == 0) {
		return TRUE; // Compare the strings and return True if both are equal
	}
	return FALSE;
}

// Uses Pointers 
HMODULE CustomGetHandle1(IN DWORD hsh) {
#ifdef _WIN64
	PPEB pPeb = (PEB*)(__readgsqword(0x60)); // If Compiled as x64 
#elif _WIN32
	PPEB pPeb = (PEB*)(__readgsqword)(0x30)) // If Compiling as x32
#endif
	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)(pPeb->Ldr); // Get The LDR

	PLDR_DATA_TABLE_ENTRY pDte = (PLDR_DATA_TABLE_ENTRY)(pLdr->InMemoryOrderModuleList.Flink); // Get The First elemant inside linked list

	while (pDte) {
		if (pDte->FullDllName.Length != NULL) { // If Not Null We Continue

			DWORD hshed = hshW(pDte->FullDllName.Buffer);
			if (hsh == hshed) { // Compare Current DLL with needed DLL
				wprintf(L"[*] Found DLL: %s\n", pDte->FullDllName.Buffer);
#ifdef  STRUCTS
				return (HMODULE)pDte->InInitializationOrderLinks.Flink; // If Structs.h is delacred Use this return
#else
				return (HMODULE)pDte->Reserved2[0]; // else use this return
#endif //  STRUCTS
			}
		}
		else {
			break;
		}
		pDte = *(PLDR_DATA_TABLE_ENTRY*)(pDte); // Move To Next Elemant
	}
	return  NULL;
}

//Uses Head and node to enumerate the dlls 
HMODULE CustomGetHandle2(IN LPCWSTR dwModuleName) {
#ifdef _WIN64
	PPEB pPeb = (PEB*)(__readgsqword(0x60));
#elif
	PPEB pPeb = (PEB*)(__readgsqword(0x30));
#endif
	PLDR_DATA_TABLE_ENTRY pDte = (PLDR_DATA_TABLE_ENTRY)(pPeb->Ldr->InMemoryOrderModuleList.Flink);

	PLIST_ENTRY pListHead = (PLIST_ENTRY)&pPeb->Ldr->InMemoryOrderModuleList;
	PLIST_ENTRY pListNode = (PLIST_ENTRY)pListHead->Flink;

	do {
		if (pDte->FullDllName.Length != NULL) {
			if (IsEqual(pDte->FullDllName.Buffer, dwModuleName)) {
				wprintf("Found Dll: %s\n", pDte->FullDllName.Buffer);
#ifdef  STRUCTS
				return (HMODULE)(pDte->InInitializationOrderLinks.Flink);
#else
				return (HMODULE)pDte->Reserved2[0];
#endif //  STRUCTS

			}

			pDte = (PLDR_DATA_TABLE_ENTRY)(pListNode->Flink);

			pListNode = (PLIST_ENTRY)(pListNode->Flink);
		}
	} while (pListNode != pListHead);

	return NULL;
}



// OUTPUT
// [1] Original: 0x00007FFEB3E60000
// [2] Replacement1: 0x00007FFEB3E60000
// [3] Replacement2: 0x00007FFEB3E60000