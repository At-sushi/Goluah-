; VC2008以降で作ったソフトを2000 SP4で動くようにするための措置。動作は保証しません。
.model flat
INCLUDELIB LIBCMT.LIB
PROTO@4 TYPEDEF PROTO STDCALL :DWORD
PROTO@8 TYPEDEF PROTO STDCALL :DWORD, :DWORD
EXTERNDEF __imp__EncodePointer@4:DWORD
EXTERNDEF __imp__DecodePointer@4:DWORD
EXTERNDEF STDCALL _imp__GetModuleHandleW@4:PTR PROTO@4
EXTERNDEF STDCALL _imp__GetProcAddress@8:PTR PROTO@8
.const
__imp__EncodePointer@4 dd EncodePointerProcedure
__imp__DecodePointer@4 dd EncodePointerProcedure
KERNEL32 db 'K', 0, 'E', 0, 'R', 0, 'N', 0, 'E', 0, 'L', 0, '3', 0, '2', 0, 0, 0
EncodePointer db "EncodePointer", 0
.code
EncodePointerProcedure PROC STDCALL Pointer:PTR
	invoke _imp__GetModuleHandleW@4, ADDR KERNEL32
	invoke _imp__GetProcAddress@8, eax, ADDR EncodePointer
	.if eax
		invoke (PROTO@4) PTR eax, Pointer
		ret
	.endif
	mov eax, Pointer
	ret
EncodePointerProcedure ENDP
END
