#include <machine/asm.h>

.intel_syntax
.global _C_LABEL(timer_int_handler)
.global _C_LABEL(contador)

.data
_C_LABEL(contador):
	.int 0 

.text
_C_LABEL(timer_int_handler):
		mov eax, _C_LABEL(contador)
		inc eax
		mov _C_LABEL(contador), eax
		ret
