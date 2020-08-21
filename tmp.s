.intel_syntax noprefix
.globl main
main:
    push 80
    push 20
    pop rdi
    pop rax
    add rax, rdi
    push rax
    push 10
    pop rdi
    pop rax
    sub rax, rdi
    push rax
    push 2
    pop rdi
    pop rax
    imul rax, rdi
    push rax
    push 100
    pop rdi
    pop rax
    sub rax, rdi
    push rax
    push 20
    push 2
    pop rdi
    pop rax
    cqo
    idiv rdi
    push rax
    pop rdi
    pop rax
    add rax, rdi
    push rax
    push 2
    pop rdi
    pop rax
    add rax, rdi
    push rax
    push 90
    pop rdi
    pop rax
    sub rax, rdi
    push rax
    pop rax
    ret
