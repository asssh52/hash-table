section .text

global strlen_memcpy

;RDI, RSI - string ptrs
strlen_memcpy:

    inline_asm_loop:
    lodsb
    test al, al
    jz inline_asm_end
    stosb
    jmp inline_asm_loop
    inline_asm_end:

    ret

