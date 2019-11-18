#include <machine/asm.h>
#include "ibm_8042.h"

.intel_syntax
.global _C_LABEL(kbc_asm_ih)
.global _C_LABEL(scan_code)

.data
_C_LABEL(scan_code):
	.byte 0 

.text
_C_LABEL(kbc_asm_ih):
		mov bl, 5 /* contador */
cicle:	        cmp bl, 0
		jl nosucess
		in al, KBD_STATUS_PORT
		mov dl, al
		and dl, OBF
		je no	
		xor eax, eax	
		in al, KBD_OUT_BUF
		and dl, (PAR_ERR | TO_ERR)
		cmp dl,0
		je sucess
no:		dec bl 
		jmp cicle
sucess:		mov _C_LABEL(scan_code), al
		jmp end
nosucess:	mov dl, 2 
		mov _C_LABEL(scan_code), dl
		jmp end
end: 		ret
