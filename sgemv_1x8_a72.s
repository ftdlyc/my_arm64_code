// function: sgemv_1x8_a72
// data type: float
//
// input (1xp) * kernel (px8) + biases (1x8) = output (1x8)
//
// inputs:
//   x0 biases start address (1 x 8)
//   x1 input data start address (1 x p)
//   x2 kernel data start address (p x 8)
//   x3 kernel size (p)
//   x4 output data save address (1 x 8)
.section .text, "ax"
.align 5
.type sgemv_1x8_a72 %function
.global sgemv_1x8_a72

sgemv_1x8_a72:
  movi v30.2d, 0x0
  movi v31.2d, 0x0
  prfm pldl1keep, [x1, 0x100]
  prfm pldl1keep, [x2, 0x800]
  cbz x0, start_mul
  ldp q30, q31, [x0]

start_mul:
  lsr x9, x3, 0x2
  and x10, x3, 0x3
  cbz x9, loop4_end

loop4:
  ldr q21, [x1]
  ldp q22, q23, [x2]
  subs x9, x9, 0x1
  ldp q24, q25, [x2, 0x20]
  prfm pldl1keep, [x1, 0x100]
  add x1, x1, 0x10
  fmla v30.4s, v22.4s, v21.s[0]
  fmla v31.4s, v23.4s, v21.s[0]
  ldp q26, q27, [x2, 0x40]
  fmla v30.4s, v24.4s, v21.s[1]
  fmla v31.4s, v25.4s, v21.s[1]
  ldp q28, q29, [x2, 0x60]
  prfm pldl1keep, [x2, 0x800]
  fmla v30.4s, v26.4s, v21.s[2]
  fmla v31.4s, v27.4s, v21.s[2]
  add x2, x2, 0x80
  fmla v30.4s, v28.4s, v21.s[3]
  fmla v31.4s, v29.4s, v21.s[3]
  b.ne loop4

loop4_end:
  cbz x10, save_result

loop1:
  subs x10, x10, 0x1
  ldr q21, [x1], 0x4
  ldp q22, q23, [x2], 0x20
  fmla v30.4s, v22.4s, v21.s[0]
  fmla v31.4s, v23.4s, v21.s[0]
  b.ne loop1

save_result:
  stp q30, q31, [x4]

  ret

.end
