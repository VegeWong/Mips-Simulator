	.data
	.word 1
_static_121:
	.asciiz "\n"
	.align 2
_static_122:
	.word 0
_static_123:
	.word 0
_static_124:
	.word 0
_static_125:
	.word 0
_static_126:
	.word 0
_static_127:
	.word 0
_static_128:
	.word 0
_static_129:
	.word 0
_static_130:
	.word 0
	.data
_buffer:
	.word 0
	.text
_buffer_init:
	li $a0, 256
	li $v0, 9
	syscall
	sw $v0, _buffer
	jr $ra
	.text
main:
	jal _buffer_init
	add $fp, $zero, $sp
	li $8, 0
	sw $8, _static_123
	li $8, 110
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10

	syscall
	sw $v0, -12($fp)
	li $8, 110
	lw $9, -12($fp)
	sw $8, 0($9)
	add $10, $9, 4
	sw $10, _static_124
	li $8, 110
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10

	syscall
	sw $v0, -32($fp)
	li $8, 110
	lw $9, -32($fp)
	sw $8, 0($9)
	add $10, $9, 4
	sw $10, _static_125
	li $8, 110
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10

	syscall
	sw $v0, -52($fp)
	li $8, 110
	lw $9, -52($fp)
	sw $8, 0($9)
	add $10, $9, 4
	sw $10, _static_126
	jal _func_main
	move $a0, $v0
	li $v0, 17
	syscall
_func_____built_in_string_less:
	sw $fp, -212($sp)
	add $fp, $sp, -4
	add $sp, $fp, -216
	sw $ra, 0($sp)
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -4($fp)
	sub $12, $11, 4
	lw $13, 0($12)
	sw $10, -8($fp)
	ble $10, $13, _end_if_201
	lw $8, -4($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	sw $10, -8($fp)
_end_if_201:
	li $8, 0
	sw $8, -12($fp)
_begin_loop_135:
	lw $8, -12($fp)
	lw $9, -8($fp)
	bge $8, $9, _end_loop_135
	lw $8, 0($fp)
	lw $9, -12($fp)
	add $10, $8, $9
	lb $11, 0($10)
	lw $12, -4($fp)
	add $13, $12, $9
	lb $14, 0($13)
	sw $11, -16($fp)
	sw $14, -20($fp)
	beq $11, $14, _end_if_202
	lw $8, -16($fp)
	lw $9, -20($fp)
	slt $10, $8, $9
	move $v0, $10
	sw $10, -76($fp)
	b _end_func_____built_in_string_less
_end_if_202:
_continue_loop135:
	lw $8, -12($fp)
	add $8, $8, 1
	sw $8, -12($fp)
	b _begin_loop_135
_end_loop_135:
	lw $8, -4($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -8($fp)
	slt $12, $11, $10
	move $v0, $12
	sw $12, -88($fp)
	b _end_func_____built_in_string_less
_end_func_____built_in_string_less:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -208($fp)
	jr $ra

_func_____built_in_string_equal:
	sw $fp, -188($sp)
	add $fp, $sp, -4
	add $sp, $fp, -192
	sw $ra, 0($sp)
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -4($fp)
	sub $12, $11, 4
	lw $13, 0($12)
	beq $10, $13, _end_if_203
	move $v0, $zero
	b _end_func_____built_in_string_equal
_end_if_203:
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	li $11, 0
	sw $10, -12($fp)
	sw $11, -8($fp)
_begin_loop_136:
	lw $8, -8($fp)
	lw $9, -12($fp)
	bge $8, $9, _end_loop_136
	lw $8, 0($fp)
	lw $9, -8($fp)
	add $10, $8, $9
	lb $11, 0($10)
	lw $12, -4($fp)
	add $13, $12, $9
	lb $14, 0($13)
	beq $11, $14, _end_if_204
	move $v0, $zero
	b _end_func_____built_in_string_equal
_end_if_204:
_continue_loop136:
	lw $8, -8($fp)
	add $8, $8, 1
	sw $8, -8($fp)
	b _begin_loop_136
_end_loop_136:
	li $8, 1
	move $v0, $8
	b _end_func_____built_in_string_equal
_end_func_____built_in_string_equal:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -184($fp)
	jr $ra

_func_____built_in_string_inequal:
	sw $fp, -188($sp)
	add $fp, $sp, -4
	add $sp, $fp, -192
	sw $ra, 0($sp)
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -4($fp)
	sub $12, $11, 4
	lw $13, 0($12)
	beq $10, $13, _end_if_205
	li $8, 1
	move $v0, $8
	b _end_func_____built_in_string_inequal
_end_if_205:
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	li $11, 0
	sw $10, -12($fp)
	sw $11, -8($fp)
_begin_loop_137:
	lw $8, -8($fp)
	lw $9, -12($fp)
	bge $8, $9, _end_loop_137
	lw $8, 0($fp)
	lw $9, -8($fp)
	add $10, $8, $9
	lb $11, 0($10)
	lw $12, -4($fp)
	add $13, $12, $9
	lb $14, 0($13)
	beq $11, $14, _end_if_206
	li $8, 1
	move $v0, $8
	b _end_func_____built_in_string_inequal
_end_if_206:
_continue_loop137:
	lw $8, -8($fp)
	add $8, $8, 1
	sw $8, -8($fp)
	b _begin_loop_137
_end_loop_137:
	move $v0, $zero
	b _end_func_____built_in_string_inequal
_end_func_____built_in_string_inequal:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -184($fp)
	jr $ra

_func_____built_in_string_greater:
	sw $fp, -132($sp)
	add $fp, $sp, -4
	add $sp, $fp, -136
	sw $ra, 0($sp)

	lw $8, -4($fp)
	sw $8, -4($sp)

	lw $9, 0($fp)
	sw $9, -8($sp)

jal _func_____built_in_string_less
	sw $v0, -8($fp)
	lw $8, -8($fp)
	move $v0, $8
	b _end_func_____built_in_string_greater
_end_func_____built_in_string_greater:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -128($fp)
	jr $ra

_func_____built_in_string_less_equal:
	sw $fp, -220($sp)
	add $fp, $sp, -4
	add $sp, $fp, -224
	sw $ra, 0($sp)
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -4($fp)
	sub $12, $11, 4
	lw $13, 0($12)
	sw $10, -8($fp)
	ble $10, $13, _end_if_207
	lw $8, -4($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	sw $10, -8($fp)
_end_if_207:
	li $8, 0
	sw $8, -12($fp)
_begin_loop_138:
	lw $8, -12($fp)
	lw $9, -8($fp)
	bge $8, $9, _end_loop_138
	lw $8, 0($fp)
	lw $9, -12($fp)
	add $10, $8, $9
	lb $11, 0($10)
	lw $12, -4($fp)
	add $13, $12, $9
	lb $14, 0($13)
	sw $11, -16($fp)
	sw $14, -20($fp)
	beq $11, $14, _end_if_208
	lw $8, -16($fp)
	lw $9, -20($fp)
	slt $10, $8, $9
	move $v0, $10
	sw $10, -76($fp)
	b _end_func_____built_in_string_less_equal
_end_if_208:
_continue_loop138:
	lw $8, -12($fp)
	add $8, $8, 1
	sw $8, -12($fp)
	b _begin_loop_138
_end_loop_138:
	lw $8, 0($fp)
	sub $9, $8, 4
	lw $10, 0($9)
	lw $11, -4($fp)
	sub $12, $11, 4
	lw $13, 0($12)
	sle $14, $10, $13
	move $v0, $14
	sw $14, -96($fp)
	b _end_func_____built_in_string_less_equal
_end_func_____built_in_string_less_equal:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -216($fp)
	jr $ra

_func_____built_in_string_greater_equal:
	sw $fp, -132($sp)
	add $fp, $sp, -4
	add $sp, $fp, -136
	sw $ra, 0($sp)

	lw $8, -4($fp)
	sw $8, -4($sp)

	lw $9, 0($fp)
	sw $9, -8($sp)

jal _func_____built_in_string_less_equal
	sw $v0, -8($fp)
	lw $8, -8($fp)
	move $v0, $8
	b _end_func_____built_in_string_greater_equal
_end_func_____built_in_string_greater_equal:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -128($fp)
	jr $ra

_func_____built_in_println:
	sw $fp, -128($sp)
	add $fp, $sp, -4
	add $sp, $fp, -132
	sw $ra, 0($sp)

	lw $8, 0($fp)
	li $v0, 4
	move $a0, $8

	syscall
	la $8, _static_121
	move $9, $8
	li $v0, 4
	move $a0, $9

	syscall
_end_func_____built_in_println:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -124($fp)
	jr $ra

_func_origin:
	sw $fp, -200($sp)
	add $fp, $sp, -4
	add $sp, $fp, -204
	sw $ra, 0($sp)
	lw $8, 0($fp)
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10

	syscall
	sw $v0, -12($fp)
	lw $8, 0($fp)
	lw $9, -12($fp)
	sw $8, 0($9)
	add $10, $9, 4
	li $11, 0
	sw $10, _static_122
	sw $11, _static_127
_begin_loop_139:
	lw $8, _static_127
	lw $9, 0($fp)
	bge $8, $9, _end_loop_139
	lw $8, 0($fp)
	add $9, $8, 1
	mul $10, $9, 4
	li $v0, 9
	move $a0, $10

	syscall
	sw $v0, -40($fp)
	lw $8, 0($fp)
	lw $9, -40($fp)
	sw $8, 0($9)
	add $10, $9, 4
	lw $11, _static_127
	mul $12, $11, 4
	lw $13, _static_122
	add $14, $13, $12
	sw $10, 0($14)
	li $15, 0
	sw $15, _static_128
_begin_loop_140:
	lw $8, _static_128
	lw $9, 0($fp)
	bge $8, $9, _end_loop_140
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_122
	add $11, $10, $9
	lw $12, 0($11)
	lw $13, _static_128
	mul $14, $13, 4
	add $15, $12, $14
	sw $zero, 0($15)
_continue_loop140:
	lw $8, _static_128
	add $8, $8, 1
	move $8, $8
	sw $8, _static_128
	b _begin_loop_140
_end_loop_140:
_continue_loop139:
	lw $8, _static_127
	add $8, $8, 1
	move $8, $8
	sw $8, _static_127
	b _begin_loop_139
_end_loop_139:
_end_func_origin:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -196($fp)
	jr $ra

_func_build:
	sw $fp, -276($sp)
	add $fp, $sp, -4
	add $sp, $fp, -280
	sw $ra, 0($sp)
	li $8, 1
	sw $8, _static_127
_begin_loop_141:
	lw $8, _static_127
	bgt $8, 49, _end_loop_141
	li $8, 50
	sw $8, _static_128
_begin_loop_142:
	li $8, 98
	lw $9, _static_127
	sub $10, $8, $9
	add $11, $10, 1
	lw $12, _static_128
	bgt $12, $11, _end_loop_142
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_122
	add $11, $10, $9
	lw $12, 0($11)
	lw $13, _static_128
	mul $14, $13, 4
	add $15, $12, $14
	li $16, 1
	sw $16, 0($15)
_continue_loop142:
	lw $8, _static_128
	add $8, $8, 1
	move $8, $8
	sw $8, _static_128
	b _begin_loop_142
_end_loop_142:
_continue_loop141:
	lw $8, _static_127
	add $8, $8, 1
	move $8, $8
	sw $8, _static_127
	b _begin_loop_141
_end_loop_141:
	li $8, 1
	sw $8, _static_127
_begin_loop_143:
	lw $8, _static_127
	bgt $8, 49, _end_loop_143
	lw $8, 0($fp)
	mul $9, $8, 4
	lw $10, _static_122
	add $11, $10, $9
	lw $12, 0($11)
	lw $13, _static_127
	mul $14, $13, 4
	add $15, $12, $14
	li $16, 1
	sw $16, 0($15)
_continue_loop143:
	lw $8, _static_127
	add $8, $8, 1
	move $8, $8
	sw $8, _static_127
	b _begin_loop_143
_end_loop_143:
	li $8, 50
	sw $8, _static_127
_begin_loop_144:
	lw $8, _static_127
	bgt $8, 98, _end_loop_144
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_122
	add $11, $10, $9
	lw $12, 0($11)
	lw $13, -4($fp)
	mul $14, $13, 4
	add $15, $12, $14
	li $16, 1
	sw $16, 0($15)
_continue_loop144:
	lw $8, _static_127
	add $8, $8, 1
	move $8, $8
	sw $8, _static_127
	b _begin_loop_144
_end_loop_144:
	move $v0, $zero
	b _end_func_build
_end_func_build:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -272($fp)
	jr $ra

_func_find:
	sw $fp, -316($sp)
	add $fp, $sp, -4
	add $sp, $fp, -320
	sw $ra, 0($sp)
	li $8, 0
	li $9, 1
	li $10, 1
	sw $8, _static_129
	sw $9, _static_130
	sw $10, _static_127
_begin_loop_145:
	lw $8, _static_127
	lw $9, 0($fp)
	bgt $8, $9, _end_loop_145
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_124
	add $11, $10, $9
	sw $zero, 0($11)
_continue_loop145:
	lw $8, _static_127
	add $8, $8, 1
	move $8, $8
	sw $8, _static_127
	b _begin_loop_145
_end_loop_145:
	li $8, 1
	mul $9, $8, 4
	lw $10, _static_126
	add $11, $10, $9
	lw $12, -4($fp)
	sw $12, 0($11)
	mul $13, $12, 4
	lw $14, _static_124
	add $15, $14, $13
	li $16, 1
	sw $16, 0($15)
	mul $17, $12, 4
	lw $18, _static_125
	add $19, $18, $17
	sw $zero, 0($19)
	li $20, 0
	sw $20, -8($fp)
_continue_loop146:
	lw $8, _static_129
	lw $9, _static_130
	slt $10, $8, $9
	move $11, $10
	sw $11, -68($fp)
	beqz $11, _short_evaluate_33
	lw $8, -8($fp)
	seq $9, $8, 0
	move $10, $9
	sw $10, -68($fp)
_short_evaluate_33:
	lw $8, -68($fp)
	beqz $8, _end_loop_146
	lw $8, _static_129
	add $8, $8, 1
	move $8, $8
	mul $9, $8, 4
	lw $10, _static_126
	add $11, $10, $9
	lw $12, 0($11)
	li $13, 1
	sw $8, _static_129
	sw $12, _static_127
	sw $13, _static_128
_begin_loop_147:
	lw $8, _static_128
	lw $9, 0($fp)
	bgt $8, $9, _end_loop_147
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_122
	add $11, $10, $9
	lw $12, 0($11)
	lw $13, _static_128
	mul $14, $13, 4
	add $15, $12, $14
	lw $16, 0($15)
	sgt $17, $16, 0
	move $18, $17
	sw $18, -104($fp)
	beqz $18, _short_evaluate_34
	lw $8, _static_128
	mul $9, $8, 4
	lw $10, _static_124
	add $11, $10, $9
	lw $12, 0($11)
	seq $13, $12, 0
	move $14, $13
	sw $14, -104($fp)
_short_evaluate_34:
	lw $8, -104($fp)
	beqz $8, _end_if_209
	lw $8, _static_128
	mul $9, $8, 4
	lw $10, _static_124
	add $11, $10, $9
	li $12, 1
	sw $12, 0($11)
	lw $13, _static_130
	add $13, $13, 1
	move $13, $13
	mul $14, $13, 4
	lw $15, _static_126
	add $16, $15, $14
	sw $8, 0($16)
	mul $17, $8, 4
	lw $18, _static_125
	add $19, $18, $17
	lw $20, _static_127
	sw $20, 0($19)
	lw $21, 0($fp)
	sw $13, _static_130
	bne $13, $21, _end_if_210
	li $8, 1
	sw $8, -8($fp)
_end_if_210:
_end_if_209:
_continue_loop147:
	lw $8, _static_128
	add $8, $8, 1
	move $8, $8
	sw $8, _static_128
	b _begin_loop_147
_end_loop_147:
	b _continue_loop146
_end_loop_146:
	lw $8, -8($fp)
	move $v0, $8
	b _end_func_find
_end_func_find:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -312($fp)
	jr $ra

_func_improve:
	sw $fp, -212($sp)
	add $fp, $sp, -4
	add $sp, $fp, -216
	sw $ra, 0($sp)
	lw $8, 0($fp)
	lw $9, _static_123
	add $9, $9, 1
	move $9, $9
	sw $8, _static_127
	sw $9, _static_123
_continue_loop148:
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_125
	add $11, $10, $9
	lw $12, 0($11)
	ble $12, 0, _end_loop_148
	lw $8, _static_127
	mul $9, $8, 4
	lw $10, _static_125
	add $11, $10, $9
	lw $12, 0($11)
	mul $13, $12, 4
	lw $14, _static_122
	add $15, $14, $13
	lw $16, 0($15)
	mul $17, $8, 4
	add $18, $16, $17
	lw $19, 0($18)
	sub $19, $19, 1
	sw $19, 0($18)
	mul $20, $8, 4
	add $21, $14, $20
	lw $22, 0($21)
	mul $23, $12, 4
	add $24, $22, $23
	lw $25, 0($24)
	add $25, $25, 1
	sw $25, 0($24)
	move $8, $12
	sw $8, _static_127
	sw $12, _static_128
	sw $19, -56($fp)
	sw $25, -84($fp)
	b _continue_loop148
_end_loop_148:
	move $v0, $zero
	b _end_func_improve
_end_func_improve:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -208($fp)
	jr $ra

_func_main:
	sw $fp, -172($sp)
	add $fp, $sp, -4
	add $sp, $fp, -176
	sw $ra, 0($sp)

	li $8, 110
	sw $8, -4($sp)

jal _func_origin
	li $8, 0
	li $9, 99
	li $10, 100
	li $11, 0
	sw $9, -4($sp)
	sw $10, -8($sp)
	sw $8, 0($fp)
	sw $9, -4($fp)
	sw $10, -8($fp)
	sw $11, -12($fp)
jal _func_build
	sw $v0, -32($fp)
_continue_loop149:

	lw $8, -8($fp)
	sw $8, -4($sp)

	lw $9, -4($fp)
	sw $9, -8($sp)

	lw $10, -12($fp)
	sw $10, -12($sp)

jal _func_find
	sw $v0, -36($fp)
	lw $8, -36($fp)
	ble $8, 0, _end_loop_149

	lw $8, -8($fp)
	sw $8, -4($sp)

jal _func_improve
	sw $v0, -44($fp)
	b _continue_loop149
_end_loop_149:

	lw $8, _static_123
	sw $8, -4($sp)

jal _func_____built_in_toString
	sw $v0, -48($fp)

	lw $8, -48($fp)
	li $v0, 4
	move $a0, $8

	syscall

	la $8, _static_121
	li $v0, 4
	move $a0, $8

	syscall
	move $v0, $zero
	b _end_func_main
_end_func_main:
	lw $ra, 0($sp)
	add $sp, $fp, 4
	lw $fp, -168($fp)
	jr $ra

	.text
_string_copy:
	_begin_string_copy:
	lb $v0, 0($a0)
	beqz $v0, _exit_string_copy
	sb $v0, 0($a1)
	add $a0, $a0, 1
	add $a1, $a1, 1
	j _begin_string_copy
	_exit_string_copy:
	sb $zero, 0($a1)
	jr $ra

_func_____built_in_substring:
	lw $a0, -4($sp)
	lw $a1, -8($sp)
	lw $a2, -12($sp)
	subu $sp, $sp, 4
	sw $ra, 0($sp)

	move $t0, $a0

	sub $t1, $a2, $a1
	add $t1, $t1, 1
	add $a0, $t1, 5
	li $v0, 9
	syscall
	sw $t1, 0($v0)
	add $v0, $v0, 4

	add $a0, $t0, $a1
	add $t2, $t0, $a2
	lb $t3, 1($t2)
	sb $zero, 1($t2)
	move $a1, $v0
	move $t4, $v0
	jal _string_copy
	move $v0, $t4
	sb $t3, 1($t2)

	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra

_count_string_length:
	move $v0, $a0

	_begin_count_string_length:
	lb $v1, 0($a0)
	beqz $v1, _exit_count_string_length
	add $a0, $a0, 1
	j _begin_count_string_length

	_exit_count_string_length:
	sub $v0, $a0, $v0
	jr $ra

_func_____built_in_getString:
	subu $sp, $sp, 4
	sw $ra, 0($sp)

	lw $a0, _buffer
	li $a1, 255
	li $v0, 8
	syscall

	jal _count_string_length

	move $a1, $v0
	add $a0, $v0, 5
	li $v0, 9
	syscall
	sw $a1, 0($v0)
	add $v0, $v0, 4
	lw $a0, _buffer
	move $a1, $v0
	move $t0, $v0
	jal _string_copy
	move $v0, $t0

	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra

_func_____built_in_parseInt:
	lw $a0, -4($sp)
	li $v0, 0
	move $t0, $a0
	li $t2, 1

	_count_number_pos:
	lb $t1, 0($t0)
	bgt $t1, 57, _begin_parse_int
	blt $t1, 48, _begin_parse_int
	add $t0, $t0, 1
	j _count_number_pos

	_begin_parse_int:
	sub $t0, $t0, 1

	_parsing_int:
	blt $t0, $a0, _finish_parse_int
	lb $t1, 0($t0)
	sub $t1, $t1, 48
	mul $t1, $t1, $t2
	add $v0, $v0, $t1
	mul $t2, $t2, 10
	sub $t0, $t0, 1
	j _parsing_int

	_finish_parse_int:
	jr $ra

_func_____built_in_string_concatenate:
	lw $a0, -4($sp)
	lw $a1, -8($sp)
	subu $sp, $sp, 4
	sw $ra, 0($sp)

	move $t2, $a0
	move $t3, $a1

	lw $t0, -4($a0)
	lw $t1, -4($a1)
	add $t5, $t0, $t1
	add $a0, $t5, 5
	li $v0, 9
	syscall
	sw $t5, 0($v0)
	add $v0, $v0, 4
	move $t4, $v0

	move $a0, $t2
	move $a1, $t4
	jal _string_copy

	move $a0, $t3
	add $a1, $t4, $t0
	
	jal _string_copy

	move $v0, $t4
	lw $ra, 0($sp)
	addu $sp, $sp, 4
	jr $ra

_func_____built_in_toString:
	lw $a0, -4($sp)
	li $t0, 0
	bgez $a0, _skip_set_less_than_zero
	li $t0, 1
	neg $a0, $a0
	_skip_set_less_than_zero:
	beqz $a0, _set_zero

	li $t1, 0
	move $t2, $a0
	move $t3, $a0
	li $t5, 10

	_begin_count_digit:
	div $t2, $t5
	mflo $v0
	mfhi $v1
	bgtz $v0, _not_yet
	bgtz $v1, _not_yet
	j _yet
	_not_yet:
	add $t1, $t1, 1
	move $t2, $v0
	j _begin_count_digit

	_yet:
	beqz $t0, _skip_reserve_neg
	add $t1, $t1, 1
	_skip_reserve_neg:
	add $a0, $t1, 5
	li $v0, 9
	syscall
	sw $t1, 0($v0)
	add $v0, $v0, 4
	add $t1, $t1, $v0
	sb $zero, 0($t1)
	sub $t1, $t1, 1

	_continue_toString:
	div $t3, $t5
	mfhi $v1
	add $v1, $v1, 48
	sb $v1, 0($t1)
	sub $t1, $t1, 1
	mflo $t3
	
	bnez $t3, _continue_toString

	beqz $t0, _skip_place_neg
	li $v1, 45
	sb $v1, 0($t1)
	_skip_place_neg:
	
	jr $ra

	_set_zero:
	li $a0, 6
	li $v0, 9
	syscall
	li $a0, 1
	sw $a0, 0($v0)
	add $v0, $v0, 4
	li $a0, 48
	sb $a0, 0($v0)
	jr $ra
