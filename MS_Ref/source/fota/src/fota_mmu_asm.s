; ============================================================================
;
; Enable the MMU function
;
; @Liu Jun 2005-07-27
; Spreadtrum Communications Inc.
;
; NOTE:
; 1. The ARM9 data cache must be used together with MMU, while the instruction
;    cache can be used independently.
;
; Related Files:
;      mmu_c.c          Create the MMU page table
;
; ============================================================================


    AREA mmu_functions, CODE, READONLY
    CODE32


;   Initialize the MMU
;
    IMPORT  g_mmu_page_table
;    IMPORT  mmu_create_page_table
    EXPORT  MMU_EnableIDCM
MMU_EnableIDCM
; save lr_USR and non-callee
	STMFD   sp!, {a1-a4, lr}               

;   Set the MMU page table address
    LDR     r2, =g_mmu_page_table
    LDR     r2, [r2]
    MCR     p15, 0, r2, c2, c0, 0
    
;   Domain Access Control: set all domains to manager
;
;   NOTE:
;   We must set domain access before enble MMU, otherwise Bus-Error will occur!
    LDR     r0, =0xFFFFFFFF
    MCR     p15, 0, r0, c3, c0, 0
    
;   Enable the ICache, DCache, write buffer, MMU
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x107D
    ORR     r0, r0, r1
    MCR     p15, 0, r0, c1, c0, 0 
    
;   Delay for the operations to finish
    NOP
    NOP
    NOP
    NOP
    NOP

;   Return
	LDMFD   sp!, {a1-a4, PC}             ; restore registers 

    EXPORT  MMU_DisableIDCM
MMU_DisableIDCM

; save lr_USR and non-callee
	STMFD   sp!, {a1-a4, lr}               

;   Disable the ICache, DCache, write buffer, MMU
    MRC     p15, 0, r0, c1, c0, 0
    LDR     r1, =0x1005
    BIC     r0, r0, r1
    ORR     r0, r0, #0x8     ;Bit_7 must be one when write c1;
    MCR     p15, 0, r0, c1, c0, 0

;write back data in data cache to memory system ;
tcv_loop
    MRC p15, 0, r15, c7, c14, 3 ; test clean and invalidate
	BNE tcv_loop

;   Invalidate the ICache and DCache
	MOV     r0, #0
    MCR     p15, 0, r0, c7, c7, 0

;   Delay for the operations to finish
    NOP
    NOP
    NOP

;   Return
	LDMFD   sp!, {a1-a4, PC}             ; restore registers 

    EXPORT  MMU_InvalideDCACHE
MMU_InvalideDCACHE

	STMFD sp!, {a1-a4, lr}               ; save lr_USR and non-callee

	;Test data cache state .
	MRC p15, 0, R0, c1, c0, 0 ; read control register
	AND R0,  R0,  #0x4
	CMP R0,  #0x0
	;If dcache is close ,to end ;
	BEQ tci_end

    ;Store irq and fiq and close them ;
	MRS     a4, CPSR                    ; Pickup current CPSR
	MOV     a3, #0xC0                   ; Build interrupt mask
	ORR     a2, a4, a3                  ; Or-in new interrupt lockout bits
	MSR     CPSR_c, a2                  ; Close all irq and fiq;

	;write data in chache to memory system  .
tcv_loop1 
    MRC p15, 0, r15, c7, c14, 3 ; test clean and invalidate
    BNE tcv_loop1

;restore irq and fiq ;
	MSR     CPSR_c, a4                  ; Close all irq and fiq;


tci_end
    LDMFD sp!, {a1-a4, PC}             ; restore registers    
         
     END