MAIN: mov r3 ,LENGTH
LOOP: jmp L1(#-1,r6)
prn #-5
LENGTH: .data 0,-9,43,-276
bne LOOP(r4,r5)
sub r1, r4
bne END
L1: inc K
bne LOOP(K,STR)
END: stop
STR: .string "rgdjyf"
K: .data 22