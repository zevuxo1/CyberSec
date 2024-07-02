.data

extern FN_OpenSSN:DWORD
extern FN_OpenSyscall:QWORD

extern FN_CreateSecSSN:DWORD
extern FN_CreateSecSyscall:QWORD

extern FN_MapViewSSN:DWORD
extern FN_MapViewSyscall:QWORD

extern FN_ThreadSSN:DWORD
extern FN_ThreadSyscall:QWORD

extern FN_QuerySSN:DWORD
extern FN_QuerySyscall:QWORD


.code

P_Open proc
		mov r10, rcx
		mov eax, FN_OpenSSN
		jmp qword ptr [FN_OpenSyscall]
		ret
P_Open endp


P_CreateSec proc
		mov r10, rcx
		mov eax, FN_CreateSecSSN
		jmp qword ptr [FN_CreateSecSyscall]
		ret
P_CreateSec endp

P_MapView proc
		mov r10, rcx
		mov eax, FN_MapViewSSN
		jmp qword ptr [FN_MapViewSyscall]
		ret
P_MapView endp

P_CreateThread proc
		mov r10, rcx
		mov eax, FN_ThreadSSN
		jmp qword ptr [FN_ThreadSyscall]
		ret
P_CreateThread endp

P_Query proc
		mov r10, rcx
		mov eax, FN_QuerySSN
		jmp qword ptr [FN_QuerySyscall]
		ret
P_Query endp


end