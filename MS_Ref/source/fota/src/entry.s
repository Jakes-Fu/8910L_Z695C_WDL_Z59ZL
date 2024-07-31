;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and 
; interrupts for each mode, and finally branches to __main in the C library (which 
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

        PRESERVE8
        AREA    vect, CODE, READONLY


; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

Mode_USR        EQU     0x10
Mode_FIQ        EQU     0x11
Mode_IRQ        EQU     0x12
Mode_SVC        EQU     0x13
Mode_ABT        EQU     0x17
Mode_UNDEF      EQU     0x1B
Mode_SYS        EQU     0x1F ; available on ARM Arch 4 and later

I_Bit           EQU     0x80 ; when I bit2 is set, IRQ is disabled
F_Bit           EQU     0x40 ; when F bit is set, FIQ is disabled

;//Enable ARM can access all internal SRAM space 0x40000000 - 0x40010000
ACCESS_IRAM_ADDR      EQU     0x8B00002C
ACCESS_IRAM_FLAG      EQU     0x00000007
;//
;// Clear/Disabel interrupt.  write 0xFFFF to 0x8000000C, 
;// Please see arm_reg.h to get more information
CLEAR_INT_ADDR  EQU     0x8000000C
CLEAR_INT_FLAG  EQU     0xFFFFFFFF 

INTER_RAM_PCTRL EQU     0x4000FFFC


; --- System memory locations

        ENTRY
        
        IMPORT  svc_stack_bottom
        IMPORT  svc_heap_bottom
        IMPORT  irq_stack_bottom
        IMPORT  fiq_stack_bottom
        ;IMPORT  IRQ_Handler        
        ;IMPORT  FIQ_Handler 
        EXPORT  Reset_Handler
	CODE32
	
    EXPORT  __vectors
__vectors

        LDR     PC, =Reset_Handler		; Reset_Addr,Entry point
        LDR		PC, =Undefined_Addr		; Undefined_Addr
        LDR		PC, =SWI_Addr			; SWI_Addr
        LDR		PC, =Prefetch_Addr		; Prefetch_Addr
        LDR		PC, =Abort_Addr			; PC, Abort_Addr
        LDR		PC, =0x00000014			; Reserved vector
       ; LDR		PC, IRQ_Addr			; IRQ_Addr
       ; LDR		PC, FIQ_Addr			; FIQ_Addr	
        
Reset_Handler

	;// Clear/Disable interrupt.
    LDR     R0, =CLEAR_INT_ADDR
    LDR     R1, =CLEAR_INT_FLAG
    STR     R1, [R0]
    EOR     R0, R0, R0
    EOR     R1, R1, R1

    LDR     R0, =ACCESS_IRAM_ADDR
    LDR     R1, [R0]
    ORR     R1,R1,#0x7
    STR     R1,[R0]

	MOV		r0, #0
	MOV		r1, #0
	MOV		r2, #0
	MOV		r3, #0
	MOV		r4, #0
	MOV		r5, #0
	MOV		r6, #0
	MOV		r7, #0
	MOV		r8,  #0
	MOV		r9,  #0
	MOV		r10, #0
	MOV		r11, #0
	MOV		r12, #0
	MOV		sp, #0
	MOV		lr, #0
		
    MSR     CPSR_c, #Mode_IRQ:OR:I_Bit          ;// No interrupts
    LDR     R4, IRQ_STACK
    LDR     SP, [R4,#0x0]
    
    MSR     CPSR_c, #Mode_FIQ:OR:I_Bit:OR:F_Bit ;// No interrupts
    LDR     R4, FIQ_STACK
    LDR     SP, [R4,#0x0]
   
    MSR     CPSR_c, #Mode_SVC                   ;// Enable IRQ and FIQ
    LDR     R4, SVC_STACK
    LDR     SP, [R4,#0x0]
    
    
; Set up other stack pointers if necessary
        ; ...

; --- Initialise memory system
        ; ...

; --- Initialise critical IO devices
        ; ...

; --- Initialise interrupt system variables here
        ; ...
	IMPORT MMU_Init
	
	BL MMU_Init        
	
	IMPORT PM_Init
	
	BL PM_Init

	  	
        IMPORT __main

; --- Now enter the C code
      B      __main   ; note use B not BL, because an application will never return this way
        
        EXPORT   Jump_to_Target
Jump_to_Target
        MOV      pc, r0
        NOP
        NOP
        
        EXPORT reboot
reboot
		MOV	pc, r0
		NOP
        NOP	
        
Undefined_Addr
      b Undefined_Addr

SWI_Addr
      b SWI_Addr

Prefetch_Addr
      b Prefetch_Addr
 
Abort_Addr
      b Abort_Addr
            
  
    EXPORT __user_initial_stackheap

__user_initial_stackheap    
    
    LDR     R0, HEAP_BOTTOM
    LDR     R0, [R0,#0x0]
    
    MOV     pc,lr 

    EXPORT _sys_exit         
    
_sys_exit
    B _sys_exit
    

;IRQ_Addr        DCD     IRQ_Handler
;FIQ_Addr        DCD     FIQ_Handler   

IRQ_STACK     
    DCD     irq_stack_bottom
FIQ_STACK
    DCD     fiq_stack_bottom
SVC_STACK                               
    DCD     svc_stack_bottom
HEAP_BOTTOM
    DCD     svc_heap_bottom  
    
    
    
             

     END    



