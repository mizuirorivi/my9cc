.intel_syntax noprefix
.globl main
main:
    push 300
    push 400
    pop rdi
    pop rax
    DWORD PTR add rax, rdi
    push rax
    pop rax
    ret
