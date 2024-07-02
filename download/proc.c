#include "headers.h"


FARPROC CProc(IN HMODULE ndll, IN DWORD hshed) {
	// Save The Base Address Of Module
	PBYTE pBase = (PBYTE)ndll;

	// Find The Dos Header And Do a Check
	PIMAGE_DOS_HEADER pIDos = (PIMAGE_DOS_HEADER)pBase;
	if (pIDos->e_magic != IMAGE_DOS_SIGNATURE) {
		//printf("Not a Valid Dos header");
		return NULL;
	}

	// Find The NT Header And Do a Check, e_lfanew points to it
	PIMAGE_NT_HEADERS pINt = (PIMAGE_NT_HEADERS)(pBase + pIDos->e_lfanew);
	if (pINt->Signature != IMAGE_NT_SIGNATURE) {
		//printf("Not a Valid NT Header");
		return NULL;
	}

	// Get The Option Header Address
	IMAGE_OPTIONAL_HEADER pIoP = pINt->OptionalHeader;


	PIMAGE_EXPORT_DIRECTORY pIExDr = (PIMAGE_EXPORT_DIRECTORY)(pBase + pIoP.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	// Save The Addresses Of Names, Functions And the Oridnals
	PDWORD FuncNameArray = (PDWORD)(pBase + pIExDr->AddressOfNames);
	PDWORD FuncAddrArray = (PDWORD)(pBase + pIExDr->AddressOfFunctions);
	PWORD FuncOrdinalArray = (PWORD)(pBase + pIExDr->AddressOfNameOrdinals);

	// Loop Through Each Function And Check if it == The Needed Function And If So
	for (DWORD i = 0; i < pIExDr->NumberOfFunctions; i++) {
		CHAR* pFuncName = (CHAR*)(pBase + FuncNameArray[i]); // Name Of Current Function
		DWORD hashed = hshA(pFuncName); // Hash The Current Function Name For comparing Against One we Want
		//printf("%s: 0x%0.8x\n", pFuncName, hashed);

		if (hshed == hashed) {
			// Get and Return The Address Of the Current Function
			PVOID pFuncAddr = (PVOID)(pBase + FuncAddrArray[FuncOrdinalArray[i]]);
			return pFuncAddr;
		}
	}
	return NULL;

}