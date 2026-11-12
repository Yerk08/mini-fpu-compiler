global A, B, f1, f2, f3, df1, df2, df3

section .text
f1:
    push ebp
    mov ebp, esp
    fld qword [const1]   ; 0.14
    fld qword [ebp + 8]  ; x
    fmulp st1, st0       ; (0.14 * x)
    fld qword [const2]   ; 3.1
    faddp st1, st0       ; ((0.14 * x) + 3.1)
    pop ebp
    ret

f2:
    push ebp
    mov ebp, esp
    fld qword [ebp + 8]  ; x
    fld qword [const3]   ; 2
    faddp st1, st0       ; (x + 2)
    fmul st0, st0        ; ((x + 2) * (x + 2))
    fld qword [ebp + 8]  ; x
    fld qword [const3]   ; 2
    faddp st1, st0       ; (x + 2)
    fmulp st1, st0       ; ((x + 2) * ((x + 2) * (x + 2)))
    fld qword [const4]   ; 1.9e+02
    fsubp                ; (((x + 2) * ((x + 2) * (x + 2))) - 1.9e+02)
    pop ebp
    ret

f3:
    push ebp
    mov ebp, esp
    fld qword [const5]   ; 3
    fld qword [ebp + 8]  ; x
    fdivp                ; (3 / x)
    fld qword [const6]   ; 2.7
    faddp st1, st0       ; ((3 / x) + 2.7)
    pop ebp
    ret

df1:
    push ebp
    mov ebp, esp
    fld qword [const1]   ; 0.14
    pop ebp
    ret

df2:
    push ebp
    mov ebp, esp
    fld qword [ebp + 8]  ; x
    fld qword [const3]   ; 2
    faddp st1, st0       ; (x + 2)
    fmul st0, st0        ; ((x + 2) * (x + 2))
    fld qword [const5]   ; 3
    fmulp st1, st0       ; (3 * ((x + 2) * (x + 2)))
    pop ebp
    ret

df3:
    push ebp
    mov ebp, esp
    fld qword [ebp + 8]  ; x
    fmul st0, st0        ; (x * x)
    fld qword [const7]   ; -3
    fdivrp               ; (-3 / (x * x))
    pop ebp
    ret

section .data
    A   dq 0.500000000000000
    B   dq 5.000000000000000
    const1   dq 0.140000000000000
    const2   dq 3.14159265358979
    const3   dq 2.00000000000000
    const4   dq 188.495559215388
    const5   dq 3.00000000000000
    const6   dq 2.71828182845905
    const7   dq -3.00000000000000

; functions.asm
; Copyright (C) 2026: Yeremey Pervunetskikh
;
; This program is free software: you can redistribute it
; and/or modify it under the terms of the GNU General Public License
