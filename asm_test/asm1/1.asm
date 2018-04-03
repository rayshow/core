TITLE Add and Subtract (AddSub.asm)


INCLUDELIB kernel32.lib

.MODEL flat,stdcall  

ExitProcess PROTO,
	dwExitCode:DWORD

.data
.code
main PROC
	mov eax, 10000h
	mov ebx, 40000h
	push 0h
	call ExitProcess
main ENDP
END main
