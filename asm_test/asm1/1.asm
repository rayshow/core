TITLE Add and Subtract (AddSub.asm)


INCLUDELIB kernel32.lib

.MODEL flat,stdcall  

ExitProcess PROTO, dwFlag:dword

.data
sum DWORD 0
.code
main PROC
	mov eax, 1
	mov ebx, 20
	add ebx, eax
	mov sum, ebx
	nop
	invoke ExitProcess,0
main ENDP



END main
