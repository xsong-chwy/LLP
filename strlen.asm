global _start

section .data
test_str: db 'hello, assembly',10,0

section .text
strlen:
  xor rax, rax

.loop:
  cmp byte[rdi+rax], 0
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
