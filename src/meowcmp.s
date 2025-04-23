section .text

global meowcmp

;RDI, RSI - string ptrs
meowcmp:

    vmovups ymm0, yword [rdi]
    vmovups ymm1, yword [rsi]

    vpcmpeqb ymm0, ymm0, ymm1
    vpmovmskb eax, ymm0

    ret

