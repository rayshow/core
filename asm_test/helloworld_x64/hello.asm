TITLE Add and Subtract (AddSub.asm)


INCLUDELIB kernel32.lib


ExitProcess PROTO

.data
sum QWORD 0
.code
main64 PROC
	mov rax, 1
	mov rbx, 20
	add rbx, rax
	mov sum, rbx
	nop
	call ExitProcess
main64 ENDP
END
