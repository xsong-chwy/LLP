global _start

section .data

test_str: db 'abcdefg', 0


section .text

strlen:
	xor rax, rax  ; return value is stored in rax

.loop:
	cmp byte[rdi + rax], 0 ; rdi stores start address

	je .end
	inc rax
	jmp .loop

.end:
	ret

_start:
	mov rdi, test_str
	call strlen

	mov rdi, rax
	mov rax, 60
	syscall
