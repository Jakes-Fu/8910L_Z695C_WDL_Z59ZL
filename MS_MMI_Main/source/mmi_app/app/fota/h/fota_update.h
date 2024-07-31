
#ifndef _MMIFOTA_UPDATE_H_ 
#define _MMIFOTA_UPDATE_H_

#ifdef FOTA_SUPPORT

PUBLIC BOOLEAN MMIFOTA_CheckApn(void);
PUBLIC void MMIFOTA_UpdateApn(void);

PUBLIC BOOLEAN MMIFOTA_CheckConfigNV(void);
PUBLIC void MMIFOTA_UpdateConfigNV(void);

PUBLIC BOOLEAN MMIFOTA_CheckIMS(void);
PUBLIC void MMIFOTA_UpdateIMS(void);


#endif
#endif

