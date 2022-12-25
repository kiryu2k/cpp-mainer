.global _start

sum:
        add     %rdi, %rsi
        mov     %rsi, %rax
        ret

_start:
        mov     $40, %rdi
        mov     $2, %rsi
        call    sum

        mov     %rax, %rdi

        mov     $1, %rax    # 1 - номер системного вызова 'exit'
        mov     %rdi, %rbx  # аргумент вызова 'exit'
        int     $0x80
