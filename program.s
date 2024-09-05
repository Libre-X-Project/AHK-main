[BITS 32]


start:
	; set eax to some distinguishable number, to read from the log afterwards
	mov eax, 0xDEADBEEF

	jmp start