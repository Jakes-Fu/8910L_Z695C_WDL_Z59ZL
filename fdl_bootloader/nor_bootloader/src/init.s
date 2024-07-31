;;; Copyright ARM Ltd 2000. All rights reserved.
;
; This module performs ROM/RAM remapping (if required), initializes stack pointers and 
; interrupts for each mode, and finally branches to __main in the C library (which 
; eventually calls main()).
;
; On reset, the ARM core starts up in Supervisor (SVC) mode, in ARM state, with IRQ and FIQ disabled.

    IMPORT  Reset_Handler
; --- Standard definitions of mode bits and interrupt (I & F) flags in PSRs

    PRESERVE8
    AREA    Init, CODE, READONLY

; --- System memory locations
    CODE32
    ENTRY
    EXPORT  __vectors
__vectors        
     IF (:DEF:SECURE_BOOT_SUPPORT) :LOR: (:DEF:SECURITY_DATA_SUPPORT)
    LDR     pc, =Reset_Handler 				;=Reset_Handler      ; Reset goes to startup function
     IF (:DEF:FOTA_SUPPORT_NONE)
    LDR     pc, =(0x00010004 + 1024)         ; Undefined handler
    LDR     pc, =(0x00010008 + 1024)         ; Software interrupt handler
    LDR     pc, =(0x0001000C + 1024)         ; Prefetch exeception handler
    LDR     pc, =(0x00010010 + 1024)         ; Abort exception handler
    LDR     pc, =(0x00010014 + 1024)         ; Reserved exception handler
    LDR     pc, =(0x00010018 + 1024)         ; IRQ interrupt handler
    LDR     pc, =(0x0001001C + 1024)         ; FIQ interrupt handler
     ELSE
    LDR     pc, =(0x00020004 + 1024)         ; Undefined handler
    LDR     pc, =(0x00020008 + 1024)         ; Software interrupt handler
    LDR     pc, =(0x0002000C + 1024)         ; Prefetch exeception handler
    LDR     pc, =(0x00020010 + 1024)         ; Abort exception handler
    LDR     pc, =(0x00020014 + 1024)         ; Reserved exception handler
    LDR     pc, =(0x00020018 + 1024)         ; IRQ interrupt handler
    LDR     pc, =(0x0002001C + 1024)         ; FIQ interrupt handler
     ENDIF
;;;1.Magic No
     IF (:DEF:PLATFORM_SC6531EFM)
                   DCD     0x36353632;;;magic number, do not changed again, it is fixed.
     ENDIF
;;;2.mHASHSize
     IF (:DEF:PLATFORM_SC6530) 
                   DCD     0xFFFFFFFF;;;HASH Size in case of 6531
     ENDIF      		   

                   DCD     0xFFFFFFFF;;; HASH Size;;;this part context can be changed in flashing or by diag command,here just reserved the space,you need modify it in flashing or by diag commmand,on sc6531efm platform
						;;;Inviald data,not used on sc6531 platform
;;;3.Flash Configuration Data
                   DCD     0x4D414748;;;SPIFLASH_MAGIC_FIRST 0x4D414748
                   DCD     0x5346432E;;;SPIFLASH_MAGIC_SECOND
    GBLA flash_config_data_count
flash_config_data_count         SETA         1
                   WHILE      flash_config_data_count <=9;;;9 can defined a macro, this part context can be changed in flashing,here just reserved the space,you need modify it in flashing.
                   DCD         0xFFFFFFFF
flash_config_data_count         SETA         flash_config_data_count +1
                   WEND
                   DCD         0x5441494C;;;SPIFLASH_MAGIC_END
;;;4.Customter Data
    GBLA customter_data_count
customter_data_count   SETA         1
                   WHILE      customter_data_count <=161;;;161*4=644 can defined a macro,this part context can be changed by diag comamd,you need modify the context in diag command,here just reserved the space, you can see the BOCA project.
                   DCD         0xFFFFFFFF
customter_data_count   SETA         customter_data_count +1
                   WEND
;;;5.RSA PUCK1
    GBLA rsa_puck1_data_count
rsa_puck1_data_count   SETA         1
                   WHILE      rsa_puck1_data_count <=65;;;65*4=260 can defined a macro,this part context can be changed by BSCGen tool,here just reserved the space.
                   DCD         0xFFFFFFFF
rsa_puck1_data_count   SETA         rsa_puck1_data_count +1
                   WEND
     ELSE  ;;;; NO SECURE SUPPORT (PLATFORM_UIX8910)
    LDR     pc, =Reset_Handler 				;=Reset_Handler      ; Reset goes to startup function
     IF (:DEF:FOTA_SUPPORT_NONE)
    LDR     pc, =(0x00010004)         ; Undefined handler    ///Nitish changed just to check if it can boot when we shift kernel start addr by 1024 bytes
    LDR     pc, =(0x00010008)         ; Software interrupt handler
    LDR     pc, =(0x0001000C)         ; Prefetch exeception handler
    LDR     pc, =(0x00010010)         ; Abort exception handler
    LDR     pc, =(0x00010014)         ; Reserved exception handler
    LDR     pc, =(0x00010018)         ; IRQ interrupt handler
    LDR     pc, =(0x0001001C)         ; FIQ interrupt handler
     ELSE
    LDR     pc, =(0x00020004)         ; Undefined handler    ///Nitish changed just to check if it can boot when we shift kernel start addr by 1024 bytes
    LDR     pc, =(0x00020008)         ; Software interrupt handler
    LDR     pc, =(0x0002000C)         ; Prefetch exeception handler
    LDR     pc, =(0x00020010)         ; Abort exception handler
    LDR     pc, =(0x00020014)         ; Reserved exception handler
    LDR     pc, =(0x00020018)         ; IRQ interrupt handler
    LDR     pc, =(0x0002001C)         ; FIQ interrupt handler
     ENDIF
     IF (:DEF:PLATFORM_SC6531EFM) :LOR: (:DEF:PLATFORM_UIX8910)
;;;Magic No
    		DCD     0x36353632;;;magic number;;;do not changed again, it is fixed.
     ENDIF    		    
     ENDIF
    END