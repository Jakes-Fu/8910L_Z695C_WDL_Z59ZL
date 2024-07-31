 
 AREA    reboot, CODE, READONLY 
    
 	CODE32
	EXPORT remap_and_reboot
remap_and_reboot

	; disable I/D cache
	IMPORT MMU_DisableIDCM
	bl MMU_DisableIDCM
	
	; remap
	LDR r1, =0x20900218
	MOV r2, #1
	STR r2, [r1]
	
	; reconfig ARM endian
  MRC p15,0,r0,c1,c0,0
  IF (:DEF:CHIP_ENDIAN_DEFAULT_BIG)
  ORR r0,r0,#0x80				            ; Set ARM926EJ to big endian mode ;	
  ELSE 
  IF (:DEF:CHIP_ENDIAN_DEFAULT_LITTLE)
  BIC r0,r0,#0x80				            ; Set ARM926EJ to little endian mode ;			
  ENDIF
  ENDIF
  
  ; jump to address 0
  MCR p15,0,r0,c1,c0,0
	MOV r0, #0	
	MOV	pc, r0
	
	
       
    END