addi    t4, zero, 0x40 // M = 64
addi    t5, zero, 0x3c // N = 60
addi    t6, zero, 0x20 // K = 32
addi    s1, zero, 0x100 // pa = a; a = start = 0x100;
lui     s2, 0x1 // pc = 0x1000; pc = c - 0x800; c = b + sizeof(int16_t) * N * K = 0x900 + 0x2 * 0x3c * 0x20 = 0x1800;
addi    s2, s2, 0x7ff // pc = pc + 0x7ff; pc = c - 1;
addi    s2, s2, 0x1 // pc = pc + 0x1; pc = c;
add     t0, zero, zero // y = 0;
bge     t0, t4, 132 // exit y-cycle if y >= M that consists of 33 operations including this
add     t1, zero, zero // x = 0;
bge     t1, t5, 96 // exit x-cycle if x >= N that consists of 24 operations including this
addi    s3, zero, 0x7ff // pb = 0x7ff; pb = b - 0x101; b = a + sizeof(int8_t) * K * M = 0x100 + 0x1 * 0x20 * 0x40 = 0x900;
addi    s3, s3, 0x101 // pb = pb + 0x101; pb = b;
add     s4, zero, zero // s = 0;
add     t2, zero, zero // k = 0;
bge     t2, t6, 48 // exit k-cycle if k >= K that consists of 12 operations including this
add     s9, s1, t2 // &pa[k] = pa + k;
lb      s6, 0(s9) // t_8 = pa[k];
add     s10, s3, t1 // &pb[x / 2] = pb + x; because pb points to 16-bit integer
add     s10, s10, t1 // &pb[x] = &pb[x / 2] + x;
lh      s7, 0(s10) // t_16 = pb[x];
mul     t3, s6, s7 // t = t_8 * t_16 = pa[k] * pb[x];
add     s4, s4, t3 // s = s + t = s + pa[k] * pb[x];
add     s3, s3, t5 // pb = pb + N; because pb points to 16-bit integer
add     s3, s3, t5 // pb = pb + N;
addi    t2, t2, 0x1 // k++;
jal     zero, -44 // repeat k-cycle that consists of 11 operations not including this
add     s11, s2, t1 // &pc[x / 4] = pc + x; because pc points to 32-bit integer
add     s11, s11, t1 // &pc[x / 2] = &pc[x / 4] + x;
add     s11, s11, t1 // &pc[3x / 4] =  &pc[x / 2] + x;
add     s11, s11, t1 // &pc[x] = &pc[3x / 4] + x;
sw      s4, 0(s11) // pc[x] = s;
addi    t1, t1, 0x1 // x++;
jal     zero, -92 // repeat x-cycle that consists of 23 operations not including this
add     s1, s1, t6 // pa = pa + K;
add     s2, s2, t5 // pc = pc + N; because pc points to 32-bit integer
add     s2, s2, t5 // pc = pc + N;
add     s2, s2, t5 // pc = pc + N;
add     s2, s2, t5 // pc = pc + N;
addi    t0, t0, 0x1 // y++;
jal     zero, -128 // repeat y-cycle that consists of 32 operations not including this
jalr    zero, ra, 0 // exit from function