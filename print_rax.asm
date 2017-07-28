section .data
codes: db '0123456789ABCDEF'
num:   db 0x99,0x00,0x00,0x99

section .text
global _start
_start:
  ;mov rax, 0x1122334455667788 ; test data
  mov rax, 0xffff
  mov rax, [num]
  mov rdi, 1  ; file descriptor
  mov rdx, 1  ; length - arg#1
  mov rcx, 64 ; # of bits to shift

.loop:
  push rax     ; store original rax
  sub rcx, 4   ; reduce rcx by 4 each time
  sar rax, cl  ; right shift cl number of times
  and rax, 0xf ; take the lower 4 bits

  lea rsi, [codes + rax] ; index into digits table - agr#2
  mov rax, 1              ; write system call

  push rcx
  syscall
  pop rcx
  pop rax

  test rcx, rcx
  jnz .loop

  mov rax, 60
  xor rdi, rdi
  syscall
