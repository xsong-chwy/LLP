section .data

newline_char: db 10
codes: db '0123456789abcdef'

section .text
global _test, print_hex, print_newline

print_newline:
  mov rax, 1             
  mov rdi, 1             ; syscall arg #1 - file descriptor
  mov rsi, newline_char  ; syscall arg #2 - start address
  mov rdx, 1             ; syscall arg #3 - length
  syscall
  ret

print_hex:
  mov rax, rdi           ; copy rdi, need rdi for system call           
  mov rdi, 1             ; syscall arg #1 - file descriptor
  mov rdx, 1             ; syscall arg #3 - length
  mov rcx, 64            ; shift amount

.loop:
  push rax

  sub rcx, 4
  sar rax, cl
  and rax, 0xf
  lea rsi, [codes + rax]  ; syscall arg #2 - start address
  
  mov rax, 1
  push rcx
  syscall                 ; write syscall
  pop rcx
  
  pop rax

  test rcx, rcx
  jnz .loop
  ret

_test:
  ; prepare arguments
  mov rdi, 0x1122334455667788
  call print_hex

  ; no arguments
  call print_newline

  ; exit
  mov rax, 60
  xor rdi, rdi
  syscall
