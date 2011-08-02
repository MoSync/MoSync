.sourcefile 'mastack.s'
.line 1

.code
.align 4
.global _getStackTop
.func _getStackTop

	ld r14, fr
	sub r14, #8
	ret
