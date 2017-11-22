section .data
digits: db '0123456789' 

section .text

exit:
  mov rax, 60
  syscall
  ret


string_length:
  mov rax, -1
.loop1:
  inc rax
  mov cl, [rdi+rax]
  test cl, cl
  jnz .loop1
  ret


print_string:
  mov rax, -1
.loop2:
  inc rax
  lea rsi, [rdi+rax]
  mov cl, [rdi+rax]
  test cl, cl
  jz .exit1

  push rax
  push rcx
  push rdi
  
  mov rax, 1             
  mov rdi, 1
  mov rdx, 1
  syscall

  pop rdi
  pop rcx
  pop rax
  jmp .loop2
.exit1: 
  ret


print_char:
  push rdi
  mov rsi, rsp
  mov rax, 1
  mov rdi, 1
  mov rdx, 1
  syscall
  pop rdi
  ret


print_newline:
  mov rcx, 0xA
  push rcx
  mov rsi, rsp
  mov rax, 1
  mov rdi, 1
  mov rdx, 1
  pop rcx
  syscall
  ret


print_uint:
  mov rax, rdi
  mov rdx, 0
  mov rdi, 0
  mov rcx, 10

.loop3:  
  div rcx
  dec rsp
  mov [rsp], dl
  mov rdx, 0
  inc rdi
  test rax, rax
  jz .print_result
  jmp .loop3

.print_result:
  test rdi, rdi
  jz .exit2
  mov rdx, [rsp]
  push rdi
  mov rdi, rdx
  and rdi, 0xff
  add rdi, 48
  call print_char
  pop rdi
  inc rsp
  dec rdi
  jmp .print_result

.exit2:
  ret


print_int:
  cmp rdi, 0 
  jge .print_unsigned
  push rdi
  mov rdi, 45
  call print_char
  pop rdi
  neg rdi

.print_unsigned:
  call print_uint
  ret


read_char:
  dec rsp
  mov rax, 0
  mov rdi, 0
  mov rsi, rsp
  mov rdx, 1
  syscall
  test rax, rax
  jz .exit3
  mov al, [rsp]
  and rax, 0xff
.exit3:
  inc rsp
  ret
  

read_word:
  cmp rsi, 0
  jg .size_ok
  mov rax, 0
  ret

.size_ok:
  push rdi
  push rsi
  
  mov rax, 0
  mov rdx, rsi
  mov rsi, rdi
  mov rdi, 0
  syscall

  mov rdx, rsi
  pop rsi
  pop rdi

.loop4:
  mov rcx, rdi
  add rcx, rsi
  cmp rdx, rcx
  jge .too_big
  
  test rax, rax
  jz .exit4
  
  xor rcx, rcx
  mov cl, [rdx]  
  cmp cl, 0x20
  je .exit4
  cmp cl, 0x09
  je .exit4
  cmp cl, 0x10
  je .exit4

  inc rdx
  dec rax
  jmp .loop4

.exit4:
  mov byte[rdx], 0
  sub rdx, rdi
  mov rax, rdi
  ret

.too_big:
  dec rdx
  mov byte[rdx], 0
  sub rdx, rdi
  mov rax, 0
  ret


; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
  xor rax, rax
  xor rsi, rsi
  xor rcx, rcx
  push rbx
  mov rbx, 10

.loop5:
  mov cl, [rdi]
  test cl, cl
  jz .end
  cmp cl, 48
  jl .end
  cmp cl, 57
  jg .end
  sub cl, 48
  mul rbx
  add rax, rcx
  inc rsi
  inc rdi
  jmp .loop5

.end:
  mov rdx, rsi
  pop rbx
  ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
  mov cl, [rdi]
  cmp cl, 45
  jne .positive
  push word 1
  inc rdi
  jmp .call_parse_uint

.positive:
  push word 0

.call_parse_uint:
  call parse_uint
  pop cx
  test cx, cx
  jnz .negate
  ret
.negate:
  neg rax
  inc rdx
  ret


string_equals:
.loop6:
  mov ch, [rdi]
  mov cl, [rsi]
  cmp ch, cl
  jne .not_equal
  cmp cl, 0
  je .equal
  inc rdi
  inc rsi
  jmp .loop6
.equal:
  mov rax, 1
  ret
.not_equal:
  mov rax, 0
  ret


string_copy:
.loop7:
  mov cl, [rdi]
  test cl, cl
  jz .done
  mov [rsi], cl
  inc rdi
  inc rsi
  jmp .loop7
.done:
  mov [rsi], byte 0
  ret
