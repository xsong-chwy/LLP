global _start

section .data
message: db 'hello, world!', 10

section .text
_start:
  mov rax, 1       ; write system call
  mov rdi, 1       ; arg#1 - file descriptor
  mov rsi, message ; arg#2 - start address
  mov rdx, 14      ; arg#3 - length
  syscall

  mov rax, 60      ; exit system call
  xor rdi, rdi     ; 0
  syscall
