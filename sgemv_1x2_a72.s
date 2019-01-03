// function: sgemv_1x2_a72
// data type: float
//
// input (1xp) * kernel (px2) + biases (1x2) = output (1x2)
//
// inputs:
//   x0 biases start address (1 x 2)
//   x1 input data start address (1 x p)
//   x2 kernel data start address (p x 2)
//   x3 kernel size (p)
//   x4 output data save address (1 x 2)
.section .text, "ax"
.align 5
.type sgemv_1x2_a72 %function
.global sgemv_1x2_a72

sgemv_1x2_a72:
  movi d30, 0
  prfm pldl1keep, [x1, 0x80]
  movi d31, 0
  prfm pldl1keep, [x2, 0x100]
  prfm pldl1keep, [x2, 0x140]
  cbz x0, start_mul
  ldr d30, [x0]

start_mul:
  lsr x9, x3, 2
  and x10, x3, 0x3
  cbz x9, loop4_end

loop4:
  ldr q25, [x1]
  ldp d26, d27, [x2]
  ldp d28, d29, [x2, 0x10]
  prfm pldl1keep, [x1, 0x100]
  subs x9, x9, 0x1
  fmla v30.2s, v26.2s, v25.s[0]
  add x1, x1, 0x10
  fmla v31.2s, v27.2s, v25.s[1]
  prfm pldl1keep, [x2, 0x200]
  add x2, x2, 0x20
  fmla v30.2s, v28.2s, v25.s[2]
  fmla v31.2s, v29.2s, v25.s[3]
  b.ne loop4
  fadd v30.2s, v30.2s, v31.2s

loop4_end:
  cbz x10, save_result

loop1:
  subs x10, x10, 0x1
  ldr s25, [x1], 0x4
  ldr d26, [x2], 0x8
  fmla v30.2s, v26.2s, v25.s[0]
  b.ne loop1

save_result:
  str d30, [x4]

  ret

.end

